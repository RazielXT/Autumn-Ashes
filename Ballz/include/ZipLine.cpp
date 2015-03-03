#include "stdafx.h"
#include "ZipLine.h"
#include "Player.h"

using namespace Ogre;

#define MIN_SLIDE_DISTANCE 3
#define LOOSENESS_OF_LINE 0.05f

ZipLine::ZipLineState ZipLine::getProj(Ogre::Vector3 point)
{
	Vector3 dir = endD - startD;
	auto len = dir.length();
	Vector3 pDir = point - startD;
	dir.normalise();
	auto dp = pDir.dotProduct(dir);
	dir *= dp;

	//auto lToP = dir.length();
	auto lleft = (endD - dir).length();

	ZipLineState state;
	state.fullL = len;
	state.projPos = dir;
	state.cProgress = dp;
	state.left = lleft;

	return state;
}

bool ZipLine::start()
{
	auto pos = Global::player->getBody()->getPosition();
	auto state = getProj(pos);

	if (state.left < MIN_SLIDE_DISTANCE || state.left > state.fullL)
		return false;

}

Vector3 ZipLine::getPPos(ZipLine::ZipLineState state)
{
	Vector3 pos = state.projPos;
	pos.y += sin(state.cProgress*Ogre::Math::PI)* state.fullL*LOOSENESS_OF_LINE;

	return pos;
}

bool ZipLine::update(Ogre::Real tslf)
{

}