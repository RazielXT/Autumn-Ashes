#pragma once

#include "stdafx.h"
#include <future>
#include <vector>

struct Pole;
struct JumpBox;
class Slide;
class PlayerAutoTarget;

class TargetableObjects
{
	friend PlayerAutoTarget;

public:

	void addLoadedSlide(Slide* slide);
	void addLoadedJumpBox(JumpBox box);
	void addLoadedPole(Pole pole);

private:

	std::vector<Pole> poles;
	std::vector<JumpBox> jumpBoxes;
	std::vector<Slide*> slides;
};

struct AutoTargetInfo
{
	Slide* targetSlide = nullptr;
	float targetSlideOffset;

	Slide* closeSlide = nullptr;
	Ogre::Vector3 closeSlidePos;

	Pole* targetPole = nullptr;
	Pole* closePole = nullptr;

	JumpBox* targetJump = nullptr;

	float closestTargetDistanceSq;
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

	AutoTargetInfo targetInfo;

	TargetableObjects objects;

private:

	bool getTargetPole(Ogre::Vector3 pos, Ogre::Vector3 dir, float rayDistance);
	bool getTargetJump(Ogre::Vector3 pos, Ogre::Vector3 dir, float rayDistance);

	struct
	{
		Slide* currentSlide;
		bool autoAttach;
	}
	slidingState;

	Slide* lastUnavailableSlide = nullptr;

	AutoTargetInfo preparedInfo;

	float targetTimer;

	std::future<bool> targetResult;
	bool getTargetFunc(Ogre::Vector3 pos, Ogre::Vector3 dir, float rayDistance);

	bool getTargetSlideRay(Ogre::Vector3 pos, Ogre::Vector3 dir, float rayDistance, Slide* ignoredSlide);
	bool getTargetSlideTouch(Ogre::Vector3 pos, Ogre::Vector3 dir, Slide* ignoredSlide);
};