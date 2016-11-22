#pragma once

#include "Ogre.h"

struct CameraHelp
{
	Ogre::Camera* cam;

	Ogre::Vector3 position;
	Ogre::Vector3 direction;

	void update();
};