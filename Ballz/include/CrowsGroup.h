#pragma once

#include "stdafx.h"
#include "EventTask.h"
#include "Crow.h"

class CrowFlight;
class CrowLanding;

class CrowsGroup
{
    friend class CrowsManager;

public:

    CrowsGroup(int mID) : id(mID) {};
    ~CrowsGroup();

    void update(Ogre::Real tslf);

    CrowFlight* getPossibleFlight();
    CrowLanding* getPossibleLanding();

    const int id;

private:

    std::vector<CrowFlight*> flights;
    std::vector<CrowLanding*> landings;

};

class CrowFlight
{
    friend class CrowsManager;

public:

    CrowFlight(int crows, float randomYaw, float flightAvgTime, float mSwitchAvgTime, Ogre::SceneNode* node, CrowsGroup* pgroup);
    ~CrowFlight();

    void update(Ogre::Real tslf);

    Ogre::Animation* createFlightAnim();
    void addCrow(Crow* crow);

private:

    void randomizeAnim(Ogre::Animation* anim);

    Ogre::Vector3 centerPos;
    float randomYawMax;
    float flightAvgTime;
    float switchAvgTime;

    std::vector<Ogre::Animation*> flightAnims;
    std::vector<Crow*> crows;
    CrowsGroup* group;
};

class CrowLanding
{
    friend class CrowsManager;

public:

    CrowLanding(int crows, int mMaxCrows, float mGroundAvgTime, bool mAllowWalk, Ogre::SceneNode* node, CrowsGroup* pgroup);
    ~CrowLanding();

    void update(Ogre::Real tslf);

    bool acceptsLanding() const;
    void addCrow(Crow* crow);

private:

    int maxCrows;
    float groundAvgTime;
    bool allowWalk;

    float maxPosOffsets;
    Ogre::Vector3 pos;

    std::vector<Crow*> crows;
    CrowsGroup* group;
};