#include "stdafx.h"
#include "ObjectSelection.h"
#include "SceneInteraction.h"
#include "Player.h"
#include "EditorControl.h"

void ObjectSelection::uiSelectItem(SelectWorldItemData& data)
{
	if(data.groupName == "Entity")
		if (Global::mSceneMgr->hasEntity(std::string(data.item.name.begin(), data.item.name.end())))
		{
			setSelectedEntity(Global::mSceneMgr->getEntity(std::string(data.item.name.begin(), data.item.name.end())));
		}

	if (data.groupName.empty() && selected)
	{
		if(!selected->filter(wtos(data.item.name)))
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

void ObjectSelection::init(EditorControl* p)
{
	parent = p;
	gizmo.init(this);
	addMode = false;
}

void ObjectSelection::setMode(SelectionMode mode)
{
	if (mode == SelectionMode::Select)
		addMode = false;

	gizmo.setMode(mode);
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
		Global::camera->camera->getParentSceneNode()->lookAt(selected->getPosition(), Ogre::Node::TS_WORLD);
	}
}

void ObjectSelection::uiEditSelection(SelectionInfoChange* change)
{
	if (selected)
	{
		Ogre::Vector3* vec = (Ogre::Vector3*)change->data;
		switch (change->change)
		{
		case SelectionInfoChange::SelectionChange::Pos:
			selected->setPosition(*vec);
			break;
		case SelectionInfoChange::SelectionChange::Scale:
			selected->setScale(*vec);
			break;
		default:
			break;
		}
	}
}

