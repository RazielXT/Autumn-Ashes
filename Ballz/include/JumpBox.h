#pragma once

#include "stdafx.h"
#include "HeadTransition.h"

struct JumpBox
{
	Ogre::Vector3 position;
	Ogre::Vector3 faceDirection;

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