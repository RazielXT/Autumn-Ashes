#include "stdafx.h"
#include "EditorParticle.h"
#include "EditorUiHandler.h"
#include "GUtils.h"
#include "GameStateManager.h"


void EditorParticle::reset()
{
	selected.clear();
	boundsNode = nullptr;
}

void EditorParticle::setPosition(Ogre::Vector3& pos)
{
	auto translate = getPosition();
	translate = pos - translate;

	move(translate);
}

void EditorParticle::setScale(Ogre::Vector3& scale)
{
	for (auto& e : selected)
	{
		e->getParentSceneNode()->setScale(scale);
	}
}

Ogre::Vector3 EditorParticle::getPosition()
{
	Ogre::Vector3 pos(0,0,0);

	for (auto& e : selected)
	{
		pos += e->getParentSceneNode()->_getDerivedPosition();
	}

	pos /= (float)selected.size();

	return pos;
}

std::vector<Ogre::Vector3> EditorParticle::getIndividualPositions()
{
	std::vector<Ogre::Vector3> out;

	for (auto& e : selected)
	{
		out.push_back(e->getParentSceneNode()->getPosition());
	}

	return out;
}

void EditorParticle::setIndividualPositions(std::vector<Ogre::Vector3>& in)
{
	if(selected.size() == in.size())
		for (size_t i = 0; i < selected.size(); i++)
		{
			setParticlePosition(selected[i], in[i]);
		}
}

void EditorParticle::move(Ogre::Vector3& move)
{
	for (auto& e : selected)
	{
		auto ePos = e->getParentSceneNode()->getPosition();
		setParticlePosition(e, ePos + move);
	}

	updateNode();
}

void EditorParticle::rotate(Ogre::Vector3& axis, Ogre::Radian& angle)
{
	for (auto& e : selected)
	{
		Ogre::Quaternion quat(angle, axis);
		setParticleOrientation(e, quat);
	}

	updateNode();
}

void EditorParticle::addScale(Ogre::Vector3& scale)
{
	for (auto& e : selected)
	{
		auto newScale = e->getParentSceneNode()->getScale() + scale;
		newScale.makeCeil(Ogre::Vector3(0.01f));

		e->getParentSceneNode()->setScale(newScale);
	}
}

void EditorParticle::duplicate()
{
	/*auto oldSelected = selected;

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

void EditorParticle::remove()
{
	/*for (auto& e : selected)
	{
		auto n = e->getParentSceneNode();
		e->detachFromParent();
		Global::sceneMgr->destroyEntity(e);
		Global::sceneMgr->destroySceneNode(n);
	}*/

	selected.clear();
}

void EditorParticle::editMouseReleased(SelectionMode mode)
{
	if (mode == SelectionMode::Move || mode == SelectionMode::Rotate)
		selected[0]->fastForward(selected[0]->getEmitter(0)->getMaxTimeToLive());
}

void EditorParticle::add(Ogre::ParticleSystem* ps)
{
	selected.push_back(ps);

	if (!boundsNode)
	{
		auto e = GUtils::MakeEntity("basicBox.mesh", getPosition());
		e->setMaterialName("lowYellow");
		e->setRenderQueueGroup(RENDER_QUEUE_MAIN + 1);
		boundsNode = e->getParentSceneNode();
	}

	updateNode();

	boundsNode->setVisible(true);
	boundsNode->showBoundingBox(true);
}

void EditorParticle::select(Ogre::ParticleSystem* ps)
{
	deselect();
	add(ps);
}

void EditorParticle::deselect()
{
	boundsNode->setVisible(false);
	boundsNode->showBoundingBox(false);
	selected.clear();
}

bool EditorParticle::filter(std::string& name)
{
	auto oldSelected = selected;

	deselect();

	for (auto& e : oldSelected)
	{
		if(e->getName() == name)
			add(e);
	}

	return !selected.empty();
}

void EditorParticle::sendUiInfoMessage(EditorUiHandler* handler)
{
	UiMessage msg;
	msg.id = UiMessageId::ShowSelectionInfo;
	SelectionInfo info;
	//EntitySelectionInfo eInfo;

	if (selected.size() == 1)
	{
		info.name = selected[0]->getName();
		info.pos = selected[0]->getParentSceneNode()->getPosition();
		info.scale = selected[0]->getParentSceneNode()->getScale();

		//info.typeData = &eInfo;
	}
	else
	{
		info.name = std::to_string(selected.size()) + " objects";
		for (auto& e : selected)
			info.names.push_back(e->getName());

		info.pos = getPosition();
		info.scale = selected[0]->getParentSceneNode()->getScale();
	}

	info.type = ItemType::Particle;
	msg.data = &info;

	handler->sendMsg(msg);
}

void EditorParticle::handleSelectionMessage(SelectionInfoChange* change)
{
	if (change->change == SelectionInfoChange::Id::ActiveAnimation)
	{

	}
}

Ogre::Quaternion EditorParticle::getOrientation()
{
	return selected[0]->getParentSceneNode()->getOrientation();
}

void EditorParticle::setParticlePosition(Ogre::ParticleSystem* ps, Ogre::Vector3 pos)
{
	ps->getParentSceneNode()->setPosition(pos);
}

void EditorParticle::setParticleOrientation(Ogre::ParticleSystem* ps, Ogre::Quaternion or )
{
	ps->getParentSceneNode()->rotate(or , Ogre::Node::TransformSpace::TS_WORLD);
}

void EditorParticle::updateNode()
{
	auto e = selected[0]->getEmitter(0);
	auto type = e->getType();

	if (type == "Box")
	{
		auto x = std::stof(e->getParameter("width"));
		auto y = std::stof(e->getParameter("height"));
		auto z = std::stof(e->getParameter("depth"));

		boundsNode->setScale(Ogre::Vector3(x,y,z));
	}
	else
		boundsNode->setScale(Ogre::Vector3(5,5,5));

	boundsNode->setPosition(getPosition());

	boundsNode->setOrientation(getOrientation());
}
