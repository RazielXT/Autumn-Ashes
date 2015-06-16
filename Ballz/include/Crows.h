#pragma once

#include "stdafx.h"
#include "EventTask.h"
#include "CrowsGroup.h"

class CrowsManager : public EventTask
{
public:

    static CrowsManager* get();

    ~CrowsManager();

    bool update(Ogre::Real tslf);

    void addCrowFlight(int num_crows, float randomYaw, float mFlightAvgTime, float mSwitchAvgTime, Ogre::SceneNode* node, int id);
    void addCrowLanding(int num_crows, int mMaxCrows, float mGroundAvgTime, bool mAllowWalk, Ogre::SceneNode* node, int id);

    Crow* getRandomCrow() const;

private:

    CrowsManager() {};
    static CrowsManager* ptr;

    std::vector<CrowsGroup*> subGroups;
};