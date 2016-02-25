#include "stdafx.h"
#include "TestTask.h"
#include "Player.h"

TestReaction::TestReaction()
{
}

TestReaction::~TestReaction()
{
}

void TestReaction::setUserData(void* data)
{
	auto ent = (Ogre::Entity*) data;

	volume.fromBox(ent);
}

bool TestReaction::start()
{
	return true;
}

bool TestReaction::update(Ogre::Real tslf)
{
	auto pos = Global::mSceneMgr->getCamera("Camera")->getDerivedPosition();
	pos.y -= 6;

	auto close = volume.isInside(pos);

	if (close)
	{
		//Global::player->body->setLinearDamping(0.5f);
		//Global::player->gravity = Ogre::Vector3(10.0f, 5.0f, 0);
	}
	else
	{
		//Global::player->body->setLinearDamping(0);
		//Global::player->gravity = Ogre::Vector3(0, -9.0f, 0);
	}


	return true;
}
