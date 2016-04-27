#include "stdafx.h"
#include "JumpBox.h"
#include "Player.h"

void JumpBox::jumpToBox()
{
	auto p = Global::player;

	p->enableControl(false);

	Global::mEventsMgr->addTask(new JumpBoxArrival(this));
}

JumpBoxArrival::JumpBoxArrival(JumpBox* t) : target(t)
{
	transition.initializeJump(target->position + Ogre::Vector3(0, 1, 0));
}

bool JumpBoxArrival::update(Ogre::Real tslf)
{
	auto finished = transition.updateJump(tslf);

	if (finished)
	{
		//Ogre::Quaternion direction = transition.transitionNode->_getDerivedOrientation();
		//Ogre::Quaternion pos = transition.transitionNode->_getDerivedOrientation();

		Global::player->enableControl(true);
		Global::player->body->setPositionOrientation(target->position, Ogre::Quaternion::IDENTITY);

		auto jumpDirection = (target->faceDirection + Ogre::Vector3(0, 1.65f, 0)) * 15;

		Global::player->body->setVelocity(jumpDirection);

		Global::camera->attachCameraWithTransition(0.25f);
		Global::camera->nodHead(50);
	}

	return !finished;
}

