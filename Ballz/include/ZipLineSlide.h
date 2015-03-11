#pragma once
#include "stdafx.h"
#include "Slide.h"

using namespace Ogre;

class ZipLineSlide : public Slide
{
public:

	ZipLineSlide(const std::vector<Ogre::Vector3>& points, const std::string& zipName, bool looped, float speed);

	virtual bool start();

protected:

	struct TurnRollState
	{
		bool first = true;
		Quaternion lastOr;
		float torque;
		float curHeadRoll;
	};

	virtual void updateSlidingCamera(float time);

	inline void updateTurningRoll(float time);

	TurnRollState turnRollState;
};