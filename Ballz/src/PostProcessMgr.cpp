#include "stdafx.h"
#include "PostProcessMgr.h"

using namespace Ogre;

#include "BasicPPListener.h"
#include "ScaryPPListener.h"

PostProcessMgr::PostProcessMgr(Ogre::Camera* cam)
{
    basicPP = true;
    scaryPP = false;
    advancedPP = false;
    ssaoEnabled = Global::gameMgr->gameConfig.ssao;
    dirty = false;

    ivp=Ogre::Matrix4::IDENTITY;
    pvp=Ogre::Matrix4::IDENTITY;
    hurtEffect=0;
    godrayEdge=0.2;
    setGodraySunPositionFar(Vector3(300, 300, 400) * Vector3(400, -300, -400));
    colourOverlaying=1;
    ContSatuSharpNoise=0;
    radialHorizBlurVignette=0;
    bloomStrDep=Ogre::Vector4(1,1,0,0);
    mbAmount=1;
    ColouringShift=Ogre::Vector4(1,1,1,0);

    camera = cam;

    basicList = new BasicPostProcessListener(&SunScreenSpacePosition,&ivp,&pvp,&hurtEffect,&godrayEdge,&colourOverlaying);
    scaryList = new AaPostProcessListener(&SunScreenSpacePosition,&ivp,&pvp,&hurtEffect,&godrayEdge,&colourOverlaying,&ContSatuSharpNoise,&radialHorizBlurVignette, &ColouringShift, &bloomStrDep);

    setToScaryBloom();

    totalBlacktime = currentBlacktime = 0;

    fadeIn(Ogre::Vector3(0,0,0),2);
}

PostProcessMgr::~PostProcessMgr()
{
    delete basicList;
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


void PostProcessMgr::update(float tslf)
{
    /*

    Vector3 sunPosition = Vector3(50000, -30000, -40000);
    Vector3 worldViewPosition = cam->getViewMatrix() * sunPosition;
    Vector3 hcsPosition = cam->getProjectionMatrix() * worldViewPosition;
    Vector2 sunScreenSpacePosition = Vector2(0.5f + (0.5f * hcsPosition.x), 0.5f + (0.5f * -hcsPosition.y));
    SunScreenSpacePosition = Vector4(sunScreenSpacePosition.x, sunScreenSpacePosition.y, 0, 1);
    Ogre::Real par=(cam->getDerivedPosition() - (Vector3(300, 300, 400) * Vector3(-400, 300, 400))).dotProduct(cam->getDerivedDirection());

    if (par > -50000)
    	godrayEdge=0.0;
    else
    	godrayEdge=(-(par/1000000))*3.5f;
    */

    dirty = false;

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

            dirty = true;
        }

    skipFadeFrame = false;
}

void PostProcessMgr::setToBasicBloom()
{
    basicPP = true;
    scaryPP = false;
    advancedPP = false;

    Ogre::String ssaoName = "";
    if(!ssaoEnabled)
    {
        ssaoName = "NoSSAO";
    }

    Ogre::CompositorManager::getSingleton().removeCompositor(camera->getViewport(), currentCompositor+ssaoName);
    currentCompositor = "BasicBloom";
    Ogre::CompositorInstance *bloomCompositor = Ogre::CompositorManager::getSingleton().addCompositor(camera->getViewport(), currentCompositor+ssaoName);
    bloomCompositor->addListener(basicList);
    Ogre::CompositorManager::getSingleton().setCompositorEnabled(camera->getViewport(), currentCompositor+ssaoName, true);
}

void PostProcessMgr::setToScaryBloom()
{
    basicPP = false;
    scaryPP = true;
    advancedPP = false;

    Ogre::String ssaoName = "";
    if(!ssaoEnabled)
    {
        ssaoName = "NoSSAO";
    }

    Ogre::CompositorManager::getSingleton().removeCompositor(camera->getViewport(), currentCompositor+ssaoName);
    currentCompositor = "ScaryBloom";
    Ogre::CompositorInstance *bloomCompositor = Ogre::CompositorManager::getSingleton().addCompositor(camera->getViewport(), currentCompositor+ssaoName);
    bloomCompositor->addListener(scaryList);
    Ogre::CompositorManager::getSingleton().setCompositorEnabled(camera->getViewport(), currentCompositor+ssaoName, true);
}

void PostProcessMgr::setToAdvancedBloom()
{
}

void PostProcessMgr::setSSAO(bool enabled)
{
    ssaoEnabled = enabled;

    if(enabled)
    {
        Ogre::CompositorManager::getSingleton().removeCompositor(camera->getViewport(), currentCompositor + "NoSSAO");
        Ogre::CompositorInstance *bloomCompositor = Ogre::CompositorManager::getSingleton().addCompositor(camera->getViewport(), currentCompositor);
        if(basicPP)
            bloomCompositor->addListener(basicList);
        if(scaryPP)
            bloomCompositor->addListener(scaryList);
        Ogre::CompositorManager::getSingleton().setCompositorEnabled(camera->getViewport(), currentCompositor, true);
    }
    else
    {
        Ogre::CompositorManager::getSingleton().removeCompositor(camera->getViewport(), currentCompositor);
        Ogre::CompositorInstance *bloomCompositor = Ogre::CompositorManager::getSingleton().addCompositor(camera->getViewport(), currentCompositor+"NoSSAO");
        if(basicPP)
            bloomCompositor->addListener(basicList);
        if(scaryPP)
            bloomCompositor->addListener(scaryList);
        Ogre::CompositorManager::getSingleton().setCompositorEnabled(camera->getViewport(), currentCompositor+"NoSSAO", true);
    }
}

void PostProcessMgr::fadeOut(Ogre::Vector3 colour, float duration, bool skipFrame)
{
    dirty = true;
    colourOverlaying = colour;
    colourOverlaying.w = 1;

    totalBlacktime = 0;
    currentBlacktime = duration;
    skipFadeFrame = skipFrame;
}

void PostProcessMgr::fadeIn(Ogre::Vector3 colour, float duration, bool skipFrame)
{
    dirty = true;
    colourOverlaying = colour;
    colourOverlaying.w = 0;

    totalBlacktime = duration;
    currentBlacktime = 0;
    skipFadeFrame = skipFrame;
}