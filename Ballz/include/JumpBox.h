#pragma once

#include "stdafx.h"
#include "HeadTransition.h"

struct JumpBox
{
	Ogre::Vector3 position;
	float radius;

	Ogre::Vector3 faceDirection;
	Ogre::Vector3 jumpDirection;

	void jumpToBox();
};

class JumpBoxArrival : public EventTask
{
public:

	JumpBoxArrival(JumpBox* target);

	virtual bool update(Ogre::Real tslf);

private:

	JumpBox* target;
	HeadTransition transition;
};