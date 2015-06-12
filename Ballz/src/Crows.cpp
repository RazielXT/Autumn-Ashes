#include "stdafx.h"
#include "Crows.h"
#include "CrowsGroup.h"

using namespace Ogre;

CrowsManager* CrowsManager::ptr = nullptr;

CrowsManager* CrowsManager::get()
{
    if (!ptr)
    {
        ptr = new CrowsManager();
        Global::mEventsMgr->addTask(ptr);
    }

    return ptr;
}

CrowsManager::~CrowsManager()
{
    for (auto g : subGroups)
    {
        delete g;
    }

    ptr = nullptr;
}

bool CrowsManager::update(Ogre::Real tslf)
{
    for (auto g : subGroups)
    {
        g->update(tslf);
    }

    return true;
}

void CrowsManager::addCrowFlight(int num_crows, float randomYaw, float mFlightMinTime, float mSwitchMinTime, Ogre::SceneNode* node, int id)
{
    for (auto g : subGroups)
    {
        if (g->id == id)
        {
            auto f = new CrowFlight(num_crows, randomYaw, mFlightMinTime, mSwitchMinTime, node, g);
            g->flights.push_back(f);
            return;
        }
    }

    CrowsGroup* g = new CrowsGroup(id);
	auto f = new CrowFlight(num_crows, randomYaw, mFlightMinTime, mSwitchMinTime, node, g);
    g->flights.push_back(f);
    subGroups.push_back(g);
}

void CrowsManager::addCrowLanding(int num_crows, int mMaxCrows, float mGroundAvgTime, bool mAllowWalk, Ogre::SceneNode* node, int id)
{
    for (auto g : subGroups)
    {
        if (g->id == id)
        {
			auto l = new CrowLanding(num_crows, mMaxCrows, mGroundAvgTime, mAllowWalk, node, g);
            g->landings.push_back(l);
            return;
        }
    }

    CrowsGroup* g = new CrowsGroup(id);
	auto l = new CrowLanding(num_crows, mMaxCrows, mGroundAvgTime, mAllowWalk, node, g);
    g->landings.push_back(l);
    subGroups.push_back(g);
}