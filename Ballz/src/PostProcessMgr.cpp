#include "stdafx.h"
#include "PostProcessMgr.h"
#include "GameStateManager.h"

using namespace Ogre;

#include "PPListener.h"

PostProcessMgr::PostProcessMgr(Ogre::Camera* cam) : camera(cam), ppListener(&vars)
{
	auto t = lut.loadTexture("normal.png");
	MaterialPtr mat = Ogre::MaterialManager::getSingleton().getByName("ScaryGlowDelay");
	mat->getTechnique(0)->getPass(0)->getTextureUnitState("lut")->setTexture(t);

	resetValues();
	setGodraySunPositionFar(Vector3(300, 300, 400) * Vector3(400, -300, -400));

	auto compositorName = "ScaryBloomNoSSAO";
	Ogre::CompositorInstance *bloomCompositor = Ogre::CompositorManager::getSingleton().addCompositor(camera->getViewport(), compositorName);
	bloomCompositor->addListener(&ppListener);
	Ogre::CompositorManager::getSingleton().setCompositorEnabled(camera->getViewport(), compositorName, true);

	vars.colourOverlaying = 1;
	totalBlacktime = currentBlacktime = 0;

	fadeIn(Ogre::Vector3(0,0,0),2);
}

PostProcessMgr::~PostProcessMgr()
{
}

void PostProcessMgr::resetValues()
{
	vars.reset();
}

void PostProcessMgr::setGodraySunPosition(Ogre::Vector3 pos)
{
	sunPosition = pos;
	sunDistance = sunPosition.length();
}

void PostProcessMgr::setGodraySunPositionFar(Ogre::Vector3 pos)
{
	sunPosition = pos;
	sunPosition.normalise();
	sunPosition*=100000;
	sunDistance = 100000;
}

void PostProcessMgr::setAutoGodraySunDirection()
{
	auto sun = Global::mSceneMgr->getLight("Sun");
	sunPosition = sun->getDerivedDirection();
	sunPosition.normalise();
	sunPosition *= 100000;
	sunDistance = 100000;
}

void PostProcessMgr::update(float tslf)
{
	Vector3 worldViewPosition = camera->getViewMatrix() * sunPosition;
	Vector3 hcsPosition = camera->getProjectionMatrix() * worldViewPosition;
	Vector2 sunScreenSpacePosition = Vector2(0.5f + (0.5f * hcsPosition.x), 0.5f + (0.5f * -hcsPosition.y));
	vars.SunScreenSpacePosition = Vector4(sunScreenSpacePosition.x, sunScreenSpacePosition.y, 0, 1);
	Ogre::Real par=(camera->getDerivedPosition() - sunPosition).dotProduct(camera->getDerivedDirection())/sunDistance;
	par-=0.15f;

	vars.godrayEdge = Ogre::Math::Clamp(par / 0.65f - 0.25f, 0.0f, 0.75f);

	if (!skipFadeFrame)
		if(totalBlacktime != currentBlacktime)
		{
			if(totalBlacktime < currentBlacktime)
			{
				//out
				totalBlacktime += tslf;
				if(totalBlacktime > currentBlacktime)
					totalBlacktime = currentBlacktime;

				vars.colourOverlaying.w = 1 - totalBlacktime / currentBlacktime;
			}
			else
			{
				//in
				currentBlacktime += tslf;
				if(totalBlacktime < currentBlacktime)
					currentBlacktime = totalBlacktime;

				vars.colourOverlaying.w = currentBlacktime / totalBlacktime;
			}
		}

	skipFadeFrame = false;
}

void PostProcessMgr::fadeOut(Ogre::Vector3 colour, float duration, bool skipFrame)
{
	vars.colourOverlaying = colour;
	vars.colourOverlaying.w = 1;

	totalBlacktime = 0;
	currentBlacktime = duration;
	skipFadeFrame = skipFrame;
}

void PostProcessMgr::fadeIn(Ogre::Vector3 colour, float duration, bool skipFrame)
{
	vars.colourOverlaying = colour;
	vars.colourOverlaying.w = 0;

	totalBlacktime = duration;
	currentBlacktime = 0;
	skipFadeFrame = skipFrame;
}

std::vector<std::string> PostProcessMgr::getColorGradingPresets()
{
	return lut.getAvailableTextures();
}

void PostProcessMgr::setColorGradingPreset(std::string name)
{
	lut.loadTexture(name);
}