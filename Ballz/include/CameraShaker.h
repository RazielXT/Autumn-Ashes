#pragma once
#include "stdafx.h"

class CameraShaker
{
public:

    Ogre::Quaternion update(float time, float power = 1.0f);

    //0 duration = permanent
    void startShaking(float duration, float initX, float initY, float powerX, float powerY, float freq);

    void stopShaking();

private:

    void makeNextTarget(float targetX, float targetY, float targetZ, float timerWeight);

    bool camShaking = false;
    bool permanent = false;
    float camShakeTimer, camShakeTimerEnd, shakeSizeX, shakeSizeY, shakeFreq, camShakeTimeLeft;
    Ogre::Quaternion camShakePrev, camShakeTarget;
};