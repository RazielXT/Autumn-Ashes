#include "stdafx.h"
#include "CrowPath.h"
#include "MathUtils.h"

using namespace Ogre;

Ogre::Vector3 CrowPathAnimations::getCurrentPos()
{
    if (mFlightTrack == nullptr)
    { }
    else
    {
        if (mTempTrack == nullptr)
        {
            Ogre::TransformKeyFrame key(0, 0);
            mFlightTrack->getInterpolatedKeyFrame(mFlightPos, &key);

            lastPos = key.getTranslate();
        }
        else
        {
            Ogre::TransformKeyFrame key0(0, 0);
            mTempTrack->getInterpolatedKeyFrame(mTempPos, &key0);

            Ogre::TransformKeyFrame key1(0, 0);
            mFlightTrack->getInterpolatedKeyFrame(mFlightPos, &key1);

            lastPos = MathUtils::lerp(key0.getTranslate(), key1.getTranslate(), animWeight);
        }
    }

    return lastPos;
}

Ogre::Quaternion CrowPathAnimations::getCurrentOr()
{
    if (mFlightTrack == nullptr)
    { }
    else
    {
        if (mTempTrack == nullptr)
        {
            Ogre::TransformKeyFrame key(0, 0);
            mFlightTrack->getInterpolatedKeyFrame(mFlightPos, &key);

            lastOr = key.getRotation();
        }
        else
        {
            Ogre::TransformKeyFrame key0(0, 0);
            mTempTrack->getInterpolatedKeyFrame(mTempPos, &key0);

            Ogre::TransformKeyFrame key1(0, 0);
            mFlightTrack->getInterpolatedKeyFrame(mFlightPos, &key1);

            lastOr = Quaternion::nlerp(animWeight, key0.getRotation(), key1.getRotation(), true);
        }
    }

    return lastOr;
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
    delete mFlightTrack;
    mFlightTrack = nullptr;
    mFlightLenght = -2;

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

    mNode->setPosition(getPosition());
    mNode->setOrientation(getOrientation());

    return false;
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
    if (animState.mFlightPos >= 0)
        animState.mFlightPos = std::fmod(animState.mFlightPos + tslf, animState.mFlightLenght);

    if (animState.mTempPos >= 0)
    {
        animState.mTempPos += tslf;
        if (animState.mTempPos > animState.mTempLenght)
            animState.mTempPos = -1;
    }


    //update weight
    if (state == Lifting)
    {
        float rest = animState.mTempLenght - animState.mTempPos;

        if (rest < animWeightSize)
        {
            animState.animWeight = 1.0f - Math::Clamp(rest / animWeightSize, 0.0f, 1.0f);
        }
        else
            animState.animWeight = 0;
    }
    else if (state == Landing)
    {
        float startOff = animState.mTempPos;

        if (startOff < animWeightSize)
        {
            animState.animWeight = 1.0f - Math::Clamp(startOff / animWeightSize, 0.0f, 1.0f);
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
    animState.mFlightPos = 0;
    animState.mFlightLenght = animState.mFlightTrack->getKeyFrame(animState.mFlightTrack->getNumKeyFrames() - 1)->getTime();

    if (state == OnGround)
    {
        //create lift anim + init
        Ogre::TransformKeyFrame key(0, 0);
        animState.mFlightTrack->getInterpolatedKeyFrame(0, &key);

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

void CrowPath::createLandAnimation(Vector3 startPos, Ogre::Quaternion startOr, Vector3 end)
{
    static int counter = 0;

    //create mTempTrack
    Animation* anim = Global::mSceneMgr->createAnimation("landing" + std::to_string(counter++), 2);
    anim->setInterpolationMode(Animation::IM_SPLINE);

    auto track = anim->createNodeTrack(0);
    track->setUseShortestRotationPath(true);

    Ogre::TransformKeyFrame* kf = track->createNodeKeyFrame(0);
    kf->setTranslate(startPos);
    kf->setRotation(startOr);

    kf = track->createNodeKeyFrame(2);
    kf->setTranslate(end);
    kf->setRotation(startOr);

    animState.mTempTrack = track;
    animState.mTempPos = 0;
    animState.mTempLenght = track->getKeyFrame(track->getNumKeyFrames() - 1)->getTime();


    animWeightSize = 1;
}

void CrowPath::createLiftAnimation(Vector3 start, Vector3 endPos, Ogre::Quaternion endOr)
{
    static int counter = 0;

    //create mTempTrack
    Animation* anim = Global::mSceneMgr->createAnimation("lifting" + std::to_string(counter++), 2);
    anim->setInterpolationMode(Animation::IM_SPLINE);

    auto track = anim->createNodeTrack(0);
    track->setUseShortestRotationPath(true);

    Ogre::TransformKeyFrame* kf = track->createNodeKeyFrame(0);
    kf->setTranslate(start);
    kf->setRotation(animState.lastOr);

    kf = track->createNodeKeyFrame(2);
    kf->setTranslate(endPos);
    kf->setRotation(endOr);

    animState.mTempTrack = track;
    animState.mTempPos = 0;
    animState.mTempLenght = track->getKeyFrame(track->getNumKeyFrames() - 1)->getTime();

    animWeightSize = 1;
}