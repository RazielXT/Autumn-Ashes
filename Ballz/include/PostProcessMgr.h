#ifndef PPMGR
#define PPMGR

#include "stdafx.h"

class BasicPostProcessListener;
class AaPostProcessListener;

struct PostProcessVariables
{
    Ogre::Matrix4 ipm;
    Ogre::Matrix4 projm;
    Ogre::Matrix4 ivp;
    Ogre::Matrix4 pvp;
    Ogre::Real hurtEffect;
    Ogre::Real godrayEdge;
    Ogre::Vector3 radialHorizBlurVignette;
    Ogre::Vector4 ContSatuSharpNoise, ColouringShift, bloomStrDepAddSize;
    Ogre::Real mbAmount;
    Ogre::Real ppDistortionIgnore;

    Ogre::Vector4 colourOverlaying;
    Ogre::Vector4 SunScreenSpacePosition;

    void reset()
    {
        ivp = Ogre::Matrix4::IDENTITY;
        pvp = Ogre::Matrix4::IDENTITY;
        ipm = Ogre::Matrix4::IDENTITY;
        hurtEffect = 0;
        godrayEdge = 0.2;
        ContSatuSharpNoise = 0;
        radialHorizBlurVignette = 0;
        mbAmount = 1;
        ppDistortionIgnore = 1;
        ColouringShift = Ogre::Vector4(1, 1, 1, 0);
        bloomStrDepAddSize = Ogre::Vector4(1, 1, 0, 1);
    }
};

class PostProcessMgr
{
public:

    PostProcessMgr(Ogre::Camera* camera);
    ~PostProcessMgr();

    void update(float tslf);
    void resetValues();

    void fadeOut(Ogre::Vector3 targetColor, float duration, bool skipFrame = false);
    void fadeIn(Ogre::Vector3 startColor, float duration, bool skipFrame = false);

    void setToScaryBloom();

    void setGodraySunPosition(Ogre::Vector3 pos);
    void setGodraySunPositionFar(Ogre::Vector3 pos);
    void setAutoGodraySunDirection();

    PostProcessVariables vars;
    Ogre::Camera* camera;

private:

    bool skipFadeFrame = false;
    float totalBlacktime,currentBlacktime;

    AaPostProcessListener* scaryList;

    Ogre::String currentCompositor;
    Ogre::Vector3 sunPosition;
    float sunDistance;

};

#endif