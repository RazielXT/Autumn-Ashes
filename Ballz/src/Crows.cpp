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

Crow* CrowsManager::getRandomCrow() const
{
    auto gcopy = subGroups;
    std::random_shuffle(gcopy.begin(), gcopy.end());

    for (auto g : gcopy)
    {
        auto fcopy = g->flights;
        std::random_shuffle(fcopy.begin(), fcopy.end());

        for (auto f : fcopy)
        {
            if (f->crows.size() > 0)
            {
                int id = (int)Math::RangeRandom(0.0f, f->crows.size() - 0.01f);
                return f->crows[id];
            }
        }

        auto lcopy = g->landings;
        std::random_shuffle(lcopy.begin(), lcopy.end());

        for (auto l : lcopy)
        {
            if (l->crows.size() > 0)
            {
                int id = (int)Math::RangeRandom(0.0f, l->crows.size() - 0.01f);
                return l->crows[id];
            }
        }
    }

    return nullptr;
}