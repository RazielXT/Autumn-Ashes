#pragma once

#include "stdafx.h"
#include "PlayerStateInfo.h"

class Player;

class PlayerAbilities
{
    friend class Player;

    bool shiftPressed = false;

    Player* p;
    float stateTimer = 0;
    float lastTimestamp = 0;
    float shiftTimeTarget = 0;
    std::list<PlayerStateInfo> playerHistory;

    void updateStateHistory(float tslf);

    Ogre::SceneNode* cameraPortNode;
    float portingTimer = -1;
    Ogre::Vector3 portTargetPos;
    Ogre::Vector3 portStartPos;
    void updatePortTarget(float tslf);

public:

    PlayerAbilities(Player* player);
    ~PlayerAbilities();

    void portForward();

    void shiftBack();
    void setShiftTime();

    void update(float tslf);

    void pressedKey(const OIS::KeyEvent &arg);
    void releasedKey(const OIS::KeyEvent &arg);
    void hidePortTarget();
    void showPortTarget();
};