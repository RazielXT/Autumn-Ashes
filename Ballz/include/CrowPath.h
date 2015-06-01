#pragma once

#include "stdafx.h"

enum PathState { OnGround, Flying, Landing, Lifting, None };

struct CrowPathAnimations
{
    Ogre::Vector3 lastPos;
    Ogre::Quaternion lastOr;

    Ogre::NodeAnimationTrack* mFlightTrack = nullptr;
    float mFlightLenght = -1;
    float mFlightPos;

    Ogre::NodeAnimationTrack* mTempTrack = nullptr;
    float mTempLenght = -1;
    float mTempPos;

    float animWeight = 0;

    Ogre::Vector3 getCurrentPos();
    Ogre::Quaternion getCurrentOr();

    void clearTemp();
    void clear();

    ~CrowPathAnimations()
    {
        clear();
    }
};

class CrowPath
{
public:

    CrowPath();
    ~CrowPath();

    bool update(Ogre::Real tslf, Ogre::SceneNode* node);

    Ogre::Vector3 getPosition();
    Ogre::Quaternion getOrientation();

    PathState state = None;
    void setLandingAnim(Ogre::Vector3 targetPos);
    void setLiftingAnim(Ogre::Animation* targetFlightAnim, float timePos);

protected:

    void createLandAnimation(Ogre::Vector3 startPos, Ogre::Quaternion startOr, Ogre::Vector3 end);
    void createLiftAnimation(Ogre::Vector3 start, Ogre::Vector3 endPos, Ogre::Quaternion endOr);

    CrowPathAnimations animState;
    float animWeightSize = 1;
    void updateAnimState(Ogre::Real tslf);

};