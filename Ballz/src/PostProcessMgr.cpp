#include "stdafx.h"
#include "PostProcessMgr.h"
#include "GameStateManager.h"

using namespace Ogre;

#include "BasicPPListener.h"
#include "ScaryPPListener.h"

PostProcessMgr::PostProcessMgr(Ogre::Camera* cam)
{
	resetValues();
	setGodraySunPositionFar(Vector3(300, 300, 400) * Vector3(400, -300, -400));

    camera = cam;

    scaryList = new AaPostProcessListener(&SunScreenSpacePosition,&ivp,&pvp,&hurtEffect,&godrayEdge,&colourOverlaying,&ContSatuSharpNoise,&radialHorizBlurVignette, &ColouringShift, &bloomStrDep,&ppDistortionIgnore);

    setToScaryBloom();

	colourOverlaying = 1;
    totalBlacktime = currentBlacktime = 0;

    fadeIn(Ogre::Vector3(0,0,0),2);
}

PostProcessMgr::~PostProcessMgr()
{
	delete scaryList;
}

void PostProcessMgr::resetValues()
{
    ivp=Ogre::Matrix4::IDENTITY;
    pvp=Ogre::Matrix4::IDENTITY;
    hurtEffect=0;
    godrayEdge=0.2;
    ContSatuSharpNoise=0;
    radialHorizBlurVignette=0;
    mbAmount=1;
	ppDistortionIgnore = 1;
    ColouringShift=Ogre::Vector4(1,1,1,0);
    bloomStrDep=Ogre::Vector4(1,1,0,0);
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
    SunScreenSpacePosition = Vector4(sunScreenSpacePosition.x, sunScreenSpacePosition.y, 0, 1);
    Ogre::Real par=(camera->getDerivedPosition() - sunPosition).dotProduct(camera->getDerivedDirection())/sunDistance;
    par-=0.15f;

    godrayEdge = Ogre::Math::Clamp(par/0.65f-0.25f,0.0f,0.75f);

    if (!skipFadeFrame)
        if(totalBlacktime != currentBlacktime)
        {
            if(totalBlacktime < currentBlacktime)
            {
                //out
                totalBlacktime += tslf;
                if(totalBlacktime > currentBlacktime)
                    totalBlacktime = currentBlacktime;

                colourOverlaying.w = 1 - totalBlacktime / currentBlacktime;
            }
            else
            {
                //in
                currentBlacktime += tslf;
                if(totalBlacktime < currentBlacktime)
                    currentBlacktime = totalBlacktime;

                colourOverlaying.w = currentBlacktime / totalBlacktime;
            }
        }

    skipFadeFrame = false;
}

void PostProcessMgr::setToScaryBloom()
{
	scaryPP = true;
	advancedPP = false;

    Ogre::CompositorManager::getSingleton().removeCompositor(camera->getViewport(), currentCompositor);
    currentCompositor = "ScaryBloom";
    Ogre::CompositorInstance *bloomCompositor = Ogre::CompositorManager::getSingleton().addCompositor(camera->getViewport(), currentCompositor);
    bloomCompositor->addListener(scaryList);
    Ogre::CompositorManager::getSingleton().setCompositorEnabled(camera->getViewport(), currentCompositor, true);
}

void PostProcessMgr::setToAdvancedBloom()
{
}

void PostProcessMgr::fadeOut(Ogre::Vector3 colour, float duration, bool skipFrame)
{
    colourOverlaying = colour;
    colourOverlaying.w = 1;

    totalBlacktime = 0;
    currentBlacktime = duration;
    skipFadeFrame = skipFrame;
}

void PostProcessMgr::fadeIn(Ogre::Vector3 colour, float duration, bool skipFrame)
{
    colourOverlaying = colour;
    colourOverlaying.w = 0;

    totalBlacktime = duration;
    currentBlacktime = 0;
    skipFadeFrame = skipFrame;
}