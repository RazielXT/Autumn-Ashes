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

ZipLine::ZipLine(const std::vector<Ogre::Vector3>& points)
{
	initZipLine(points);
}

void ZipLine::initZipLine(const std::vector<Ogre::Vector3>& points)
{
	zipLine.clear();
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
			point.dir = (points[points.size() - 1] - points[points.size() - 2]).normalise();
		//else inside
		else
		{
			auto dirB = (points[i] - points[i - 1]);
			dirB.normalise();
			auto dirF = (points[i + 1] - points[i]);
			dirF.normalise();

			point.dir = (dirB + dirF).normalise();
		}
	}

	zipLine[0].lenghtCoef = 1;
	zipLine[zipLine.size()-1].lenghtCoef = 1;

	for (int i = 1; i < points.size() - 1; i++)
	{
		auto generalDir = zipLine[i + 1].pos - zipLine[i].pos;
		auto angleDeviation = zipLine[i].dir.angleBetween(generalDir);
		angleDeviation += zipLine[i + 1].dir.angleBetween(generalDir);

		//180 deg = PI deviation => full circle instead of straight line => 4*r=2*PI*r => w=PI/2 => w=dev/PI, min 1
		zipLine[i].lenghtCoef = 1 + ((angleDeviation.valueRadians() / Ogre::Math::PI) * ((Ogre::Math::PI / 2.0f) - 1));
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
		sliding.speed = 3;
		active = true;

		return true;
	}
	
	return false;
}

void ZipLine::updateSlidingSpeed(float time)
{
	sliding.speed = 3;
}

void ZipLine::release()
{
	active = false;
}

void ZipLine::updateSlidingState(float time)
{
	updateSlidingSpeed(time);

	auto lineLenght = (zipLine[sliding.mPoint + 1].pos - zipLine[sliding.mPoint].pos).length() * zipLine[sliding.mPoint].lenghtCoef;

	//jump to next line segment
	while (lineLenght*(1 - sliding.mProgress) < time*sliding.speed && (sliding.mPoint != zipLine.size()-1))
	{
		time -= lineLenght*(1 - sliding.mProgress) / sliding.speed;

		sliding.mPoint++;
		sliding.mProgress = 0;

		if ((sliding.mPoint != zipLine.size() - 1))
			lineLenght = (zipLine[sliding.mPoint + 1].pos - zipLine[sliding.mPoint].pos).length() * zipLine[sliding.mPoint].lenghtCoef;
	}

	bool pastEnd = (sliding.mPoint == zipLine.size() - 1);

	//get new pos, if past, get him back
	if (!pastEnd)
	{
		auto addProgress = time*sliding.speed / lineLenght;
		sliding.mProgress += addProgress;

		getCurrentLinePos();
	}
	else
		sliding.currentPos = zipLine[zipLine.size() - 1].pos -= zipLine[zipLine.size() - 1].dir*MIN_SLIDE_DISTANCE;

	//past/near end
	if (pastEnd || ((sliding.mPoint == zipLine.size() - 2) && (1 - sliding.mProgress)*lineLenght<MIN_SLIDE_DISTANCE))
		release();
}

void ZipLine::getCurrentLinePos()
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

	sliding.currentPos = posBase + posOffset;

	//direction 0-0.5-1 = startDir/lineDir/endDir
	auto generalDir = zipLine[sliding.mPoint + 1].pos - zipLine[sliding.mPoint].pos;
	generalDir.normalise();

	if (sliding.mProgress<0.5f)
		sliding.currentDir = zipLine[sliding.mPoint].dir*(1 - sliding.mProgress * 2) + generalDir*sliding.mProgress * 2;
	else
		sliding.currentDir = generalDir*(1 - (sliding.mProgress - 0.5) * 2) + zipLine[sliding.mPoint + 1].dir*(sliding.mProgress - 0.5) * 2;
	
}

bool ZipLine::update(Ogre::Real tslf)
{
	updateSlidingState(tslf);

	return active;
}