#include "stdafx.h"
#include "EditorGrass.h"
#include "EditorUiHandler.h"
#include "GUtils.h"
#include "GameStateManager.h"
#include "GrassHeightFunction.h"


EditorGrass::EditorGrass()
{
	moveOffset = Ogre::Vector3::ZERO;
	scaleOffset = Ogre::Vector3(1, 1, 1);
}

void EditorGrass::reset()
{
	selected.clear();
	boundsNode = nullptr;
}

void EditorGrass::setPosition(Ogre::Vector3& pos)
{
	auto translate = getPosition();
	translate = pos - translate;

	for (auto& e : selected)
	{
		auto ePos = e.pg->getSceneNode()->getPosition();
		setGrassPosition(e, ePos + translate);
	}
}

void EditorGrass::setScale(Ogre::Vector3& scale)
{
	for (auto& e : selected)
	{
		//e->getParentSceneNode()->setScale(scale);
	}
}

Ogre::Vector3 EditorGrass::getPosition()
{
	return getVisualBounds().getCenter() + moveOffset;
}

std::vector<Ogre::Vector3> EditorGrass::getIndividualPositions()
{
	std::vector<Ogre::Vector3> out;

	for (auto& e : selected)
	{
		out.push_back(e.pg->getSceneNode()->getPosition());
	}

	return out;
}

void EditorGrass::setIndividualPositions(std::vector<Ogre::Vector3>& in)
{
	if (selected.size() == in.size())
		for (size_t i = 0; i < selected.size(); i++)
		{
			selected[i].pg->getSceneNode()->setPosition(in[i]);
		}
}

Forests::TBounds EditorGrass::scaleBounds(Forests::TBounds& b, Ogre::Vector3& scale)
{
	auto bounds = Ogre::AxisAlignedBox(b.left, 0, b.top, b.right, 0, b.bottom);
	auto center = bounds.getCenter();
	auto size = bounds.getHalfSize()*scale;

	return Forests::TBounds(center.x - size.x, center.z - size.z, center.x + size.x, center.z + size.z);
}

void EditorGrass::editMouseReleased(SelectionMode mode)
{
	if (boundsNode)
		boundsNode->setVisible(false);

	if(mode == SelectionMode::Move)
		for (auto& g : selected)
		{
			auto b = g.pg->getBounds();
			b.bottom += moveOffset.z;
			b.top += moveOffset.z;
			b.right += moveOffset.x;
			b.left += moveOffset.x;

			auto lvls = g.pg->getDetailLevels();
			auto tr = lvls.front()->getTransition();
			auto len = lvls.front()->getFarRange();

			g.pg->removeDetailLevels();
			g.pg->setBounds(b);
			g.pg->addDetailLevel<Forests::GrassPage>((float)len, (float)tr);

			if(g.bake.layer)
			{
				auto bounds = g.bake.layer->mapBounds;
				g.bake.layer->setMapBounds(Forests::TBounds(bounds.left + moveOffset.x, bounds.top + moveOffset.z, bounds.right + moveOffset.x, bounds.bottom + moveOffset.z));

				g.bake.pos += moveOffset;
				Global::gameMgr->geometryMgr->bakeLight(g.bake);
			}

			g.pg->reloadGeometry();

			g.terrainQuery->offset_maxY += moveOffset.y;
			g.terrainQuery->offset_minY += moveOffset.y;
		}

	moveOffset = Ogre::Vector3::ZERO;

	if (mode == SelectionMode::Scale)
		for (auto& g : selected)
		{
			auto lvls = g.pg->getDetailLevels();
			auto tr = lvls.front()->getTransition();
			auto len = lvls.front()->getFarRange();

			g.pg->removeDetailLevels();

			auto b = g.pg->getBounds();
			auto newBounds = scaleBounds(b, scaleOffset);
			auto size = std::max({ newBounds.right - newBounds.left, newBounds.bottom - newBounds.top, 25.f });
			g.pg->setBounds(Forests::TBounds(newBounds.left, newBounds.top, newBounds.left + size, newBounds.top + size));
			g.pg->addDetailLevel<Forests::GrassPage>((float)len, (float)tr);

			if (g.bake.layer)
			{
				auto bounds = scaleBounds(g.bake.layer->mapBounds, scaleOffset);
				g.bake.layer->setMapBounds(bounds);
				g.bake.size = Ogre::Vector2(bounds.right - bounds.left, bounds.bottom - bounds.top);

				Global::gameMgr->geometryMgr->bakeLight(g.bake);
			}

			g.pg->reloadGeometry();

			//g.terrainQuery->offset_maxY += moveOffset.y;
			//g.terrainQuery->offset_minY += moveOffset.y;
		}

	scaleOffset = Ogre::Vector3(1, 1, 1);
}

void EditorGrass::move(Ogre::Vector3& move)
{
	boundsNode->setVisible(true);

	moveOffset += move;
	updateNode();
}

void EditorGrass::rotate(Ogre::Vector3&, Ogre::Radian&)
{
}

void EditorGrass::addScale(Ogre::Vector3& scale)
{
	boundsNode->setVisible(true);

	scaleOffset += scale*0.1f;
	updateNode();
}

void EditorGrass::duplicate()
{
	/*std::vector<Ogre::Entity*> oldSelected = selected;

	deselect();

	for (auto& e : oldSelected)
	{
		auto ent = e->clone(getCloneName(e->getName()));
		auto newRoot = Global::sceneMgr->getRootSceneNode()->createChildSceneNode(e->getParentSceneNode()->getPosition(), e->getParentSceneNode()->getOrientation());
		newRoot->setScale(e->getParentSceneNode()->getScale());
		newRoot->attachObject(ent);

		add(ent);
	}*/
}

void EditorGrass::remove()
{
	/*for (auto& e : selected)
	{
		auto n = e->getParentSceneNode();
		e->detachFromParent();
		Global::sceneMgr->destroyEntity(e);
		Global::sceneMgr->destroySceneNode(n);
	}

	selected.clear();*/
}

void EditorGrass::add(GrassInfo& grass)
{
	selected.push_back(grass);

	if (!boundsNode)
	{
		auto e = GUtils::MakeEntity("basicBox.mesh", getPosition());
		e->setMaterialName("lowYellow");
		e->setRenderQueueGroup(RENDER_QUEUE_MAIN + 1);
		boundsNode = e->getParentSceneNode();
	}


	boundsNode->setVisible(false);
	updateNode();

	boundsNode->showBoundingBox(true);
}

void EditorGrass::select(GrassInfo& grass)
{
	deselect();
	add(grass);
}

void EditorGrass::deselect()
{
	boundsNode->setVisible(false);
	boundsNode->showBoundingBox(false);
	selected.clear();
}

bool EditorGrass::filter(std::string& name)
{
	auto oldSelected = selected;

	deselect();

	for (auto& e : oldSelected)
	{
		if (e.name == name)
			add(e);
	}

	return !selected.empty();
}

void EditorGrass::sendUiInfoMessage(EditorUiHandler* handler)
{
	UiMessage msg;
	msg.id = UiMessageId::ShowSelectionInfo;
	SelectionInfo info;

	if (selected.size() == 1)
	{
		info.name = std::wstring(selected[0].name.begin(), selected[0].name.end());
		info.pos = selected[0].pg->getSceneNode()->getPosition();
	}
	else
	{
		info.name = std::to_wstring(selected.size()) + L" objects";
		for (auto& e : selected)
			info.names.push_back(std::wstring(e.name.begin(), e.name.end()));

		info.pos = getPosition();
	}

	info.scale = Ogre::Vector3(1, 1, 1);
	msg.data = &info;

	handler->sendMsg(msg);
}

void EditorGrass::updateNode()
{
	auto b = getVisualBounds();
	boundsNode->setScale(b.getSize()*scaleOffset);
	boundsNode->setPosition(b.getCenter() + moveOffset);
}

Ogre::AxisAlignedBox EditorGrass::getVisualBounds()
{
	Ogre::AxisAlignedBox box;

	for (auto& g : selected)
	{
		auto b = g.bake.layer ? g.bake.layer->mapBounds : g.pg->getBounds();

		Ogre::AxisAlignedBox subbox(b.left, g.terrainQuery->offset_minY, b.top, b.right, g.terrainQuery->offset_maxY, b.bottom);

		box = subbox;// .merge(subbox);
	}

	return box;
}

void EditorGrass::setGrassPosition(GrassInfo& pg, Ogre::Vector3& pos)
{
	pg.pg->getSceneNode()->setPosition(pos);
}
