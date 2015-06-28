#pragma once
#include "stdafx.h"
#include "InputListener.h"
#include "Player.h"
#include "Crows.h"
#include "MUtils.h"

using namespace Ogre;

class CrowWatch : public EventTask, public InputListener
{

public:

    CrowWatch();

    bool update(Ogre::Real tslf);

    void activate(bool activate);

    virtual void pressedKey(const OIS::KeyEvent &arg);

private:

    Ogre::SceneNode* mNode;
    Crow* targetCrow;
};

CrowWatch::CrowWatch()
{
    targetCrow = nullptr;
    mNode = Global::mSceneMgr->getRootSceneNode()->createChildSceneNode();
    registerInputListening();

    Global::mEventsMgr->addTask(this);
}

void CrowWatch::activate(bool activate)
{
    if (activate)
    {
        targetCrow = CrowsManager::get()->getRandomCrow();

        if (targetCrow)
        {
            auto cam = Global::player->detachCamera();
            mNode->attachObject(cam);
        }
    }
    else if (targetCrow)
    {
        Global::player->attachCamera();
        targetCrow = nullptr;
    }
}

bool CrowWatch::update(Ogre::Real tslf)
{
    if (targetCrow)
    {
        Vector3 offset(5, 5, 5);

        mNode->setPosition(targetCrow->getNode()->_getDerivedPosition() + offset);

        offset.normalise();
        offset *= -1;
        Quaternion or = Vector3::NEGATIVE_UNIT_Z.getRotationTo(offset);
        mNode->setOrientation(or);
    }

    return true;
}

void CrowWatch::pressedKey(const OIS::KeyEvent &arg)
{
    if (arg.key == OIS::KC_F1)
    {
        activate(true);
    }
    else if (arg.key == OIS::KC_F2)
    {
        activate(false);
    }
}