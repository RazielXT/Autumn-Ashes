#pragma once

#include "stdafx.h"

#include "GUtils.h"

#define DEGUB_CROWS(x) GUtils::DebugPrint(x)
//#define DEGUB_CROWS(x)

enum CrowState { Standing, Walking, Flying, SwitchFlying, Landing, Lifting, None };

struct CrowPathAnimations
{
    mutable Ogre::Vector3 lastPos;
    mutable Ogre::Quaternion lastOr;

    Ogre::NodeAnimationTrack* mOldFlightTrack = nullptr;
    float mOldFlightLenght = -1;
    float mOldFlightPos;

    Ogre::NodeAnimationTrack* mFlightTrack = nullptr;
    float mFlightLenght = -1;
    float mFlightPos;

    Ogre::NodeAnimationTrack* mTempTrack = nullptr;
    float mTempLenght = -1;
    float mTempPos;

    float animWeight = 0;

    void refresh();

    void clearManuallyFlight();
    void clearOldFlight();
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

    bool update(Ogre::Real tslf, Ogre::SceneNode* node, Ogre::Quaternion& qOffset, Ogre::Vector3& pOffset);

    Ogre::Vector3 getPosition() const;
    Ogre::Quaternion getOrientation() const;

    CrowState state = None;
    void setLandingAnim(Ogre::Vector3 targetPos);
    void setLiftingAnim(Ogre::Animation* targetFlightAnim, float timePos);
    void setSwitchFlightAnim(Ogre::Animation* targetFlightAnim, float timePos);
    void setWalkingAnim(Ogre::Vector3 pos);

    float getTempTimeLeft();
    float getTempTime();

protected:

    void createLandAnimation(Ogre::Vector3 startPos, Ogre::Quaternion startOr, Ogre::Vector3 end);
    void createLiftAnimation(Ogre::Vector3 start, Ogre::Vector3 endPos, Ogre::Quaternion endOr);
    void createSwitchFlightAnimation(Ogre::Vector3 endPos, Ogre::Quaternion endOr);
    void createWalkAnimation(Ogre::Vector3 endPos);

    CrowPathAnimations animState;
    float animWeightSize = 1;
    void updateAnimState(Ogre::Real tslf);

};