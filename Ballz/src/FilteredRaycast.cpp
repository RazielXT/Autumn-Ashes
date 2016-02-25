#include "stdafx.h"
#include "FilteredRaycast.h"

FilteredRaycast::FilteredRaycast(const OgreNewt::World* world, const Ogre::Vector3& startpt, const Ogre::Vector3& endpt, bool sorted, OgreNewt::Body* bodyTarget)
	: target(bodyTarget)
{
	Raycast::go(world, startpt, endpt);
}