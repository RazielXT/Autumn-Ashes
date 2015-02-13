#include "stdafx.h"
#include "EffectsTasks.h"

QuickScaryBlink::QuickScaryBlink()
{
    timer = 0;
    totalTime = 0.55;
    effectStr = 1;
    doneBlink = false;
}

bool QuickScaryBlink::start()
{
    if(timer>0)
        return false;

    doneBlink = false;
    mgr = (Global::mPPMgr);

    std::string soundName;
    if(id==1)
    {
        soundName = AudioLibrary::getPath("staticBlinkRip.wav");
        effectStr = 0.9;
        totalTime = 0.45f;
    }
    else if(id==2)
    {
		soundName = AudioLibrary::getPath("staticBlinkShort.wav");
        effectStr = 0.7;
        totalTime = 0.4f;
    }
    else if(id==3)
    {
		soundName = AudioLibrary::getPath("staticBlinkLong.wav");
        effectStr = 0.5;
        totalTime = 0.85f;
    }
    else
    {
		soundName = AudioLibrary::getPath("staticBlink.wav");
        effectStr = 1;
        totalTime = 0.55f;
    }

    Global::soundEngine->play2D(soundName.c_str(),false , false, false, irrklang::ESM_AUTO_DETECT, false);

    return true;
}

bool QuickScaryBlink::update(Ogre::Real tslf)
{
    timer+=tslf;

    float val = std::max(0.15f,std::min(timer*2.0f,totalTime*2-timer*2.0f));

    mgr->radialHorizBlurVignette.y = val*3*effectStr;
    mgr->ContSatuSharpNoise.w = val*5*effectStr;
    mgr->ContSatuSharpNoise.z = val*4*effectStr;


    if(timer>totalTime)
    {
        timer = 0;
        mgr->radialHorizBlurVignette.y = 0;
        mgr->ContSatuSharpNoise.w = 0;
        mgr->ContSatuSharpNoise.z = 0;
        return false;
    }

    return true;
}