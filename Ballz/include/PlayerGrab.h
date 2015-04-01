#pragma once

#include "stdafx.h"

class Player;

class PlayerGrab
{
    friend class Player;

    Player* p;
    OgreNewt::Body* body;

    //grabbed obj
    Ogre::Vector3 gADT;
    Ogre::Real gLDT;
    OgreNewt::Body* Gbody;

public:

    void grabbed_callback(OgreNewt::Body* me, float timeStep, int threadIndex);

    PlayerGrab(Player* player);

    void tryToGrab();
    void releaseObj();
};