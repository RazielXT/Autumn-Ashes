#include "stdafx.h"
#include "CameraShaker.h"

void CameraShaker::update(float time)
{
    if (!camShaking)
        return;

    camShakeTimer += time;
    Ogre::Quaternion shakeOr = Ogre::Quaternion::Slerp(camShakeTimer / camShakeTimerEnd, camShakePrev, camShakeTarget);

    node->setOrientation(shakeOr);

    if (camShakeTimer >= camShakeTimerEnd)
    {
        if (!camShakeTimeLeft)
        {
            node->resetOrientation();
            camShaking = false;
            return;
        }

        float timerVar = Ogre::Math::Clamp(camShakeTimeLeft / 4.0f + 0.5f, 0.5f, 1.0f);
        camShakeTimer = 0;
        camShakeTimerEnd = Ogre::Math::Clamp<float>((shakeSizeX*(0.5f + shakeFreq) * 50) / (timerVar + 1), 0.05f, shakeFreq);
        camShakeTimeLeft -= camShakeTimerEnd;

        if (camShakeTimeLeft <= 0)
        {
            camShakeTimeLeft = 0;
            camShakePrev = camShakeTarget;
            camShakeTarget = Ogre::Quaternion::IDENTITY;
        }
        else
        {
            float camShakeDirectionX = Ogre::Math::RangeRandom(0.1f, 0.45f);
            float camShakeDirectionY = Ogre::Math::RangeRandom(0.25f, 0.6f);
            float camShakeDirectionZ = Ogre::Math::RangeRandom(0.1f, 0.4f)*shakeSizeX;

            if (camShakeTarget.getPitch().valueRadians() > 0)
            {
                camShakeDirectionX *= -1;
                camShakeDirectionZ *= -1;
            }
            if (Ogre::Math::RangeRandom(-1, 1) < 0)
                camShakeDirectionY *= -1;

            float random = Ogre::Math::RangeRandom(0.5, 1);

            camShakePrev = camShakeTarget;
            camShakeTarget.FromAngleAxis(random*Ogre::Degree(std::min<float>(shakeSizeX*timerVar * 50, 65)), Ogre::Vector3(camShakeDirectionX, camShakeDirectionY, camShakeDirectionZ));
        }
    }
}

void CameraShaker::startShaking(Ogre::SceneNode* node, float duration, float sizeX, float sizeY, float freq)
{
    permanent = duration == 0;
    this->node = node;
    camShakeTimeLeft = duration;
    shakeSizeX = Ogre::Math::Clamp(sizeX, 0.01f, 1.0f);
    shakeSizeY = Ogre::Math::Clamp(sizeY, 0.01f, 1.0f);
    shakeFreq = Ogre::Math::Clamp(freq, 0.01f, 1.0f);
    camShakeTimer = 0;
    camShakeTimerEnd = 0.01;

    if (!camShaking)
    {
        camShakePrev = Ogre::Quaternion::IDENTITY;
        camShakeTarget = Ogre::Quaternion::IDENTITY;
    }
}

void CameraShaker::stopShaking()
{
    camShaking = false;
}