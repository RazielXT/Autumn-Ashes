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

inline LineProjState getProjectedState(Ogre::Vector3& point, Ogre::Vector3& start, Ogre::Vector3& end)
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

inline SegmentsDistanceInfo getSegmentsDistanceInfo(Vector3 S1P0, Vector3 S1P1, Vector3 S2P0, Vector3 S2P1)
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