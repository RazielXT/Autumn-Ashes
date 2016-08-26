#pragma once

#include "MovingAnimation.h"
#include "OgreQuaternion.h"

enum AnimEvent
{
	Jump,
	Float,
	Swim,
	Slide,
	Wallrun,
	Fly
};

class PlayerModel
{
public:

	PlayerModel();
	~PlayerModel();

	void notifyEvent(AnimEvent evt);

	void update(float time);

	void setTransformation(bool enabled);

	void setRotation(Ogre::Quaternion q, float time = 0);

private:

	MovingAnimation moving;
};