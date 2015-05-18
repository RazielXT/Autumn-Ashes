#ifndef PPMGR
#define PPMGR

#include "stdafx.h"

class BasicPostProcessListener;
class AaPostProcessListener;

class PostProcessMgr
{
public:

    PostProcessMgr(Ogre::Camera* camera);
    ~PostProcessMgr();

    void update(float tslf);
    void resetValues();

    void fadeOut(Ogre::Vector3 targetColor, float duration, bool skipFrame = false);
    void fadeIn(Ogre::Vector3 startColor, float duration, bool skipFrame = false);

    void setToBasicBloom();
    void setToScaryBloom();
    void setToAdvancedBloom();
    void setSSAO(bool enabled);

    void setGodraySunPosition(Ogre::Vector3 pos);
    void setGodraySunPositionFar(Ogre::Vector3 pos);
    void setAutoGodraySunDirection();

    Ogre::Matrix4 ivp;
    Ogre::Matrix4 pvp;
    Ogre::Real hurtEffect;
    Ogre::Real godrayEdge;
    Ogre::Vector3 radialHorizBlurVignette;
    Ogre::Vector4 ContSatuSharpNoise,ColouringShift, bloomStrDep;
    Ogre::Real mbAmount;

    Ogre::Camera* camera;

private:

    bool skipFadeFrame = false;
    bool dirty;
    float totalBlacktime,currentBlacktime;
    bool basicPP,scaryPP,advancedPP,ssaoEnabled;

    BasicPostProcessListener* basicList;
    AaPostProcessListener* scaryList;
    Ogre::Vector4 colourOverlaying;

    Ogre::String currentCompositor;
    Ogre::Vector3 sunPosition;
    float sunDistance;
    Ogre::Vector4 SunScreenSpacePosition;
};

#endif