#include "stdafx.h"
#include "ZipLine.h"
#include "Player.h"

using namespace Ogre;

void ZipLine::pressedKey(const OIS::KeyEvent &arg)
{
	if (arg.key == OIS::KC_SPACE)
	{

	}
}

ZipLine::ZipLine(std::vector<Ogre::Vector3> points)
{
	zipLine.resize(points.size());

	for (int i = 0; i < points.size(); i++)
	{
		ZipLinePoint& point = zipLine[i];
		point.pos = points[i];

		//first 2
		if (i == 0 || i == 1)
			point.dir = (points[1] - points[0]).normalise();
		//last 2
		else if (i >= points.size() - 2)
			point.dir = (points[points.size() - 1] - points[points.size()-2]).normalise();
		//rest inside
		else
		{
			auto dirB = (points[i] - points[i-1]);
			dirB.normalise();
			auto dirF = (points[i + 1] - points[i]);
			dirF.normalise();

			point.dir = (dirB + dirF).normalise();
		}
	}
}

ZipLine::LineProjState ZipLine::getProjectedState(Ogre::Vector3& point, Ogre::Vector3& start, Ogre::Vector3& end)
{
	Vector3 dir = end - start;
	Vector3 pDir = point - start;
	dir.normalise();
	auto dp = pDir.dotProduct(dir);
	dir *= dp;

	//auto lToP = dir.length();
	auto lleft = (end - dir).length();

	LineProjState state;
	state.projPos = dir;
	state.cProgress = dp;
	state.left = lleft;
	state.sqDistance = std::min(point.squaredDistance(end), std::min(point.squaredDistance(start), point.squaredDistance(state.projPos)));

	return state;
}

#define MAX_PLAYER_DISTANCE 10
#define MIN_SLIDE_DISTANCE 3

bool ZipLine::placePointOnLine(Vector3& point)
{
	sliding.mPoint = 0;
	sliding.speed = 0;
	sliding.mProgress = 0;

	auto zipPos = zipLine[0];
	float minDist = MAX_PLAYER_DISTANCE;
	float leftDist = 1000;

	for (int id = 1; id < zipLine.size(); id++)
	{
		auto state = getProjectedState(point, zipLine[id - 1].pos, zipLine[id].pos);

		if (state.sqDistance < minDist)
		{
			sliding.mPoint = id;
			sliding.mProgress = state.cProgress;
			minDist = state.sqDistance;
			leftDist = state.left;
		}
	}

	bool nearEnd = ((sliding.mPoint == zipLine.size() - 1) && leftDist < MIN_SLIDE_DISTANCE);

	return !(nearEnd || minDist == MAX_PLAYER_DISTANCE);
}

bool ZipLine::start()
{
	auto pos = Global::player->getBody()->getPosition();

	if (placePointOnLine(pos))
	{
		//TODO attach player

		return true;
	}
	
	return false;
}

Vector3 ZipLine::getCurrentLinePos()
{
	auto posBase = zipLine[sliding.mPoint].pos*(1 - sliding.mProgress) + zipLine[sliding.mPoint + 1].pos*sliding.mProgress;

	float c = sliding.mProgress;
	auto startDir = zipLine[sliding.mPoint].dir;
	auto endDirInv = zipLine[sliding.mPoint + 1].dir * -1;

	float l = zipLine[sliding.mPoint].pos.distance(zipLine[sliding.mPoint+1].pos);
	float w = std::min(c * 2, 2 - c * 2);
	float angleWeight = 1.0f;
	float offWeight = w*l*angleWeight;

	auto posOffset = ((1 - c)*startDir + c*endDirInv)*offWeight;

	auto pos = posBase + posOffset;

	//TODO compute direction

	return pos;
}

bool ZipLine::update(Ogre::Real tslf)
{

}