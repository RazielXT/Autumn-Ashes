#pragma once

#include "stdafx.h"

enum PathState{ OnGround, Flying, Landing, Lifting, None };

struct CrowPathAnimations
{
	Ogre::Vector3 lastPos;
	Ogre::Quaternion lastOr;

	Ogre::AnimationState * mFlightState = nullptr;
	Ogre::AnimationState * mTempState = nullptr;
	float animWeight = 0;

	Ogre::Vector3 getAnimPos();
	Ogre::Quaternion getAnimOr();

	void clear();

	~CrowPathAnimations() { clear(); }
};

class CrowPath
{
public:

	CrowPath();
	~CrowPath();

	bool update(Ogre::Real tslf, Ogre::SceneNode* node);

	Ogre::Vector3 getPosition();
	Ogre::Quaternion getOrientation();
	float getPathTimeLeft();

	PathState state = None;
	void setLandingAnim(Ogre::Vector3 targetPos);
	void setLiftingAnim(Ogre::Animation* targetFlightAnim, float timePos);

protected:

	CrowPathAnimations animState;


};