#pragma once

#include "stdafx.h"

class Player;
class WaterCurrent;

class PostProcessSwimming
{
	WaterCurrent* currents;

	Ogre::RenderTarget *rttTex;
	Ogre::TexturePtr texture;
	Ogre::Camera* mWaterCam = nullptr;
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
	struct
	{
		Ogre::Vector4 ColorShift;
		Ogre::ColourValue fogColor;
		float fogStart, fogEnd;
	} lastPPValues;

public:

	float outOfWaterTimer = 1.0f;

	bool inWater = false;
	PostProcessSwimming();
	~PostProcessSwimming();

	void reset();
	void update(float tslf);
};