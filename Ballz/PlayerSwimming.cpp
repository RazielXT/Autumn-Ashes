#include "stdafx.h"
#include "PlayerSwimming.h"
#include "Player.h"
#include "WaterCurrent.h"

PlayerSwimming::PlayerSwimming(Player* p)
{
	player = p;
	currents = WaterCurrent::get();

	/*
	bubbles = Global::mSceneMgr->createParticleSystem("WaterBubbles", "WaterBubbles");
	bubbles->setRenderQueueGroup(91);
	bubbles->setVisibilityFlags(8);

	bubblesNode = Global::mSceneMgr->getRootSceneNode()->createChildSceneNode();
	bubblesNode->attachObject(bubbles);
	*/
}

PlayerSwimming::~PlayerSwimming()
{
	currents->reset();
}

void PlayerSwimming::enteredWater(Ogre::Vector3 position)
{
	player->body->setLinearDamping(0.5f);
	player->gravity = Ogre::Vector3(0, 0.3f, 0) + currents->getCurrent(player->bodyPosition);
}

void PlayerSwimming::leftWater()
{
	player->body->setLinearDamping(0);
	player->gravity = PlayerSettings::defaultGravity;
}

