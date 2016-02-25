#pragma once

#include "stdafx.h"

class TrackBuilder
{
public:

	void init(float length);
	void addKey(float t, Ogre::Vector3 pos, Ogre::Quaternion or);

	Ogre::NodeAnimationTrack* track;

private:

	inline void fixSpline(Ogre::Quaternion& rotation);
	Ogre::Quaternion lastOr;
	static int counter;
};