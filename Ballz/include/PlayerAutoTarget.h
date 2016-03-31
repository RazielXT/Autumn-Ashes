#pragma once

#include "stdafx.h"
#include <future>
#include <vector>

struct JumpBox;
class Slide;
class PlayerAutoTarget;

class TargetableObjects
{
	friend PlayerAutoTarget;

public:

	void addLoadedSlide(Slide* slide);
	void addLoadedJumpBox(JumpBox box);

private:

	std::vector<JumpBox> jumpBoxes;
	std::vector<Slide*> slides;
};

struct TargetSlideInfo
{
	Slide* targetSlide = nullptr;
	float targetSlidePosOffset;
	Ogre::Vector3 targetSlidePos;
};

class PlayerAutoTarget
{
	friend class PlayerSliding;

public:

	PlayerAutoTarget();
	~PlayerAutoTarget();

	bool spacePressed();

	void setSlidingInfo(bool autoAttach, Slide* currentSlide);

	void updateAutoTarget(Ogre::Vector3 pos, Ogre::Vector3 dir, float tslf, float rayDistance);

	TargetSlideInfo targetInfo;

	TargetableObjects objects;

private:

	struct
	{
		Slide* currentSlide;
		bool autoAttach;
	}
	slidingState;

	Slide* lastUnavailableSlide = nullptr;

	Slide* preparedSlide;
	float preparedSlideOffset;
	Ogre::Vector3 preparedSlidePos;

	float targetTimer;

	std::future<bool> targetResult;
	bool getTargetFunc(Ogre::Vector3 pos, Ogre::Vector3 dir, float rayDistance);

	bool getTargetSlideRay(Ogre::Vector3 pos, Ogre::Vector3 dir, float rayDistance, Slide* ignoredSlide);
	bool getTargetSlideTouch(Ogre::Vector3 pos, Ogre::Vector3 dir, Slide* ignoredSlide);
};