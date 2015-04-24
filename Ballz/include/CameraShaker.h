#pragma once
#include "stdafx.h"

class CameraShaker
{
public:

    void update(float time);

    //0 duration = permanent
    void startShaking(float power, float freq, float duration = 5.0f, float angleShake = 0.5f, float initPowerW = 1.0f, float endPowerW = 1.0f, float wX = 1.0f, float wY = 1.0f, bool forceDominantSideDownFirst = false);

    void stopShaking();

    Ogre::Quaternion current;

private:

    void makeNextTarget(bool first = false);

    bool camShaking = false;
    bool permanent = false;
    bool nextRight = false;
    bool nextUp = false;

    float initPwrW, endPwrW;
    float shakeSizeX, shakeSizeY, shakeFreq;
    float angleShaking;

    float camShakeTimer, camShakeTimerEnd;
    float camShakeTimeLeft, shakingDuration;

    Ogre::Quaternion camShakePrev, camShakeTarget;
};