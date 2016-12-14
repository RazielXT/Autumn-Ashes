#include "stdafx.h"
#include "EditorGrass.h"
#include "EditorUiHandler.h"
#include "GUtils.h"
#include "GameStateManager.h"
#include "GrassHeightFunction.h"


EditorGrass::EditorGrass()
{
	moveOffset = Ogre::Vector3::ZERO;
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
	return getBounds().getCenter() + moveOffset;
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

void EditorGrass::editMouseReleased()
{
	if (boundsNode)
		boundsNode->setVisible(false);

	for (auto& g : selected)
	{
		auto b = g.pg->getBounds();
		b.bottom += moveOffset.z;
		b.top += moveOffset.z;
		b.right += moveOffset.x;
		b.left += moveOffset.x;

		g.terrainQuery->offset_maxY += moveOffset.y;
		g.terrainQuery->offset_minY += moveOffset.y;

		auto lvls = g.pg->getDetailLevels();
		auto tr = lvls.front()->getTransition();
		auto len = lvls.front()->getFarRange();

		g.pg->removeDetailLevels();
		g.pg->setBounds(b);

		g.pg->addDetailLevel<Forests::GrassPage>((float)len, (float)tr);
		g.pg->reloadGeometry();
	}

	moveOffset = Ogre::Vector3::ZERO;
}

void EditorGrass::move(Ogre::Vector3& move)
{
	boundsNode->setVisible(true);

	moveOffset += move;
	updateNode();

	/*for (auto& e : selected)
	{
		auto ePos = e.pg->getSceneNode()->getPosition();
		setGrassPosition(e, ePos + move);
	}*/
}

void EditorGrass::rotate(Ogre::Vector3& axis, Ogre::Radian& angle)
{
	for (auto& e : selected)
	{
		Ogre::Quaternion quat(angle, axis);
		//setEntityOrientation(e, quat);
	}
}

void EditorGrass::addScale(Ogre::Vector3& scale)
{
	for (auto& e : selected)
	{
		//auto newScale = e->getParentSceneNode()->getScale() + scale;
		//newScale.makeCeil(Ogre::Vector3(0.01f));

		//e->getParentSceneNode()->setScale(newScale);
	}
}
/*
std::string getCloneName(std::string oldName)
{
	int counter = 1;

	int i = oldName.length();
	auto endId = oldName.find_last_of("_c");
	bool needAppend = (endId == std::string::npos);

	if (!needAppend)
		for (size_t i = endId + 1; i < oldName.length(); i++)
		{
			if (!isdigit(oldName[i]))
				needAppend = true;
		}

	if (!needAppend)
	{
		char* end;
		counter = (endId + 1) == oldName.length() ? 1 : strtol(oldName.data() + endId + 1, &end, 10) + 1;
		oldName = oldName.substr(0, endId + 1);
	}
	else
		oldName += "_c";

	while (Global::sceneMgr->hasEntity(oldName + std::to_string(counter++)));

	return oldName + std::to_string(counter - 1);
}*/

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
		info.scale = Ogre::Vector3(1,1,1);// selected[0]->getParentSceneNode()->getScale();
	}
	else
	{
		info.name = std::to_wstring(selected.size()) + L" objects";
		for (auto& e : selected)
			info.names.push_back(std::wstring(e.name.begin(), e.name.end()));

		info.pos = getPosition();
		info.scale = Ogre::Vector3(1, 1, 1); //selected[0]->getParentSceneNode()->getScale();
	}

	msg.data = &info;

	handler->sendMsg(msg);
}

void EditorGrass::updateNode()
{
	auto b = getBounds();
	boundsNode->setScale(b.getSize());
	boundsNode->setPosition(b.getCenter() + moveOffset);
}

Ogre::AxisAlignedBox EditorGrass::getBounds()
{
	Ogre::AxisAlignedBox box;

	for (auto& g : selected)
	{
		auto b = g.pg->getBounds();

		Ogre::AxisAlignedBox subbox(b.left, g.terrainQuery->offset_minY, b.top, b.right, g.terrainQuery->offset_maxY, b.bottom);

		box = subbox;// .merge(subbox);
	}

	return box;
}

void EditorGrass::setGrassPosition(GrassInfo& pg, Ogre::Vector3& pos)
{
	pg.pg->getSceneNode()->setPosition(pos);
}
