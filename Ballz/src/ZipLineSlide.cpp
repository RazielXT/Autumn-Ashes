#include "stdafx.h"
#include "ZipLineSlide.h"
#include "MathUtils.h"

ZipLineSlide::ZipLineSlide(const std::vector<Ogre::Vector3>& points, const std::string& zipName, bool looped, float speed) : Slide(zipName, looped, speed)
{
	tracker = Global::mSceneMgr->getRootSceneNode()->createChildSceneNode();
	base = tracker->createChildSceneNode();
	head = base->createChildSceneNode();
	head->setPosition(0, -1.5f, 0);
	avgSpeed *= 5;

	initSlide(points);
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
	const float stabilityForce = 0.5f;
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
	auto yaw = MathUtils::getYawBetween(q, turnRollState.lastOr);
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
	Global::debug = headRoll;
}

void ZipLineSlide::updateSlidingCamera(float time)
{
	if (headArrival.timer > 0)
		updateHeadArrival(time);
	else
		updateTurningRoll(time);
}