#include "stdafx.h"
#include "CameraShaker.h"
#include "MathUtils.h"

using namespace Ogre;

Quaternion CameraShaker::update(float time)
{
    if (!camShaking)
        return Quaternion::IDENTITY;

    camShakeTimer = std::min(time + camShakeTimer, camShakeTimerEnd);
    Ogre::Quaternion shakeOr = Ogre::Quaternion::Slerp(camShakeTimer / camShakeTimerEnd, camShakePrev, camShakeTarget);

    //last target duration passed, end shaking
    if (camShakeTimer == camShakeTimerEnd && camShakeTimeLeft<=0)
    {
        camShaking = false;
        return Ogre::Quaternion::IDENTITY;
    }
    //time for next target
    else if (camShakeTimer >= camShakeTimerEnd)
    {
        camShakeTimer = 0;

        float timerWeight = MathUtils::lerp(initPwrW, endPwrW, camShakeTimeLeft / shakingDuration);
        float shakeLen = timerWeight * std::max(shakeSizeX, shakeSizeY) / 5.0f;

        camShakeTimerEnd = shakeLen / shakeFreq;
        camShakeTimeLeft -= camShakeTimerEnd;

        //time ended, last target
        if (camShakeTimeLeft <= 0)
        {
            camShakeTimeLeft = 0;
            camShakePrev = camShakeTarget;
            camShakeTarget = Ogre::Quaternion::IDENTITY;
        }
        else
        {
            float camShakeDirectionX = Ogre::Math::RangeRandom(0.1f, 0.45f)*shakeSizeX;
            float camShakeDirectionY = Ogre::Math::RangeRandom(0.25f, 0.6f)*shakeSizeY;
            float camShakeDirectionZ = Ogre::Math::RangeRandom(0.1f, 0.4f)*shakeSizeX*shakeSizeY;

            if (Ogre::Math::RangeRandom(-1, 1) < 0)
                camShakeDirectionY *= -1;

            makeNextTarget(camShakeDirectionX, camShakeDirectionY, camShakeDirectionZ, timerWeight);
        }
    }

    return shakeOr;
}

void CameraShaker::makeNextTarget(float targetX, float targetY, float targetZ, float timerWeight)
{
    if (nextRight)
        targetX *= -1;

    nextRight = !nextRight;

    float random = Ogre::Math::RangeRandom(0.5, 1.0f) * angleShaking * timerWeight;

    camShakeTimer = 0;
    camShakeTimerEnd = std::max(shakeSizeX, shakeSizeY) / 5.0f;

    camShakePrev = camShakeTarget;
    camShakeTarget.FromAngleAxis(random*Ogre::Degree(Ogre::Math::Clamp(targetX * 50, -50.0f, 50.0f)), Ogre::Vector3(targetX, targetY, targetZ));
}

void CameraShaker::startShaking(float power, float freq, float duration, float angleShake, float initPowerW, float endPowerW, float wX, float wY)
{
    permanent = (duration == 0);

    camShakeTimeLeft = shakingDuration = duration;
    shakeSizeX = Ogre::Math::Clamp(power * wX, 0.01f, 2.0f);
    shakeSizeY = Ogre::Math::Clamp(power * wY, 0.01f, 2.0f);
    shakeFreq = Ogre::Math::Clamp(freq, 0.1f, 5.0f);
    angleShaking = angleShake;

    initPwrW = Ogre::Math::Clamp(initPowerW, 0.05f, 2.0f);
    endPwrW = Ogre::Math::Clamp(endPowerW, 0.05f, 2.0f);

    if (!camShaking)
    {
        camShakePrev = Ogre::Quaternion::IDENTITY;
    }

    float random = Ogre::Math::RangeRandom(-1.0f, 1.0f);
    float random2 = Ogre::Math::RangeRandom(-1.0f, 1.0f);

    makeNextTarget(shakeSizeX*random, -shakeSizeY, shakeSizeX*random2, initPwrW);
    camShaking = true;
}

void CameraShaker::stopShaking()
{
    camShaking = false;
}