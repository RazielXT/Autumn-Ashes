#include "stdafx.h"
#include "LoadNextPart.h"
#include "Player.h"
#include "PostProcessMgr.h"
#include "GameStateManager.h"

using namespace Ogre;

LoadNextPart::LoadNextPart(Ogre::String target)
{
	this->target = target;
}

bool LoadNextPart::start()
{
	Global::ppMgr->fadeIn(Ogre::Vector3(0,0,0), 1);
	Global::player->stopMoving();
	Global::player->enableControl(false);
	timer = 0;

	return true;
}

bool LoadNextPart::update(Ogre::Real tslf)
{
	timer += tslf;

	if (loadNext)
	{
		auto pos = Global::player->body->getPosition();
		auto or = Global::sceneMgr->getCamera("Camera")->getOrientation();

		Global::gameMgr->switchState(SS_MAINMENU);

		if (Global::player)
		{
			Global::player->body->setPositionOrientation(pos, Global::player->body->getOrientation());
			Global::sceneMgr->getCamera("Camera")->setOrientation(or);
		}

		return false;
	}
	else if (timer >= 1)
		loadNext = true;

	return true;
}