#include "stdafx.h"
#include "WaterCurrent.h"
#include "Player.h"

using namespace Ogre;

WaterCurrent* WaterCurrent::instance = nullptr;

WaterCurrent::WaterCurrent()
{
}

WaterCurrent::~WaterCurrent()
{
}

void WaterCurrent::addCurrent(Ogre::Vector3 start, Ogre::Vector3 end, float power, float minWidth)
{
	auto dir = end - start;
	float r = std::max(minWidth, dir.length() / 2.0f);
	auto pos = (end+start)/2.0f;

	dir.normalise();
	dir *= power;

	WCurrent cur;
	cur.dir = dir;
	cur.sqRad = r*r;
	cur.pos = pos;
	currents.push_back(cur);
}

Ogre::Vector3 WaterCurrent::getCurrent(Ogre::Vector3 pos)
{
	Vector3 out = Vector3::ZERO;
	float closestDistanceOffSq = 1000000;

	for (auto& c : currents)
	{
		float sqD = pos.squaredDistance(c.pos);

		if (sqD < c.sqRad)
		{
			closestDistanceOffSq = 0;
			out = c.pos;
		}
		else if (closestDistanceOffSq > (sqD - c.sqRad))
		{
			closestDistanceOffSq = sqD - c.sqRad;
			out = c.pos;
		}
	}

	return (closestDistanceOffSq<25) ? out : Vector3::ZERO;
}
