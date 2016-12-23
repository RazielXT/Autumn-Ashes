#include "stdafx.h"
#include "ObjectSelection.h"
#include "SceneInteraction.h"
#include "Player.h"
#include "EditorControl.h"
#include "GUtils.h"
#include "GeometryManager.h"
#include "GameStateManager.h"

void ObjectSelection::uiSelectItem(SelectWorldItemData& data)
{
	auto name = wtos(data.item.name);

	if(data.groupName == "Entity")
		if (Global::sceneMgr->hasEntity(name))
		{
			setSelectedEntity(Global::sceneMgr->getEntity(name));
		}

	if (data.groupName == "Grass")
	{
		auto grasses = Global::gameMgr->geometryMgr->getPagedGrasses();
		for (auto&g : grasses)
		{
			if(g.name == name)
				setSelectedGrass(g);
		}
	}

	if (data.groupName.empty() && selected)
	{
		if(!selected->filter(name))
			setSelectedEntity(nullptr);
		else
			updateUiSelectedInfo();
	}
}

void ObjectSelection::removeSelection()
{
	if (selected)
	{
		selected->remove();
		setSelectedEntity(nullptr);
	}
}

ObjectSelection::ObjectSelection(EditorControl* control) : parent(control), selectedGrasses(&control->painter)
{

}

void ObjectSelection::init()
{
	gizmo.init(this);
	addMode = false;

	if(selected)
		selected->reset();
	selected = nullptr;
	gizmo.setRoot(nullptr);

	updateUiSelectedInfo();
}

void ObjectSelection::setMode(SelectionMode mode)
{
	if (mode == SelectionMode::Select)
		addMode = false;

	gizmo.setMode(mode);
}

void ObjectSelection::setSelectedGrass(GrassInfo& grass, bool forceDeselect)
{
	if (selected && (forceDeselect || selected != &selectedGrasses))
		selected->deselect();

	selected = &selectedGrasses;
	selectedGrasses.add(grass);

	gizmo.setRoot(selected);
	updateUiSelectedInfo();
}

void ObjectSelection::setSelectedEntity(Ogre::Entity* ent, bool forceDeselect)
{
	if (selected && (forceDeselect || selected != &selectedEntities))
		selected->deselect();

	if (ent)
	{
		selected = &selectedEntities;
		selectedEntities.add(ent);
	}
	else
		selected = nullptr;

	gizmo.setRoot(selected);
	updateUiSelectedInfo();
}

void ObjectSelection::updateUiSelectedInfo()
{
	parent->displayItemInfo(selected);
}

Ogre::Entity* ObjectSelection::pickMouseRayItem()
{
	auto ent = SceneInteraction::PickEntity(SceneInteraction::getMouseRay());
	setSelectedEntity(ent, !addMode);

	return ent;
}

void ObjectSelection::lootAtSelection()
{
	if (selected)
	{
		Global::camera->cam->getParentSceneNode()->lookAt(selected->getPosition(), Ogre::Node::TS_WORLD);
	}
}

void ObjectSelection::rayPlaceSelection(float offset)
{
	if (selected)
	{
		auto pos = selected->getIndividualPositions();

		for (auto& p : pos)
		{
			if(GUtils::getVerticalRayPos(p, 5, 100))
				p.y += offset;
		}

		selected->setIndividualPositions(pos);
	}
}

void ObjectSelection::uiEditSelection(SelectionInfoChange* change)
{
	if (selected)
	{
		Ogre::Vector3* vec = (Ogre::Vector3*)change->data;

		switch (change->change)
		{
		case SelectionInfoChange::Id::Pos:
			selected->setPosition(*vec);
			break;
		case SelectionInfoChange::Id::Scale:
			selected->setScale(*vec);
			break;
		default:
			selected->handleSelectionMessage(change);
			break;
		}
	}
}

