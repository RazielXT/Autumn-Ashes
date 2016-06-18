#include "stdafx.h"
#include "TopSlide.h"
#include "Player.h"
#include "OgreMath.h"
#include "MUtils.h"
#include "GUtils.h"

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

	manualRoll = 0;

	return Slide::start(pos);
}

void TopSlide::updateSlidingState(float time)
{
	/*Ogre::Vector3 oldUp = Ogre::Vector3(0,1,0);//MUtils::dirUpFromQuaternion(getDirectionState());

	Slide::updateSlidingState(time);

	Ogre::Vector3 newUp = Ogre::Vector3(0, 1, 1);
	newUp.normalise(); //MUtils::dirUpFromQuaternion(getDirectionState());
	Ogre::Vector3 facing = Ogre::Vector3(0, 1, -1);
	facing.normalise(); //MUtils::dirFromQuaternion(getDirectionState());*/

	Ogre::Vector3 oldUp = MUtils::dirUpFromQuaternion(getDirectionState());
	//GUtils::DebugPrint(GUtils::v2s(oldUp), true);

	Slide::updateSlidingState(time);

	auto q = getDirectionState();

	Ogre::Vector3 actualUp = MUtils::dirUpFromQuaternion(base->_getDerivedOrientation());
	Ogre::Vector3 newUp = MUtils::dirUpFromQuaternion(q);
	Ogre::Vector3 facing = MUtils::dirFromQuaternion(q);

	oldUp = MUtils::projectOnPlane(oldUp, facing);
	actualUp = MUtils::projectOnPlane(actualUp, facing);

	auto autoRoll = newUp.angleBetween(oldUp).valueDegrees();
	auto angle = newUp.angleBetween(actualUp).valueDegrees();

	Ogre::Vector3 right = MUtils::dirRightFromQuaternion(q);
	if (right.dotProduct(oldUp) < 0)
	{
		angle *= -1;
		autoRoll *= -1;
	}

	manualRoll = angle;

	base->roll(Ogre::Degree(-autoRoll));

	//GUtils::DebugPrint(GUtils::v2s(newUp), true);
	//GUtils::DebugPrint(GUtils::v2s(oldUp), true);
	//GUtils::DebugPrint(std::to_string(manualRoll), true);
}

void TopSlide::updateSlidingCamera(float time)
{
	auto dirRoll = tracker->getOrientation().getRoll().valueDegrees();

	//manualRoll += time*rolling * 5;
	//Quaternion baseQ(Degree(manualRoll - dirRoll), Vector3(0, 0, 1));
	//base->setOrientation(baseQ);

	Slide::updateSlidingCamera(time);

	base->roll(Ogre::Degree(time * -1 * avgSpeed * rolling));
}

void TopSlide::updateSlidingSpeed(float time)
{
	//currentSpeed = Math::Clamp(currentSpeed + 0.5f*time, 0.5f, 1.5f)*avgSpeed;

	currentSpeed = avgSpeed;

	//Slide::updateSlidingSpeed(time);
}