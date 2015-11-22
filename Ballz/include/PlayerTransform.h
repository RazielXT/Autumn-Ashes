#pragma once

#include "stdafx.h"
#include "PlayerStateInfo.h"

class Player;

class PlayerTransform
{
    friend class Player;
    friend class PlayerAbilities;

    Player* p;
    OgreNewt::BallAndSocket* joint;
    Ogre::Entity* ball;

    void move_callback(OgreNewt::Body* me, float timeStep, int threadIndex);

public:

    PlayerTransform(Player* player);
    ~PlayerTransform();

    void update(float tslf);

    void pressedKey(const OIS::KeyEvent &arg);
    void releasedKey(const OIS::KeyEvent &arg);
    void movedMouse(const OIS::MouseEvent &e);
};