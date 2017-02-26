#pragma once

#include "Ogre.h"

struct CameraHelp
{
	Ogre::Radian defaultFov;
	Ogre::Camera* cam;

	Ogre::Vector3 position;
	Ogre::Vector3 direction;

	void update();
	void init(Ogre::Camera* mCamera);
};