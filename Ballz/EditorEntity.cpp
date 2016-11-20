#include "stdafx.h"
#include "EditorEntity.h"
#include "EditorUiHandler.h"


void EditorEntity::setPosition(Ogre::Vector3& pos)
{
	auto translate = getPosition();
	translate = pos - translate;

	move(translate);
}

void EditorEntity::setScale(Ogre::Vector3& scale)
{
	for (auto& e : selected)
	{
		e->getParentSceneNode()->setScale(scale);
	}
}

Ogre::Vector3 EditorEntity::getPosition()
{
	Ogre::Vector3 pos;

	for (auto& e : selected)
	{
		pos += e->getParentSceneNode()->getPosition();
	}

	pos /= selected.size();

	return pos;
}

void EditorEntity::move(Ogre::Vector3& move)
{
	for (auto& e : selected)
	{
		e->getParentSceneNode()->translate(move, Ogre::Node::TransformSpace::TS_WORLD);
	}
}

void EditorEntity::rotate(Ogre::Vector3& axis, Ogre::Radian& angle)
{
	for (auto& e : selected)
	{
		e->getParentSceneNode()->rotate(axis, angle, Ogre::Node::TransformSpace::TS_WORLD);
	}
}

void EditorEntity::addScale(Ogre::Vector3& scale)
{
	for (auto& e : selected)
	{
		auto newScale = e->getParentSceneNode()->getScale() + scale;
		newScale.makeCeil(Ogre::Vector3(0.01f));

		e->getParentSceneNode()->setScale(newScale);
	}
}

std::string getCloneName(std::string oldName)
{
	int counter = 1;

	int i = oldName.length();
	auto endId = oldName.find_last_of("_c");
	bool needAppend = (endId == std::string::npos);

	if(!needAppend)
		for (int i = endId + 1; i < oldName.length(); i++)
		{
			if (!isdigit(oldName[i]))
				needAppend = true;
		}

	if (!needAppend)
	{
		char* end;
		counter = (endId+1)==oldName.length() ? 1 : strtol(oldName.data() + endId + 1, &end, 10)+1;
		oldName = oldName.substr(0, endId + 1);
	}
	else
		oldName += "_c";

	while (Global::mSceneMgr->hasEntity(oldName + std::to_string(counter++)));

	return oldName + std::to_string(counter-1);
}

void EditorEntity::duplicate()
{
	std::vector<Ogre::Entity*> oldSelected = selected;

	deselect();

	for (auto& e : oldSelected)
	{
		auto ent = e->clone(getCloneName(e->getName()));
		auto newRoot = Global::mSceneMgr->getRootSceneNode()->createChildSceneNode(e->getParentSceneNode()->getPosition(), e->getParentSceneNode()->getOrientation());
		newRoot->setScale(e->getParentSceneNode()->getScale());
		newRoot->attachObject(ent);

		add(ent);
	}
}

void EditorEntity::remove()
{
	for (auto& e : selected)
	{
		auto n = e->getParentSceneNode();
		e->detachFromParent();
		Global::mSceneMgr->destroyEntity(e);
		Global::mSceneMgr->destroySceneNode(n);
	}

	selected.clear();
}

void EditorEntity::add(Ogre::Entity* ent)
{
	selected.push_back(ent);
	ent->getParentSceneNode()->showBoundingBox(true);
}

void EditorEntity::select(Ogre::Entity* ent)
{
	deselect();
	add(ent);
}

void EditorEntity::deselect()
{
	for (auto& e : selected)
	{
		e->getParentSceneNode()->showBoundingBox(false);
	}

	selected.clear();
}

void EditorEntity::sendUiInfoMessage(EditorUiHandler* handler)
{
	UiMessage msg;
	msg.id = UiMessageId::ShowEntityInfo;
	EntityInfo info;

	if (selected.size() == 1)
	{
		info.name = std::wstring(selected[0]->getName().begin(), selected[0]->getName().end());
		info.pos = selected[0]->getParentSceneNode()->getPosition();
		info.scale = selected[0]->getParentSceneNode()->getScale();
	}
	else
	{
		info.name = std::to_wstring(selected.size()) + L" objects";
		for (auto& e : selected)
			info.names.push_back(std::wstring(e->getName().begin(), e->getName().end()));

		info.pos = getPosition();
		info.scale = selected[0]->getParentSceneNode()->getScale();
	}

	msg.data = &info;

	handler->sendMsg(msg);
}

