#pragma once

#include "stdafx.h"

using namespace Ogre;

namespace MUtils
{

//normalized input
inline float smoothjump(float x, float exponent)
{
	auto y = 1 - std::min(x, 1 - x) * 2; //1-0-1
	float h = 1 - pow(y, exponent);

	return h;
}

//normalized input
inline float smoothjump(float x, float center, float exponent)
{
	auto modX = (x < center) ? (x / center) : ((x - center) / (1 - center));

	return smoothjump(modX, exponent);
}


inline float quickstep(float x, float smoothing)
{
	float w0 = smoothing;
	float w1 = 1.0f - smoothing;

	float w = (4 - 4 * x)*x;

	if (x > 0.5)
		w = 2 - w;

	return (w0*x + w1*w*0.5f);
}

inline float smoothstep(float x)
{
	return x*x*(3 - 2 * x);
}

inline float smoothstep(float edge0, float edge1, float x)
{
	float diff = edge1 - edge0;

	// Scale, bias and saturate x to 0..1 range
	x = Ogre::Math::Clamp((x - edge0) / (diff), 0.0f, 1.0f);

	return edge0 + diff*smoothstep(x);
}

inline float getYawBetween(Quaternion& q1, Quaternion& q2)
{
	auto yaw = q1.getYaw().valueDegrees();
	auto yaw2 = q2.getYaw().valueDegrees();

	auto r = yaw - yaw2;
	if (r > 180)
		r -= 360;
	if (r < -180)
		r += 360;

	return r;
}

inline Quaternion quaternionFromDir(Vector3 dir, bool normalize = true)
{
	if(normalize)
		dir.normalise();

	return Vector3::NEGATIVE_UNIT_Z.getRotationTo(dir);
}

inline Vector3 projectOnPlane(Vector3 v, Vector3 planeNormal)
{
	auto d = v.dotProduct(planeNormal);

	return v - planeNormal*d;
}

inline Vector3 dirFromQuaternion(Quaternion or)
{
	return or*Vector3::NEGATIVE_UNIT_Z;
}

inline Vector3 dirUpFromQuaternion(Quaternion or )
{
	return or *Vector3::UNIT_Y;
}

inline Vector3 dirRightFromQuaternion(Quaternion or )
{
	return or *Vector3::UNIT_X;
}

inline Quaternion quaternionFromNormal(Vector3 dirFront)
{
	dirFront.normalise();

	return Vector3::UNIT_Y.getRotationTo(dirFront);
}

inline Quaternion crowQuaternionFromDir(Vector3 dirFront)
{
	dirFront.normalise();

	Vector3 dirRight = Vector3(-dirFront.z, 0, dirFront.x);
	dirRight.normalise();

	Vector3 dirUp = dirRight.crossProduct(dirFront);

	return Quaternion(dirFront, dirUp, dirRight);
}

inline Ogre::Vector3 getMinimum(Vector3 v1, Vector3 v2)
{
	Vector3 out = v1;

	if (v2.x < out.x) out.x = v2.x;
	if (v2.y < out.y) out.y = v2.y;
	if (v2.z < out.z) out.z = v2.z;

	return out;
}

inline Ogre::Vector3 getMaximum(Vector3 v1, Vector3 v2)
{
	Vector3 out = v1;

	if (v2.x > out.x) out.x = v2.x;
	if (v2.y > out.y) out.y = v2.y;
	if (v2.z > out.z) out.z = v2.z;

	return out;
}

inline Quaternion crowQuaternionFromDirNoPitch(Vector3 dirFront)
{
	dirFront.y = 0;
	dirFront.normalise();

	Vector3 dirRight = Vector3(-dirFront.z, 0, dirFront.x);

	Vector3 dirUp(0,1,0);

	return Quaternion(dirFront, dirUp, dirRight);
}

inline float getSign(float x)
{
	return (float)((x > 0) - (x < 0));
}

inline float getPitch(Quaternion& q)
{
	auto pDir = q*Ogre::Vector3(0, 0, -1);
	pDir.normalise();

	float x = sqrt(1 - pDir.y*pDir.y);

	Vector3 dir(0, pDir.y, x);
	auto r = dir.angleBetween(Vector3(0, 0, 1))*getSign(pDir.y);

	return r.valueDegrees();
}

inline float getPitchBetween(Quaternion& q1, Quaternion& q2)
{
	auto pi = getPitch(q1);
	auto pi2 = getPitch(q2);

	auto r = pi - pi2;
	if (r > 180)
		r -= 360;
	if (r < -180)
		r += 360;

	return r;
}


inline float getSideDotProduct(Vector3 v1, Vector3 v2)
{
	v1.y = 0;
	v1.normalise();

	Vector3 v2rot = Vector3(-v2.z, 0, v2.x);
	v2rot.normalise();

	return v2rot.dotProduct(v1);
}

struct LineProjState
{
	Vector3 projPos;
	float sqMinDistance;
};

inline LineProjState getProjectedPointOnLine(Ogre::Vector3& point, Ogre::Vector3& start, Ogre::Vector3& end)
{
	Vector3 dir = end - start;
	Vector3 pDir = point - start;
	dir.normalise();

	auto dp = pDir.dotProduct(dir);
	dir *= dp;

	bool behindS = dp < 0;
	Vector3 eDir = point - end;
	bool behindE = eDir.dotProduct(-dir) < 0;

	//auto lToP = dir.length();
	//auto lleft = (end - dir).length();

	LineProjState state;
	state.projPos = dir + start;

	if (behindE || behindS)
		state.sqMinDistance = std::min(point.squaredDistance(end), point.squaredDistance(start));
	else
		state.sqMinDistance = point.squaredDistance(state.projPos);

	return state;
}

inline Vector3 lerp(Vector3 from, Vector3 to, float w)
{
	return to*w + from*(1 - w);
}

inline float lerp(float from, float to, float w)
{
	return to*w + from*(1 - w);
}

#define SMALL_NUM   0.00000001 // anything that avoids division overflow

struct SegmentsDistanceInfo
{
	float sqMinDistance;
	float s1Pos;
	float s2Pos;
};

inline SegmentsDistanceInfo getLinesDistanceInfo(Vector3 S1P0, Vector3 S1P1, Vector3 S2P0, Vector3 S2P1)
{
	Vector3   u = S1P1 - S1P0;
	Vector3   v = S2P1 - S2P0;
	Vector3   w = S1P0 - S2P0;
	float    a = u.dotProduct(u);         // always >= 0
	float    b = u.dotProduct(v);
	float    c = v.dotProduct(v);         // always >= 0
	float    d = u.dotProduct(w);
	float    e = v.dotProduct(w);
	float    D = a*c - b*b;        // always >= 0
	float    sc, sN, sD = D;       // sc = sN / sD, default sD = D >= 0
	float    tc, tN, tD = D;       // tc = tN / tD, default tD = D >= 0

	// compute the line parameters of the two closest points
	if (D < SMALL_NUM)   // the lines are almost parallel
	{
		sN = 0.0;         // force using point P0 on segment S1
		sD = 1.0;         // to prevent possible division by 0.0 later
		tN = e;
		tD = c;
	}
	else                   // get the closest points on the infinite lines
	{
		sN = (b*e - c*d);
		tN = (a*e - b*d);
		if (sN < 0.0)          // sc < 0 => the s=0 edge is visible
		{
			sN = 0.0;
			tN = e;
			tD = c;
		}
		else if (sN > sD)    // sc > 1  => the s=1 edge is visible
		{
			sN = sD;
			tN = e + b;
			tD = c;
		}
	}

	if (tN < 0.0)              // tc < 0 => the t=0 edge is visible
	{
		tN = 0.0;
		// recompute sc for this edge
		if (-d < 0.0)
			sN = 0.0;
		else if (-d > a)
			sN = sD;
		else
		{
			sN = -d;
			sD = a;
		}
	}
	else if (tN > tD)        // tc > 1  => the t=1 edge is visible
	{
		tN = tD;
		// recompute sc for this edge
		if ((-d + b) < 0.0)
			sN = 0;
		else if ((-d + b) > a)
			sN = sD;
		else
		{
			sN = (-d + b);
			sD = a;
		}
	}
	// finally do the division to get sc and tc
	sc = (abs(sN) < SMALL_NUM ? 0.0f : sN / sD);
	tc = (abs(tN) < SMALL_NUM ? 0.0f : tN / tD);

	// get the difference of the two closest points
	Vector3   dP = w + (sc * u) - (tc * v);  // =  S1(sc) - S2(tc)

	SegmentsDistanceInfo info;
	info.s1Pos = sc;
	info.s2Pos = tc;
	info.sqMinDistance = dP.squaredLength();

	return info;   // return the closest distance
}

};