#include "stdafx.h"
#include "ZipLineSlide.h"
#include "MUtils.h"

ZipLineSlide::ZipLineSlide(const std::vector<Ogre::Vector3>& points, const std::string& zipName, bool looped, bool is_walkable, float speed) : Slide(zipName, looped, is_walkable, speed)
{
	tracker = Global::sceneMgr->getRootSceneNode()->createChildSceneNode();
	base = tracker->createChildSceneNode();
	head = base->createChildSceneNode();

	resetHead();

	initTrack(points);
}

bool ZipLineSlide::start()
{
	auto ret = Slide::start();
	turnRollState.first = ret;

	return ret;
}

void ZipLineSlide::updateTurningRoll(float time)
{
	const float turnForce = 50;
	const float stabilityForce = 0.35f;
	const float rollLimit = 1.0f;

	auto q = tracker->getOrientation();

	if (turnRollState.first)
	{
		turnRollState.first = false;
		turnRollState.lastOr = q;
		turnRollState.curHeadRoll = 0;
		turnRollState.torque = 0;
	}

	//force to side, faster speed means more turn
	auto yaw = MUtils::getYawBetween(q, turnRollState.lastOr)*Math::PI/180.0f;
	auto force = yaw*time*currentSpeed*turnForce;

	auto& headRoll = turnRollState.curHeadRoll;

	float torqDamp = 1;
	if (headRoll*turnRollState.torque < 0)
		torqDamp = 0.3f;
	else if (abs(yaw)>0.025f)
		torqDamp = 0.01f;

	turnRollState.torque -= time*headRoll*stabilityForce*torqDamp;

	headRoll += force + turnRollState.torque;

	/*
	//force to center
	float centerForce = time*stabilityForce;
	if (headRoll > 0)
	headRoll = std::max(0.0f, headRoll - centerForce);
	else
	headRoll = std::min(0.0f, headRoll + centerForce);
	*/

	headRoll = Math::Clamp(headRoll, -rollLimit, rollLimit);
	base->setOrientation(Quaternion(Radian(headRoll), Vector3(0, 0, 1)));

	turnRollState.lastOr = q;
	//Global::debug = headRoll;
}

void ZipLineSlide::updateSlidingCamera(float time)
{
	updateTurningRoll(time);

	Slide::updateSlidingCamera(time);
}
