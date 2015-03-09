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
    id = 1;

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


SwitchColorSchemeFx::SwitchColorSchemeFx(Ogre::String info)
{
    const char* text = info.c_str();
    char* token;
    char* temp = strtok_s(const_cast<char *> (text), ",", &token);

    colorTarget.x = Ogre::StringConverter::parseReal(temp);
    temp = strtok_s(NULL, ",", &token);

    colorTarget.y = Ogre::StringConverter::parseReal(temp);
    temp = strtok_s(NULL, ",", &token);

    colorTarget.z = Ogre::StringConverter::parseReal(temp);
    temp = strtok_s(NULL, ",", &token);

    fulltime = Ogre::StringConverter::parseReal(temp);
}

bool SwitchColorSchemeFx::start()
{
    timer = 0;
    colorBase.x = Global::mPPMgr->ColouringShift.x;
    colorBase.y = Global::mPPMgr->ColouringShift.y;
    colorBase.z = Global::mPPMgr->ColouringShift.z;

    return true;
}

bool SwitchColorSchemeFx::update(float tslf)
{
    timer = std::max(fulltime, timer + tslf*Global::timestep);

    auto w = 1 - timer / fulltime;
    Global::mPPMgr->ColouringShift = colorBase*w + colorTarget*(1-w);

    if (timer == 1)
        return false;

    return true;
}