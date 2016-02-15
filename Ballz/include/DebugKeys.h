#pragma once

#include "stdafx.h"
#include "PostProcessMgr.h"
#include "EffectsTasks.h"


class DebugKeys : public InputListener
{
public:
    DebugKeys()
    {
        executionState = UNDEFINED;
    }

    void pressedKey(const OIS::KeyEvent &arg);

    void reloadShaders();

    void default_callback(OgreNewt::Body* me, float timeStep, int threadIndex)
    {
        me->addForce(Ogre::Vector3(0, -6, 0));
    }

    void makecube(bool platform = false);

};