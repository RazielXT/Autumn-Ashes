#include "stdafx.h"
#include "CameraHelp.h"

void CameraHelp::update()
{
	direction = cam->getDerivedOrientation()*Ogre::Vector3(0, 0, -1);
	position = cam->getDerivedPosition();
}

void CameraHelp::init(Ogre::Camera* mCamera)
{
	cam = mCamera;
	defaultFov = cam->getFOVy();
}

