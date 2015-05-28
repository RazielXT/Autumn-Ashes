#pragma once

#include "stdafx.h"

enum CrowState{ OnGround, Flying, Landing, Lifting, None };

class CrowPath
{
public:

	CrowPath();
	~CrowPath();

	bool update(Ogre::Real tslf);

	Ogre::Vector3 getPosition();
	Ogre::Vector3 getOrientation();
	float getPathTimeLeft();

	CrowState state = None;
	void setLandingAnim(Ogre::Vector3 targetPos);
	void setLiftingAnim(Ogre::Animation* targetFlightAnim, float timePos);

protected:

	//basic, blended anim, weights
};