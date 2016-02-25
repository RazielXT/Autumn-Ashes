#include "stdafx.h"
#include "TopSlide.h"
#include "Player.h"
#include "OgreMath.h"

TopSlide::TopSlide(SceneNode* node, const std::string& zipName, const std::string& zipAnimName, bool looped, bool walkable, float speed) : Slide(zipAnimName, looped, walkable, speed)
{
	tracker = node;
	base = tracker->createChildSceneNode();
	head = base->createChildSceneNode();

	resetHead();

	initTrack(zipAnimName);
}

TopSlide::TopSlide(const std::vector<Ogre::Vector3>& points, const std::string& zipName, bool looped, bool walkable, float speed) : Slide(zipName, looped, walkable, speed)
{
	tracker = Global::mSceneMgr->getRootSceneNode()->createChildSceneNode();
	base = tracker->createChildSceneNode();
	head = base->createChildSceneNode();

	resetHead();

	initTrack(points);
}

void TopSlide::pressedKey(const OIS::KeyEvent &arg)
{
	if (!sliding)
		return;

	if (arg.key == OIS::KC_A)
	{
		rolling = -1;
	}
	else if (arg.key == OIS::KC_D)
	{
		rolling = 1;
	}
	else
		Slide::pressedKey(arg);
}

void TopSlide::releasedKey(const OIS::KeyEvent &arg)
{
	if (arg.key == OIS::KC_A && rolling == -1)
	{
		rolling = 0;
	}
	else if (arg.key == OIS::KC_D  && rolling == 1)
	{
		rolling = 0;
	}

	Slide::releasedKey(arg);
}

bool TopSlide::start()
{
	auto pos = Global::player->bodyPosition;
	pos.y -= 1.5f;

	return Slide::start(pos);
}

void TopSlide::updateSlidingCamera(float time)
{
	auto dirRoll = tracker->getOrientation().getRoll().valueDegrees();

	manualRoll += time*rolling * 5;
	Quaternion baseQ(Degree(manualRoll - dirRoll), Vector3(0, 0, 1));
	//base->setOrientation(baseQ);

	Slide::updateSlidingCamera(time);
}

void TopSlide::updateSlidingSpeed(float time)
{
	//currentSpeed = Math::Clamp(currentSpeed + 0.5f*time, 0.5f, 1.5f)*avgSpeed;

	Slide::updateSlidingSpeed(time);
}