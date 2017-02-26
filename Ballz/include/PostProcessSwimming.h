#pragma once

#include "stdafx.h"

class Player;

class PostProcessSwimming
{
	Ogre::RenderTarget *rttTex;
	Ogre::TexturePtr texture;
	Ogre::Camera* mWaterCam = nullptr;
	Ogre::SceneNode* mWaterCamNode;

	void initWaterDepthReading();
	void readWaterDepth();

	void enteredWater();
	void leftWater();
	struct
	{
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