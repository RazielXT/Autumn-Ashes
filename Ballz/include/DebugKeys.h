#pragma once

#include "stdafx.h"
#include "PostProcessMgr.h"
#include "EffectsTasks.h"


struct DebugVar
{
    std::string name;
    float* target;
    float step;

    DebugVar()
    {
        target = nullptr;
    }

    DebugVar(std::string n, float* tar, float st) : name(n), target(tar), step(st)
    {}
};

class DebugKeys : public InputListener
{
public:
    DebugKeys()
    {
        executionState = UNDEFINED;
    }

    void reloadVariables();

    void pressedKey(const OIS::KeyEvent &arg);


    void reloadShaders();

    void default_callback(OgreNewt::Body* me, float timeStep, int threadIndex)
    {
        me->addForce(Ogre::Vector3(0, -6, 0));
    }

    void makecube(bool platform = false);

    std::vector<DebugVar> debugVars;
    int debugVarsLine = 0;
};