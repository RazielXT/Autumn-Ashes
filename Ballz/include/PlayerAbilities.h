#pragma once

#include "stdafx.h"
#include "PlayerTimeshift.h"
#include "PlayerEnergies.h"

class PlayerFlash
{
    Player* p;
    PlayerEnergies* pEnergies;

    Ogre::SceneNode* cameraPortNode;
    float portingTimer = -1;
    Ogre::Vector3 portTargetPos;
    Ogre::Vector3 portStartPos;

public:

    void update(float tslf);

    PlayerFlash(Player* player, PlayerEnergies* energies);

    void portForward();
};

class PlayerAbilities
{
    friend class Player;

    Player* p;

    PlayerEnergies pEnergies;
    PlayerTimeshift timeshift;
    PlayerFlash flash;

public:

    PlayerAbilities(Player* player);
    ~PlayerAbilities();

    void update(float tslf);

    bool pressedKey(const OIS::KeyEvent &arg);
    bool releasedKey(const OIS::KeyEvent &arg);

    bool pressedMouse(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
    bool releasedMouse(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
};