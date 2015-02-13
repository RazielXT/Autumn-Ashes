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

    void colourIn(Ogre::Vector3 colour, float duration);
    void colourOut(Ogre::Vector3 colour, float duration);

    void setToBasicBloom();
    void setToScaryBloom();
    void setToAdvancedBloom();
    void setSSAO(bool enabled);

    void setGodraySunPosition(Ogre::Vector3 pos);
    void setGodraySunPositionFar(Ogre::Vector3 pos);

    Ogre::Matrix4 ivp;
    Ogre::Matrix4 pvp;
    Ogre::Real hurtEffect;
    Ogre::Real godrayEdge;
    Ogre::Vector3 radialHorizBlurVignette;
    Ogre::Vector4 ContSatuSharpNoise,ColouringShift, bloomStrDep;
    Ogre::Real mbAmount;

private:

    bool dirty;
    float totalBlacktime,currentBlacktime;
    bool basicPP,scaryPP,advancedPP,ssaoEnabled;
    Ogre::Camera* cam;
    BasicPostProcessListener* basicList;
    AaPostProcessListener* scaryList;
    Ogre::Vector4 colourOverlaying;

    Ogre::String currentCompositor;
    Ogre::Vector3 sunPosition;
    float sunDistance;
    Ogre::Vector4 SunScreenSpacePosition;
};

#endif