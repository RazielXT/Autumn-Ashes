#include "stdafx.h"
#include "CrowPath.h"

using namespace Ogre;

Ogre::Vector3 CrowPathAnimations::getAnimPos()
{

}

Ogre::Quaternion CrowPathAnimations::getAnimOr()
{

}

void CrowPathAnimations::clear()
{

}

CrowPath::CrowPath()
{
	//init path
}

CrowPath::~CrowPath()
{
	//destroy path
}

bool CrowPath::update(Ogre::Real tslf, Ogre::SceneNode* mNode)
{
	//update path/paths + state

	mNode->setPosition(getPosition());
	mNode->setOrientation(getOrientation());
}

Ogre::Vector3 CrowPath::getPosition()
{
	//current +blending
}

Ogre::Quaternion CrowPath::getOrientation()
{
	//current +blending
}

float CrowPath::getPathTimeLeft()
{
	//for landing anim
	if (animState.mTempState != nullptr)
		return animState.mTempState->getLength() - animState.mTempState->getTimePosition();
	else
		return 100;
}

void CrowPath::setLandingAnim(Ogre::Vector3 pos)
{
	if (state == Flying)
	{
		//create land anim + init

		state = Landing;
	}
	else
	{
		animState.lastPos = pos;
		state = OnGround;
	}
}

void CrowPath::setLiftingAnim(Ogre::Animation* flightAnim, float timePos)
{
	if (state == OnGround)
	{
		//create lift anim + init

		state = Lifting;
	}
	else
	{
		animState.mFlightState = Global::mSceneMgr->createAnimationState(flightAnim->getName());
		animState.mFlightState->setTimePosition(timePos);
		state = Flying;
	}
}