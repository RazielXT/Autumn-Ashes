#include "stdafx.h"
#include "CrowPath.h"
#include "MathUtils.h"

using namespace Ogre;

Ogre::Vector3 CrowPathAnimations::getCurrentPos()
{
	if (mFlightState == nullptr)
	{ }
	else
	{
		if (mTempState == nullptr)
		{
			Ogre::TransformKeyFrame key(0, 0);
			mFlightTrack->getInterpolatedKeyFrame(mFlightState->getTimePosition(), &key);

			lastPos = key.getTranslate();
		}
		else
		{
			Ogre::TransformKeyFrame key0(0, 0);
			mTempTrack->getInterpolatedKeyFrame(mTempState->getTimePosition(), &key0);

			Ogre::TransformKeyFrame key1(0, 0);
			mFlightTrack->getInterpolatedKeyFrame(mFlightState->getTimePosition(), &key1);

			lastPos = MathUtils::lerp(key0.getTranslate(), key1.getTranslate(), animWeight);		
		}
	}

	return lastPos;
}

Ogre::Quaternion CrowPathAnimations::getCurrentOr()
{
	if (mFlightState == nullptr)
	{ }
	else
	{
		if (mTempState == nullptr)
		{
			Ogre::TransformKeyFrame key(0, 0);
			mFlightTrack->getInterpolatedKeyFrame(mFlightState->getTimePosition(), &key);

			lastOr = key.getRotation();
		}
		else
		{
			Ogre::TransformKeyFrame key0(0, 0);
			mTempTrack->getInterpolatedKeyFrame(mTempState->getTimePosition(), &key0);

			Ogre::TransformKeyFrame key1(0, 0);
			mFlightTrack->getInterpolatedKeyFrame(mFlightState->getTimePosition(), &key1);

			lastOr = Quaternion::nlerp(animWeight, key0.getRotation(), key1.getRotation(), true);
		}
	}

	return lastOr;
}

void CrowPathAnimations::clearTemp()
{
	if (mTempState)
	{
		auto name = mTempTrack->getParent()->getName();
		Global::mSceneMgr->destroyAnimationState(name);
		Global::mSceneMgr->destroyAnimation(name);

		mTempState = nullptr;
		mTempTrack = nullptr;
	}
}

void CrowPathAnimations::clear()
{
	mFlightState = nullptr;
	mFlightTrack = nullptr;

	clearTemp();
}

CrowPath::CrowPath()
{
}

CrowPath::~CrowPath()
{
}

bool CrowPath::update(Ogre::Real tslf, Ogre::SceneNode* mNode)
{
	//update path/paths
	updateAnimState(tslf);

	mNode->setPosition(getPosition());
	mNode->setOrientation(getOrientation());

	//update state
	if (state == Lifting)
	{
		if (animState.mTempState->hasEnded())
		{
			animState.clearTemp();
			state = Flying;
		}
	}
	else if (state == Landing)
	{
		if (animState.mTempState->hasEnded())
		{
			animState.clearTemp();
			state = OnGround;
		}
	}
}

Ogre::Vector3 CrowPath::getPosition()
{
	//current +blending
	return animState.getCurrentPos();
}

Ogre::Quaternion CrowPath::getOrientation()
{
	//current +blending
	return animState.getCurrentOr();
}

void CrowPath::updateAnimState(Ogre::Real tslf)
{
	if (animState.mFlightState)
		animState.mFlightState->addTime(tslf);

	if (animState.mTempState)
		animState.mTempState->addTime(tslf);

	//update weight
	if (state == Lifting)
	{
		float rest = animState.mTempState->getLength() - animState.mTempState->getTimePosition();

		if (rest < animWeightSize)
		{
			animState.animWeight = 1.0 - Math::Clamp(rest / animWeightSize, 0.0f, 1.0f);
		}
		else
			animState.animWeight = 0;
	}
	else if (state == Landing)
	{
		float startOff = animState.mTempState->getTimePosition();

		if (startOff < animWeightSize)
		{
			animState.animWeight = Math::Clamp(startOff / animWeightSize, 0.0f, 1.0f);
		}
		else
			animState.animWeight = 0;
	}
	else
		animState.animWeight = 1;
}

void CrowPath::setLandingAnim(Ogre::Vector3 pos)
{
	if (state == Flying)
	{
		//create land anim + init
		createLandAnimation(animState.getCurrentPos(), animState.getCurrentOr(), pos);

		state = Landing;
	}
	else
	{
		animState.lastPos = pos;
		animState.lastOr = Quaternion(Degree(Math::RangeRandom(0, 360)), Vector3(0, 1, 0));
		state = OnGround;
	}
}

void CrowPath::setLiftingAnim(Ogre::Animation* flightAnim, float timePos)
{
	animState.mFlightTrack = flightAnim->getNodeTrack(0);
	animState.mFlightState = Global::mSceneMgr->createAnimationState(flightAnim->getName());
	animState.mFlightState->setTimePosition(timePos);

	if (state == OnGround)
	{
		//create lift anim + init
		Ogre::TransformKeyFrame key(0, 0);
		animState.mFlightTrack->getInterpolatedKeyFrame(animState.mFlightState->getTimePosition(), &key);

		createLiftAnimation(animState.getCurrentPos(), key.getTranslate(), key.getRotation());

		state = Lifting;
	}
	else
	{		
		state = Flying;
	}
}

void CrowPath::createLandAnimation(Vector3 startPos, Ogre::Quaternion startOr, Vector3 end)
{
	//create mTempTrack

	animWeightSize = 1;
}

void CrowPath::createLiftAnimation(Vector3 start, Vector3 endPos, Ogre::Quaternion endOr)
{
	//create mTempTrack

	animWeightSize = 1;
}