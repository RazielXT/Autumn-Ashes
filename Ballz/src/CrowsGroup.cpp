#include "stdafx.h"
#include "CrowsGroup.h"

using namespace Ogre;

CrowsGroup::~CrowsGroup()
{
    //destroy vectors
    for (auto c : landings)
    {
        delete c;
    }

    for (auto c : flights)
    {
        delete c;
    }
}

void CrowsGroup::update(Ogre::Real tslf)
{
    tslf *= 0.8f;

    //update vectors
    for (auto l : landings)
    {
        l->update(tslf);
    }

    for (auto f : flights)
    {
        f->update(tslf);
    }
}

CrowFlight* CrowsGroup::getPossibleFlight()
{
    //try in all
    /*for (auto f : flights)
    {
    return f;
    }*/

    int rand = (int)Ogre::Math::RangeRandom(0, flights.size() - 0.01f);
    return flights[rand];
}

CrowLanding* CrowsGroup::getPossibleLanding()
{
    int rand = (int)Ogre::Math::RangeRandom(0, landings.size() - 0.01f);
    return landings[rand];

    //try in all
    for (auto l : landings)
    {
        if (l->acceptsLanding()) return l;
    }

    return nullptr;
}

CrowFlight::CrowFlight(int num_crows, float randomYaw, float mFlightAvgTime, float mSwitchAvgTime, Ogre::SceneNode* node, CrowsGroup* pgroup)
{
    randomYawMax = randomYaw;
    flightAvgTime = mFlightAvgTime;
    switchAvgTime = mSwitchAvgTime;

    //init paths
    auto animNode = Global::mSceneMgr->getSceneNode(node->getName() + "Anim");
    auto anims = Global::mSceneMgr->getAnimations();

    centerPos = node->_getDerivedPosition();

    for (auto it = anims.cbegin(); it != anims.cend(); it++)
    {
        if (it->second->getNumNodeTracks()==0)
            continue;

        auto track = it->second->getNodeTrack(0);

        if (track->getAssociatedNode() == animNode)
            flightAnims.push_back(it->second);
    }

    //init crows
    for (int i = 0; i < num_crows; i++)
    {
        addCrow(new Crow(false));
    }

    group = pgroup;
}

CrowFlight::~CrowFlight()
{
    //destroy crows
    for (auto c : crows)
    {
        delete c;
    }
}

void CrowFlight::update(Ogre::Real tslf)
{
    //update crows
    //change crow state if available
    auto c = crows.begin();
    while (c != crows.end())
    {
        (*c)->update(tslf);

        if ((*c)->readyToLand())
        {
            CrowLanding* l = group->getPossibleLanding();

            if (l)
            {
                l->addCrow(*c);
                crows.erase(c);
                continue;
            }
        }
        else if ((*c)->readyToChangeFlyPath())
        {
            (*c)->switchFlyTo(createFlightAnim());
        }

        c++;
    }
}

void CrowFlight::addCrow(Crow* crow)
{
    crow->minFlightTime = flightAvgTime / 2.0f + Ogre::Math::RangeRandom(0, flightAvgTime);
    crow->switchFlightTime = switchAvgTime / 2.0f + Ogre::Math::RangeRandom(0, switchAvgTime);

    crow->flyTo(createFlightAnim());
    crows.push_back(crow);
}

Ogre::Animation* CrowFlight::createFlightAnim()
{
    static int flightsNum = 0;

    //find best free path/ random may be enough?
    int rand = (int)Ogre::Math::RangeRandom(0, flightAnims.size() - 0.01f);
    auto chosenAnim = flightAnims[rand];

    //create cloned
    auto newAnim = chosenAnim->clone(chosenAnim->getName() + std::to_string(flightsNum++));

    //randomize
    randomizeAnim(newAnim);

    return newAnim;
}

void CrowFlight::randomizeAnim(Ogre::Animation* anim)
{
    auto track = anim->getNodeTrack(0);
    track->setUseShortestRotationPath(true);
    track->_keyFrameDataChanged();

    float yaw = Math::RangeRandom(-randomYawMax / 2.0f, randomYawMax / 2.0f);

    Quaternion rot(Degree(yaw),Vector3(0, 1, 0));

    //todo fix rot maybe?
    for (size_t i = 0; i < track->getNumKeyFrames(); i++)
    {
        TransformKeyFrame* key = (TransformKeyFrame*)track->getKeyFrame(i);
        Vector3 centralized = key->getTranslate() - centerPos;
        key->setTranslate(centerPos + rot*centralized);
        key->setRotation(key->getRotation()*rot);
    }
}

CrowLanding::CrowLanding(int crows, int mMaxCrows, float mGroundAvgTime, bool mAllowWalk, Ogre::SceneNode* node, CrowsGroup* pgroup)
{
    groundAvgTime = mGroundAvgTime;
    maxCrows = mMaxCrows;
    allowWalk = mAllowWalk;

    pos = node->_getDerivedPosition();
    auto bounds = node->getAttachedObject(0)->getBoundingBox().getHalfSize();

    maxPosOffsets = std::max(bounds.x, bounds.z);

    //init crows
    for (int i = 0; i < crows && acceptsLanding(); i++)
    {
        addCrow(new Crow(true));
    }

    group = pgroup;
}

CrowLanding::~CrowLanding()
{
    //destroy crows
    for (auto c : crows)
    {
        delete c;
    }
}

void CrowLanding::update(Ogre::Real tslf)
{
    //update crows
    for (auto c : crows)
    {
        c->update(tslf);
    }

    //change crow state if available (crow->group->flight)
    auto c = crows.begin();
    while (c != crows.end())
    {
        if ((*c)->readyToFly())
        {
            CrowFlight* f = group->getPossibleFlight();

            if (f)
            {
                f->addCrow(*c);
                crows.erase(c);
                continue;
            }
        }

        c++;
    }
}

bool CrowLanding::acceptsLanding() const
{
    //check if free
    return (crows.size() < maxCrows);
}

void CrowLanding::addCrow(Crow* crow)
{
    crow->minGroundTime = groundAvgTime / 2.0f + Ogre::Math::RangeRandom(0, groundAvgTime);
    crow->allowedWalking = allowWalk;

    crow->landTo(pos, maxPosOffsets);
    crows.push_back(crow);
}
