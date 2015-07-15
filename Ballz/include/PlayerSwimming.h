#pragma once

#include "stdafx.h"

class Player;
class WaterCurrent;

class PlayerSwimming
{
    friend class Player;

    Player* p;
	WaterCurrent* currents;

    Ogre::RenderTarget *rttTex;
    Ogre::TexturePtr texture;
    Ogre::Camera* mWaterCam;
    Ogre::SceneNode* mWaterCamNode;

    void initWaterDepthReading();
    void readWaterDepth();

    Ogre::ParticleSystem* bubbles;
    Ogre::SceneNode* bubblesNode;

    Ogre::ParticleSystem* splash;
    Ogre::SceneNode* splashNode;

    Ogre::ParticleSystem* dust;
    Ogre::SceneNode* dustNode;

    void enteredWater();
    void leftWater();
	float outOfWaterTimer;

public:

	bool inWater = false;
    PlayerSwimming(Player* player);

	void update(float tslf);
};