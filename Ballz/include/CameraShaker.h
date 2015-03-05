#pragma once
#include "stdafx.h"

class CameraShaker
{
public:

    void update(float time);

    //0 duration = permanent
    void startShaking(Ogre::SceneNode* node, float duration, float sizeX, float sizeY, float freq);

    void stopShaking();

private:

    bool camShaking = false;
    bool permanent = false;
    Ogre::SceneNode* node;
    float camShakeTimer, camShakeTimerEnd, shakeSizeX, shakeSizeY, shakeFreq, camShakeTimeLeft;
    Ogre::Quaternion camShakePrev, camShakeTarget;
};