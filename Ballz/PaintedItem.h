#pragma once
#include "Ogre.h"
#include "OgreNewt.h"

struct PaitedItem
{
	~PaitedItem() {};

	virtual void paint(Ogre::Vector3 pos, float w, float size) = 0;
	virtual OgreNewt::Body* getPaintTarget() = 0;
};