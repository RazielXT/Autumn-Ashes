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

    void addCrowFlight(int num_crows, float randomYaw, float mFlightMinTime, Ogre::SceneNode* node, int id);
    void addCrowLanding(int num_crows, float mRadius, Ogre::SceneNode* node, int id);

private:

    CrowsManager() {};
    static CrowsManager* ptr;

    std::vector<CrowsGroup*> subGroups;
};