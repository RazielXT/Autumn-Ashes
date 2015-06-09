#include "stdafx.h"
#include "CrowPath.h"
#include "MathUtils.h"
#include "Player.h"

using namespace Ogre;

void CrowPathAnimations::refresh()
{
    if (mFlightTrack == nullptr)
    { 
		//onground
	}
    else
    {
		//flight
        if (mTempTrack == nullptr)
        {
            Ogre::TransformKeyFrame key(0, 0);
            mFlightTrack->getInterpolatedKeyFrame(mFlightPos, &key);

			lastOr = key.getRotation();
            lastPos = key.getTranslate();
        }
		//lift/land/switch
        else
        {
            Ogre::TransformKeyFrame key0(0, 0);
            mTempTrack->getInterpolatedKeyFrame(mTempPos, &key0);

            Ogre::TransformKeyFrame key1(0, 0);

			//check if start of switching
			if (mOldFlightTrack)
				mOldFlightTrack->getInterpolatedKeyFrame(mOldFlightPos, &key1);
			else
				mFlightTrack->getInterpolatedKeyFrame(mFlightPos, &key1);

			lastOr = Quaternion::Slerp(animWeight, key0.getRotation(), key1.getRotation(), true);
            lastPos = MathUtils::lerp(key0.getTranslate(), key1.getTranslate(), animWeight);
        }
    }
}

void CrowPathAnimations::clearOldFlight()
{
	if (mOldFlightTrack)
	{
		if (mOldFlightTrack)
			delete mOldFlightTrack->getParent();

		mOldFlightTrack = nullptr;
		mOldFlightLenght = -2;
	}
}

void CrowPathAnimations::clearTemp()
{
    if (mTempTrack)
    {
        auto name = mTempTrack->getParent()->getName();
        Global::mSceneMgr->destroyAnimation(name);

        mTempTrack = nullptr;
        mTempLenght = -2;
    }
}

void CrowPathAnimations::clear()
{
	if (mFlightTrack)
		delete mFlightTrack->getParent();

    mFlightTrack = nullptr;
    mFlightLenght = -2;

	clearOldFlight();

    clearTemp();
}

CrowPath::CrowPath()
{
}

CrowPath::~CrowPath()
{
}

bool CrowPath::update(Ogre::Real tslf, Ogre::SceneNode* mNode, Ogre::Quaternion& qOffset, Ogre::Vector3& pOffset)
{
    //update path/paths
    updateAnimState(tslf);

    //update state
    if (state == Lifting)
    {
        if (animState.mTempPos == -1)
        {
            animState.clearTemp();
            Global::DebugPrint("fly");
            state = Flying;
            return true;
        }
    }
    else if (state == Landing)
    {
        if (animState.mTempPos == -1)
        {
            animState.clear();
            Global::DebugPrint("ground");
            state = OnGround;
            return true;
        }
    }
	else if (state == SwitchFlying)
	{
		if (animState.mTempPos == -1)
		{
			animState.clearTemp();
			Global::DebugPrint("fly");
			state = Flying;
			return true;
		}
	}

	animState.refresh();

    mNode->setPosition(getPosition() + pOffset);
    mNode->setOrientation(getOrientation()*qOffset);

    return false;
}

Ogre::Vector3 CrowPath::getPosition()
{
    //current +blending
    return animState.lastPos;
}

Ogre::Quaternion CrowPath::getOrientation()
{
    //current +blending
	return animState.lastOr;
}

void CrowPath::updateAnimState(Ogre::Real tslf)
{
	if (animState.mOldFlightTrack)
		animState.mOldFlightPos = std::fmod(animState.mOldFlightPos + tslf, animState.mOldFlightLenght);
	else if (animState.mFlightPos >= 0 && animState.animWeight > 0)
        animState.mFlightPos = std::fmod(animState.mFlightPos + tslf, animState.mFlightLenght);

    if (animState.mTempPos >= 0)
    {
        animState.mTempPos += tslf;
        if (animState.mTempPos > animState.mTempLenght)
            animState.mTempPos = -1;
    }

	animState.animWeight = 1;

    //update weight
	if (state == Flying)
		animState.animWeight = 1;

	//from flight to temp
	if (state == Landing || (state == SwitchFlying && animState.mOldFlightTrack))
	{
		float startOff = animState.mTempPos;

		if (startOff < animWeightSize)
		{
			animState.animWeight = 1.0f - Math::Clamp(startOff / animWeightSize, 0.0f, 1.0f);

			//at end of oldtrack blending delete it
			if (animState.animWeight == 0)
				animState.clearOldFlight();
		}
		else
			animState.animWeight = 0;
	}

	//from temp to flight
	if (state == Lifting || (state == SwitchFlying && !animState.mOldFlightTrack))
	{
		float rest = animState.mTempLenght - animState.mTempPos;

		if (rest < animWeightSize)
		{
			animState.animWeight = 1.0f - Math::Clamp(rest / animWeightSize, 0.0f, 1.0f);
		}
		else
			animState.animWeight = 0;
	}
}

void CrowPath::setLandingAnim(Ogre::Vector3 pos)
{
    if (state == Flying)
    {
        //create land anim + init
        createLandAnimation(animState.lastPos, animState.lastOr, pos);
        Global::DebugPrint("land");
        state = Landing;
    }
    else
    {
        animState.lastPos = pos;
        animState.lastOr = Quaternion(Degree(Math::RangeRandom(0, 360)), Vector3(0, 1, 0));
        Global::DebugPrint("ground");

        state = OnGround;
    }
}

void CrowPath::setLiftingAnim(Ogre::Animation* flightAnim, float timePos)
{
    animState.mFlightTrack = flightAnim->getNodeTrack(0);
    animState.mFlightTrack->setUseShortestRotationPath(true);
    animState.mFlightTrack->_keyFrameDataChanged();
	animState.mFlightPos = timePos;
    animState.mFlightLenght = animState.mFlightTrack->getKeyFrame(animState.mFlightTrack->getNumKeyFrames() - 1)->getTime();

    if (state == OnGround)
    {
        //create lift anim + init
        Ogre::TransformKeyFrame key(0, 0);
		animState.mFlightTrack->getInterpolatedKeyFrame(timePos, &key);

        createLiftAnimation(animState.lastPos, key.getTranslate(), key.getRotation());
        Global::DebugPrint("lift");
        state = Lifting;
    }
    else
    {
        Global::DebugPrint("fly");
        state = Flying;
    }
}

void CrowPath::setSwitchFlightAnim(Ogre::Animation* flightAnim, float timePos)
{
	animState.mOldFlightTrack = animState.mFlightTrack;
	animState.mOldFlightPos = animState.mFlightPos;
	animState.mOldFlightLenght = animState.mFlightLenght;

	animState.mFlightTrack = flightAnim->getNodeTrack(0);
	animState.mFlightTrack->setUseShortestRotationPath(true);
	animState.mFlightTrack->_keyFrameDataChanged();
	animState.mFlightPos = timePos;
	animState.mFlightLenght = animState.mFlightTrack->getKeyFrame(animState.mFlightTrack->getNumKeyFrames() - 1)->getTime();

	if (state == Flying)
	{
		//create lift anim + init
		Ogre::TransformKeyFrame key(0, 0);
		animState.mFlightTrack->getInterpolatedKeyFrame(timePos, &key);

		createSwitchFlightAnimation(key.getTranslate(), key.getRotation());
		Global::DebugPrint("switch");
		state = SwitchFlying;
	}
	else
	{
		Global::DebugPrint("fly");
		state = Flying;
	}
}

float CrowPath::getTempTimeLeft()
{
    return animState.mTempLenght - animState.mTempPos;
}

float CrowPath::getTempTime()
{
    return animState.mTempPos;
}

void CrowPath::createSwitchFlightAnimation(Ogre::Vector3 endPos, Ogre::Quaternion endOr)
{
	static int counter = 0;

	float animSpeed = 1.5f;

	//create mTempTrack
	Animation* anim = Global::mSceneMgr->createAnimation("switch" + std::to_string(counter++), 3 / animSpeed);
	anim->setInterpolationMode(Animation::IM_SPLINE);
	anim->setRotationInterpolationMode(Animation::RIM_SPHERICAL);

	auto track = anim->createNodeTrack(0);
	track->setUseShortestRotationPath(true);

	Ogre::TransformKeyFrame* kf = track->createNodeKeyFrame(0);
	kf->setTranslate(animState.lastPos);
	kf->setRotation(animState.lastOr);

	//+side offset
	Vector3 flyDir(endPos - animState.lastPos);

	kf = track->createNodeKeyFrame(1 / animSpeed);
	Vector3 p1(animState.lastPos + flyDir / 3.0f);
	kf->setTranslate(p1);
	kf->setRotation(MathUtils::quaternionFromDir(p1 - animState.lastPos));

	kf = track->createNodeKeyFrame(2 / animSpeed);
	Vector3 p2(animState.lastPos + flyDir * 2.0f/ 3.0f);
	kf->setTranslate(p2);
	kf->setRotation(MathUtils::quaternionFromDir(p2 - p1));

	kf = track->createNodeKeyFrame(3 / animSpeed);
	kf->setTranslate(endPos);
	kf->setRotation(endOr);

	animState.mTempTrack = track;
	animState.mTempPos = 0;
	animState.mTempLenght = track->getKeyFrame(track->getNumKeyFrames() - 1)->getTime();

	animWeightSize = 1;
}

void CrowPath::createLandAnimation(Vector3 startPos, Ogre::Quaternion startOr, Vector3 end)
{
    static int counter = 0;

    float animSpeed = 1;// 0.15f;

    //create mTempTrack
    Animation* anim = Global::mSceneMgr->createAnimation("landing" + std::to_string(counter++), 2 / animSpeed);
    anim->setInterpolationMode(Animation::IM_SPLINE);
    anim->setRotationInterpolationMode(Animation::RIM_SPHERICAL);

    auto track = anim->createNodeTrack(0);
    track->setUseShortestRotationPath(true);

    Vector3 landDir(end - startPos);
    landDir.normalise();
    Vector3 landPrepPos = end - landDir*5 + Vector3(0, 0.5f, 0);
    Vector3 halfPos = (landPrepPos + startPos) / 2 - Vector3(0, 1, 0);
    Quaternion neutralDir = MathUtils::quaternionFromDirNoPitch(landDir);

    Ogre::TransformKeyFrame* kf = track->createNodeKeyFrame(0);
    kf->setTranslate(startPos);
    kf->setRotation(startOr);

    kf = track->createNodeKeyFrame(1 / animSpeed);
    kf->setTranslate(halfPos);
    kf->setRotation(MathUtils::quaternionFromDir(landPrepPos - startPos));

    kf = track->createNodeKeyFrame((2 - 0.35f) / animSpeed);
    kf->setTranslate(landPrepPos);
    kf->setRotation(neutralDir);

    kf = track->createNodeKeyFrame(2 / animSpeed);
    kf->setTranslate(end);
    kf->setRotation(neutralDir);

    animState.mTempTrack = track;
    animState.mTempPos = 0;
    animState.mTempLenght = track->getKeyFrame(track->getNumKeyFrames() - 1)->getTime();

    animWeightSize = 1;
}

void CrowPath::createLiftAnimation(Vector3 start, Vector3 endPos, Ogre::Quaternion endOr)
{
    static int counter = 0;

    float animSpeed = 1.15f;

    //create mTempTrack
    Animation* anim = Global::mSceneMgr->createAnimation("lifting" + std::to_string(counter++), 2 / animSpeed);
    anim->setInterpolationMode(Animation::IM_SPLINE);
    anim->setRotationInterpolationMode(Animation::RIM_SPHERICAL);

    auto track = anim->createNodeTrack(0);
    track->setUseShortestRotationPath(true);

    Vector3 flightDir(endPos - start);
    flightDir.normalise();
    Vector3 jumpPos = start + flightDir + Vector3(0, 2.5f, 0);
    Vector3 halfPos = (jumpPos + endPos) / 2 - Vector3(0, 1, 0);


    Ogre::TransformKeyFrame* kf = track->createNodeKeyFrame(0);
    kf->setTranslate(start);
    kf->setRotation(animState.lastOr);

    kf = track->createNodeKeyFrame(0.35f/animSpeed);
    kf->setTranslate(jumpPos);
    kf->setRotation(MathUtils::quaternionFromDir(halfPos - jumpPos));

    kf = track->createNodeKeyFrame(1 / animSpeed);
    kf->setTranslate(halfPos);
    kf->setRotation(MathUtils::quaternionFromDir(endPos - halfPos));

    kf = track->createNodeKeyFrame(2 / animSpeed);
    kf->setTranslate(endPos);
    kf->setRotation(endOr);

    animState.mTempTrack = track;
    animState.mTempPos = 0;
    animState.mTempLenght = track->getKeyFrame(track->getNumKeyFrames() - 1)->getTime();

    animWeightSize = 1;
}