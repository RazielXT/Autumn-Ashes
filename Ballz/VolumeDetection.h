#pragma once

#include "stdafx.h"

class OrientedVolume
{
public:

	Ogre::Vector3 position;

	bool isInside(Ogre::Vector3& point)
	{
		if (position.squaredDistance(point) > sqRadius)
			return false;

		return (volumeType==Sphere) ? true : isInsideBox(point);	
	}	

	void fromSphere(float r)
	{
		axis[0] = Ogre::Vector3(1, 0, 0);
		axis[1] = Ogre::Vector3(0, 1, 0);
		axis[2] = Ogre::Vector3(0, 0, 1);

		sqRadius = r*r;
		hSize = r;
		volumeType = Sphere;
	}

	void fromBox(Ogre::Vector3 halfSize, Ogre::Quaternion q)
	{
		q.ToAxes(axis);

		float radiusEstimate = halfSize.length();
		sqRadius = radiusEstimate*radiusEstimate;
		hSize = halfSize;
		volumeType = Box;
	}

protected:

	enum { Box, Sphere } volumeType;
	float sqRadius;
	Ogre::Vector3 hSize;
	Ogre::Vector3 axis[3];

	bool isInsideBox(Ogre::Vector3 point)
	{
		Ogre::Vector3 pt = point - position;

		auto xp = pt.absDotProduct(axis[0]);
		auto yp = pt.absDotProduct(axis[1]);
		auto zp = pt.absDotProduct(axis[2]);

		return xp <= hSize.x && yp <= hSize.y && zp <= hSize.z;
	}
};