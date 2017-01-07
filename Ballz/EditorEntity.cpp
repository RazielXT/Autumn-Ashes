#include "stdafx.h"
#include "EditorEntity.h"
#include "EditorUiHandler.h"
#include "GUtils.h"
#include "GameStateManager.h"
#include "Edit.h"
#include "EditorControl.h"

void EditorEntity::reset()
{
	selected.clear();
	animLoop.reset();
	delete matEdit;
	matEdit = nullptr;
}

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
	Ogre::Vector3 pos(0,0,0);

	for (auto& e : selected)
	{
		pos += e->getParentSceneNode()->_getDerivedPosition();
	}

	pos /= (float)selected.size();

	return pos;
}

std::vector<Ogre::Vector3> EditorEntity::getIndividualPositions()
{
	std::vector<Ogre::Vector3> out;

	for (auto& e : selected)
	{
		out.push_back(e->getParentSceneNode()->getPosition());
	}

	return out;
}

void EditorEntity::setIndividualPositions(std::vector<Ogre::Vector3>& in)
{
	if(selected.size() == in.size())
		for (size_t i = 0; i < selected.size(); i++)
		{
			setEntityPosition(selected[i], in[i]);
		}
}

void EditorEntity::move(Ogre::Vector3& move)
{
	for (auto& e : selected)
	{
		auto ePos = e->getParentSceneNode()->getPosition();
		setEntityPosition(e, ePos + move);
	}
}

void EditorEntity::rotate(Ogre::Vector3& axis, Ogre::Radian& angle)
{
	for (auto& e : selected)
	{
		Ogre::Quaternion quat(angle, axis);
		setEntityOrientation(e, quat);
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
		for (size_t i = endId + 1; i < oldName.length(); i++)
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

	while (Global::sceneMgr->hasEntity(oldName + std::to_string(counter++)));

	return oldName + std::to_string(counter-1);
}

void EditorEntity::duplicate()
{
	std::vector<Ogre::Entity*> oldSelected = selected;

	deselect();

	for (auto& e : oldSelected)
	{
		auto ent = e->clone(getCloneName(e->getName()));
		auto newRoot = Global::sceneMgr->getRootSceneNode()->createChildSceneNode(e->getParentSceneNode()->getPosition(), e->getParentSceneNode()->getOrientation());
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
		Global::sceneMgr->destroyEntity(e);
		Global::sceneMgr->destroySceneNode(n);
	}

	selected.clear();
}

void EditorEntity::add(Ogre::Entity* ent)
{
	selected.push_back(ent);
	ent->getParentSceneNode()->showBoundingBox(true);

	if (selected.size() == 1 && MaterialEdit::editableEntity(ent))
	{
		matEdit = new MaterialEdit(ent);
	}
	else
	{
		delete matEdit;
		matEdit = nullptr;
	}
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

	animLoop.stop();
	reset();
}

bool EditorEntity::filter(std::string& name)
{
	std::vector<Ogre::Entity*> oldSelected = selected;

	deselect();

	for (auto& e : oldSelected)
	{
		if(e->getName() == name)
			add(e);
	}

	return !selected.empty();
}

void EditorEntity::sendUiInfoMessage(EditorUiHandler* handler)
{
	UiMessage msg;
	msg.id = UiMessageId::ShowSelectionInfo;
	SelectionInfo info;
	EntitySelectionInfo eInfo;

	if (selected.size() == 1)
	{
		info.name = selected[0]->getName();
		info.pos = selected[0]->getParentSceneNode()->getPosition();
		info.scale = selected[0]->getParentSceneNode()->getScale();

		auto animSet = selected[0]->getAllAnimationStates();
		if (animSet)
		{
			auto iter = animSet->getAnimationStateIterator();
			while (iter.hasMoreElements())
			{
				eInfo.animNames.push_back(iter.peekNextKey());
				iter.moveNext();
			}
		}

		info.hasParams = matEdit != nullptr;

		info.typeData = &eInfo;
	}
	else
	{
		info.name = std::to_string(selected.size()) + " objects";
		for (auto& e : selected)
			info.names.push_back(e->getName());

		info.pos = getPosition();
		info.scale = selected[0]->getParentSceneNode()->getScale();
	}

	info.usePaint = false;
	info.type = ItemType::Entity;
	msg.data = &info;

	handler->sendMsg(msg);
}

void EditorEntity::handleSelectionMessage(SelectionInfoChange* change)
{
	if (change->change == SelectionInfoChange::Id::ActiveAnimation)
	{
		std::string& name = *(std::string*)change->data;

		if (name.empty())
		{
			animLoop.stop();
		}
		else
		{
			animLoop.start(selected[0], name);
		}
	}
	else if (change->change == SelectionInfoChange::Id::GetParams && matEdit)
	{
		auto& response = *(EditableParamsResponse*)change->data;

		auto rows = matEdit->getBaseRows();
		for (auto r : rows)
		{
			if (r.type == EditRow::Params)
			{
				EditableParams params;
				params.groupName = matEdit->originName + " " + r.name;

				auto matparams = matEdit->getParams(r.name);

				for (auto& mparam : *matparams)
				{
					params.params.push_back({ mparam.name, r.name, EditableParam::Type::Floats, { mparam.size, mparam.buffer[0], mparam.buffer[1], mparam.buffer[2], mparam.buffer[3], mparam.step } });
				}

				response.params.push_back(params);
			}
		}
	}
	else if (change->change == SelectionInfoChange::Id::ParamChanged && matEdit)
	{
		EditableParam& param = *(EditableParam*)change->data;

		auto params = matEdit->getParams(param.group);

		for (auto& p : *params)
		{
			if (p.name == param.name && param.type == EditableParam::Type::Floats)
			{
				p.buffer[param.floats.size] = param.floats.buffer[param.floats.size];
				matEdit->editChanged(p, param.group);

				break;
			}
		}
	}
	else if (change->change == SelectionInfoChange::Id::SaveParams && matEdit)
	{
		matEdit->customAction("Save");
	}
}

void EditorEntity::setEntityPosition(Ogre::Entity* ent, Ogre::Vector3& pos)
{
	auto b = Global::gameMgr->loadedBodies->find(ent->getName());

	if (b == Global::gameMgr->loadedBodies->end())
	{
		ent->getParentSceneNode()->setPosition(pos);
	}
	else
	{
		b->second->setPositionOrientation(pos, b->second->getOrientation());
	}
}

void EditorEntity::setEntityOrientation(Ogre::Entity* ent, Ogre::Quaternion& q)
{
	auto b = Global::gameMgr->loadedBodies->find(ent->getName());
	ent->getParentSceneNode()->rotate(q, Ogre::Node::TransformSpace::TS_WORLD);

	if (b != Global::gameMgr->loadedBodies->end())
	{
		b->second->setPositionOrientation(b->second->getPosition(), ent->getParentSceneNode()->getOrientation());
	}
}

