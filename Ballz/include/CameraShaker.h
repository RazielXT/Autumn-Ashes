#pragma once
#include "stdafx.h"

class CameraShaker
{
public:

    Ogre::Quaternion update(float time);

    //0 duration = permanent
    void startShaking(float power, float freq, float duration = 5.0f, float angleShake = 0.5f, float initPowerW = 1.0f, float endPowerW = 1.0f, float wX = 1.0f, float wY = 1.0f);

    void stopShaking();

private:

    void makeNextTarget(float targetX, float targetY, float targetZ, float timerWeight);

    bool camShaking = false;
    bool permanent = false;
    bool nextRight = false;

    float initPwrW, endPwrW;
    float shakeSizeX, shakeSizeY, shakeFreq;
    float angleShaking;

    float camShakeTimer, camShakeTimerEnd;
    float camShakeTimeLeft, shakingDuration;

    Ogre::Quaternion camShakePrev, camShakeTarget;
};