#pragma once

#include "stdafx.h"

using namespace Ogre;

namespace MathUtils
{
	inline float getYawBetween(Quaternion& q1, Quaternion& q2)
	{
		auto yaw = q1.getYaw().valueRadians();
		auto yaw2 = q2.getYaw().valueRadians();

		auto r = yaw - yaw2;
		if (r > Math::PI)
			r -= Math::TWO_PI;
		if (r < -Math::PI)
			r += Math::TWO_PI;

		return r;
	}

	struct LineProjState
	{
		Vector3 projPos;
		float sqMinDistance;
	};

	inline LineProjState getProjectedState(Ogre::Vector3& point, Ogre::Vector3& start, Ogre::Vector3& end)
	{
		Vector3 dir = end - start;
		Vector3 pDir = point - start;
		dir.normalise();
		auto dp = pDir.dotProduct(dir);
		dir *= dp;

		//auto lToP = dir.length();
		//auto lleft = (end - dir).length();

		LineProjState state;
		state.projPos = dir + start;
		state.sqMinDistance = std::min(point.squaredDistance(end), std::min(point.squaredDistance(start), point.squaredDistance(state.projPos)));

		return state;
	}
};