#include "stdafx.h"
#include "PostProcessSwimming.h"
#include "GameStateManager.h"
#include "Player.h"
#include "..\PlayerSwimming.h"

using namespace Ogre;

PostProcessSwimming::PostProcessSwimming()
{
}

PostProcessSwimming::~PostProcessSwimming()
{
}

void PostProcessSwimming::reset()
{
	if(mWaterCam)
		Global::sceneMgr->destroyCamera(mWaterCam);

	initWaterDepthReading();
}

void PostProcessSwimming::initWaterDepthReading()
{
	auto sceneMgr = Global::sceneMgr;
	auto camera = Global::camera->cam;
	auto window = Global::window;

	texture = TextureManager::getSingleton().createManual("waterDepth",
	          ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, TEX_TYPE_2D,
	          1, 1, 0, PF_FLOAT32_R, TU_RENDERTARGET);

	rttTex = texture->getBuffer()->getRenderTarget();

	mWaterCam = sceneMgr->createCamera("waterDepthCam");
	mWaterCam->setNearClipDistance(camera->getNearClipDistance());
	mWaterCam->setFarClipDistance(camera->getFarClipDistance());
	mWaterCam->setAspectRatio(1);

	mWaterCam->setProjectionType(PT_ORTHOGRAPHIC);
	mWaterCam->setOrthoWindow(1.0f, 1.0f);
	mWaterCam->lookAt(0, 0, -1);
	//mReflectCam->setVisibilityFlags(2);
	Viewport *v = rttTex->addViewport(mWaterCam);
	v->setClearEveryFrame(true);
	v->setBackgroundColour(ColourValue::Black);
	v->setShadowsEnabled(false);
	v->setMaterialScheme("WaterDepth");

	mWaterCamNode = Global::sceneMgr->getRootSceneNode()->createChildSceneNode();
	mWaterCamNode->attachObject(mWaterCam);
	mWaterCamNode->pitch(Degree(-90));

	v->setVisibilityMask(VisibilityFlag_Water);
	v->setSkiesEnabled(false);
}

void PostProcessSwimming::readWaterDepth()
{
	float depth;
	Ogre::PixelBox tempPb(1, 1, 1, Ogre::PF_FLOAT32_R, &depth);

	texture->getBuffer()->blitToMemory(tempPb);

	float playerDepth = Global::camera->position.y - 0.2f;

	if (depth == 0 || playerDepth > depth)
	{
		if (inWater)
		{
			leftWater();
			inWater = false;
		}
	}
	else
	{
		if (!inWater)
		{
			enteredWater();
			inWater = true;
		}
	}

	mWaterCamNode->setPosition(Global::camera->position + Vector3(0, 20, 0));
}


void PostProcessSwimming::enteredWater()
{
	lastPPValues.fogColor = Global::sceneMgr->getFogColour();
	lastPPValues.fogStart = Global::sceneMgr->getFogStart();
	lastPPValues.fogEnd = Global::sceneMgr->getFogEnd();

	Global::sceneMgr->setFog(FOG_LINEAR, Ogre::ColourValue(0.4, 0.4, 0.5, 0.85f), 1, 5, 25);
}

void PostProcessSwimming::leftWater()
{
	Global::sceneMgr->setFog(FOG_LINEAR, lastPPValues.fogColor, 1, lastPPValues.fogStart, lastPPValues.fogEnd);
}

void PostProcessSwimming::update(float tslf)
{
	readWaterDepth();

	if (!inWater)
	{
		outOfWaterTimer += tslf;
	}
	else
		outOfWaterTimer = 0;


	float outOfWaterEffect = std::min(outOfWaterTimer / 2.0f, 1.0f);
	Global::ppMgr->vars.ppDistortionIgnore = outOfWaterEffect;
	Global::ppMgr->vars.ColouringShift.w = 1 - outOfWaterEffect;
}
