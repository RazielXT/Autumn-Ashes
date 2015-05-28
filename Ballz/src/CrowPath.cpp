#include "stdafx.h"
#include "CrowPath.h"

using namespace Ogre;

CrowPath::CrowPath()
{
	//init path
}

CrowPath::~CrowPath()
{
	//destroy path
}

bool CrowPath::update(Ogre::Real tslf)
{
	//update path/paths + state
}

Ogre::Vector3 CrowPath::getPosition()
{
	//current +blending
}

Ogre::Vector3 CrowPath::getOrientation()
{
	//current +blending
}

float CrowPath::getPathTimeLeft()
{
	//for landing anim
}

void CrowPath::setLandingAnim(Ogre::Vector3 pos)
{
	//creation
}

void CrowPath::setLiftingAnim(Ogre::Animation* flightAnim, float timePos)
{
	//creation
}