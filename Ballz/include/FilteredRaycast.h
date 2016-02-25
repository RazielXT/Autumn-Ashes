#pragma once

#include "stdafx.h"

class FilteredRaycast : public OgreNewt::BasicRaycast
{
public:

	FilteredRaycast(const OgreNewt::World* world, const Ogre::Vector3& startpt, const Ogre::Vector3& endpt, bool sorted, OgreNewt::Body* bodyTarget);

	virtual bool userPreFilterCallback(OgreNewt::Body* body)
	{
		return target == body;
	}

	OgreNewt::Body* target;
};