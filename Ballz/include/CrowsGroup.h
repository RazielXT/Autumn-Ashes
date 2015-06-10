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
public:

    CrowFlight(int crows, float randomYaw, float flightMinTime, Ogre::SceneNode* node, CrowsGroup* pgroup);
    ~CrowFlight();

    void update(Ogre::Real tslf);

    Ogre::Animation* createFlightAnim();
    void addCrow(Crow* crow);

private:

    void randomizeAnim(Ogre::Animation* anim);

    Ogre::Vector3 centerPos;
    float randomYawMax;
    float flightMinTime;

    std::vector<Ogre::Animation*> flightAnims;
    std::vector<Crow*> crows;
    CrowsGroup* group;
};

class CrowLanding
{
public:

    CrowLanding(int crows, float radius, Ogre::SceneNode* node, CrowsGroup* pgroup);
    ~CrowLanding();

    void update(Ogre::Real tslf);

    bool acceptsLanding() const;
    void addCrow(Crow* crow);

private:

    float radius;
    Ogre::Vector3 pos;

    std::vector<Crow*> crows;
    CrowsGroup* group;
};