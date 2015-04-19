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
        makeNextTarget();

        //time ended, last target
        if (camShakeTimeLeft <= 0)
        {
            camShakeTimeLeft = 0;
            //camShakePrev = camShakeTarget;
            camShakeTarget = Ogre::Quaternion::IDENTITY;
        }
    }

    return shakeOr;
}

void CameraShaker::makeNextTarget()
{
    float timerWeight = MathUtils::lerp(initPwrW, endPwrW, 1.0f - camShakeTimeLeft / shakingDuration);

    float targetX = timerWeight * Ogre::Math::RangeRandom(0.5f, 1.0f) *shakeSizeX;
    float targetY = timerWeight * Ogre::Math::RangeRandom(0.5f, 1.0f) *shakeSizeY;
    float targetZ = targetX*angleShaking;

    if (nextUp && shakeSizeX<shakeSizeY)
        targetY *= -1;

    if (nextRight && shakeSizeX>=shakeSizeY)
        targetX *= -1;

    nextUp = !nextUp;
    nextRight = !nextRight;

    float random = Ogre::Math::RangeRandom(0.5, 1.0f) * timerWeight;

    camShakePrev = camShakeTarget;

    camShakeTarget.FromAngleAxis(random*Ogre::Degree(Ogre::Math::Clamp(targetX * 50, -50.0f, 50.0f)), Ogre::Vector3(0, abs(targetX), targetZ));
    Quaternion camTemp = camShakeTarget;
    camShakeTarget.FromAngleAxis(random*Ogre::Degree(Ogre::Math::Clamp(targetY * 50, -50.0f, 50.0f)), Ogre::Vector3(abs(targetY), 0, 0));
    camShakeTarget = camShakeTarget*camTemp;

    float shakeLen = std::max(shakeSizeX, shakeSizeY)/5.0f;

    camShakeTimer = 0;
    camShakeTimerEnd = shakeLen / shakeFreq;

    camShakeTimeLeft -= camShakeTimerEnd;
}

void CameraShaker::startShaking(float power, float freq, float duration, float angleShake, float initPowerW, float endPowerW, float wX, float wY, bool forceDominantSideDownFirst)
{
    permanent = (duration == 0);

    camShakeTimeLeft = shakingDuration = duration;
    shakeSizeX = Ogre::Math::Clamp(power * wX, 0.01f, 2.0f);
    shakeSizeY = Ogre::Math::Clamp(power * wY, 0.01f, 2.0f);
    shakeFreq = Ogre::Math::Clamp(freq, 0.1f, 5.0f);
    angleShaking = angleShake;

    initPwrW = Ogre::Math::Clamp(initPowerW, 0.05f, 2.0f);
    endPwrW = Ogre::Math::Clamp(endPowerW, 0.05f, 2.0f);

    if (forceDominantSideDownFirst)
    {
        if (shakeSizeX < shakeSizeY)
            nextUp = true;

        if (shakeSizeX >= shakeSizeY)
            nextRight = true;
    }

    if (!camShaking)
    {
        camShakePrev = Ogre::Quaternion::IDENTITY;
    }

    makeNextTarget();

    camShaking = true;
}

void CameraShaker::stopShaking()
{
    camShaking = false;
}