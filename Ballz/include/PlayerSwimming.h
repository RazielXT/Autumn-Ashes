#pragma once

#include "stdafx.h"

class Player;

class PlayerSwimming
{
    friend class Player;

    Player* p;
    Ogre::RenderTarget *rttTex;
    Ogre::TexturePtr texture;
    Ogre::Camera* mWaterCam;
    Ogre::SceneNode* mWaterCamNode;

    void readWaterDepth();

public:

    PlayerSwimming(Player* player);

    void update();
};