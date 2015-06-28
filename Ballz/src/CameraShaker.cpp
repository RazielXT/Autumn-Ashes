#include "stdafx.h"
#include "CameraShaker.h"
#include "MUtils.h"

using namespace Ogre;

void CameraShaker::update(float time)
{
    if (!camShaking)
        return;

    camShakeTimer = std::min(time + camShakeTimer, camShakeTimerEnd);
    current = Ogre::Quaternion::Slerp(camShakeTimer / camShakeTimerEnd, camShakePrev, camShakeTarget);

    //last target duration passed, end shaking
    if (!permanent && camShakeTimer == camShakeTimerEnd && camShakeTimeLeft<0)
    {
        camShaking = false;
        current = Quaternion::IDENTITY;
    }
    //time for next target
    else if (camShakeTimer >= camShakeTimerEnd)
    {
        makeNextTarget();

        //time ended, last target
        if (!permanent && camShakeTimeLeft <= 0)
        {
            camShakeTimeLeft = 0;
            //camShakePrev = camShakeTarget;
            camShakeTarget = Ogre::Quaternion::IDENTITY;
        }
    }

}

void CameraShaker::makeNextTarget(bool first)
{
    float timerWeight = permanent ? initPwrW : MUtils::lerp(initPwrW, endPwrW, 1.0f - camShakeTimeLeft / shakingDuration);

    float targetX = timerWeight * Ogre::Math::RangeRandom(0.5f, 1.0f) *shakeSizeX;
    float targetY = timerWeight * Ogre::Math::RangeRandom(0.5f, 1.0f) *shakeSizeY;
    float targetZ = targetX*angleShaking;

    if (nextUp && (shakeSizeX<shakeSizeY || first))
        targetY *= -1;

    if (nextRight && (shakeSizeX>=shakeSizeY || first))
        targetX *= -1;

    nextUp = !nextUp;
    nextRight = !nextRight;

    float random = Ogre::Math::RangeRandom(0.5, 1.0f) * timerWeight;

    if (first && camShaking)
        camShakePrev = current;
    else
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
    shakeFreq = Ogre::Math::Clamp(freq, 0.01f, 5.0f);
    angleShaking = angleShake;

    initPwrW = Ogre::Math::Clamp(initPowerW, 0.05f, 2.0f);
    endPwrW = Ogre::Math::Clamp(endPowerW, 0.05f, 2.0f);

    if (camShaking)
        nextUp = true;

    if (!camShaking)
    {
        nextUp = nextRight = Ogre::Math::RangeRandom(-1.0, 1.0f) < 0;
        camShakePrev = camShakeTarget = Ogre::Quaternion::IDENTITY;
    }

    if (forceDominantSideDownFirst)
    {
        if (shakeSizeX < shakeSizeY)
            nextUp = true;

        if (shakeSizeX >= shakeSizeY)
            nextRight = true;
    }

    makeNextTarget(true);

    if (camShakeTimeLeft < 0)
        camShakeTimeLeft = 0;

    camShaking = true;
}

void CameraShaker::stopShaking()
{
    camShaking = false;
}