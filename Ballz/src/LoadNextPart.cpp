#include "stdafx.h"
#include "LoadNextPart.h"
#include "player.h"
#include "PostProcessMgr.h"

using namespace Ogre;

LoadNextPart::LoadNextPart(Ogre::String target)
{
	this->target = target;
}

bool LoadNextPart::start()
{
	Global::mPPMgr->colourOut(Ogre::Vector3(0,0,0), 1);
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
		auto pos = Global::player->getBody()->getPosition();
		auto or = Global::mSceneMgr->getCamera("Camera")->getOrientation();

		Global::gameMgr->switchToLevel(0);

		if (Global::player)
		{
			Global::player->getBody()->setPositionOrientation(pos, Global::player->getBody()->getOrientation());
			Global::mSceneMgr->getCamera("Camera")->setOrientation(or);
		}

		return false;
	}
	else
	if (timer >= 1)
		loadNext = true;

	return true;
}