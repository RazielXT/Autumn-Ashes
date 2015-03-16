#include "stdafx.h"
#include "CameraShaker.h"

using namespace Ogre;

Quaternion CameraShaker::update(float time, float power)
{
    if (!camShaking)
		return Quaternion::IDENTITY;

    camShakeTimer = std::min(time + camShakeTimer, camShakeTimerEnd);
    Ogre::Quaternion shakeOr = Ogre::Quaternion::Slerp(camShakeTimer / camShakeTimerEnd, camShakePrev, camShakeTarget);

    if (camShakeTimer >= camShakeTimerEnd)
    {
        if (!camShakeTimeLeft)
        {
            camShaking = false;
        }
		else
		{
			camShakeTimer = 0;
			auto shakeLen = std::max(shakeSizeX, shakeSizeY)/5.0;
			float timerWeight = Ogre::Math::Clamp(camShakeTimeLeft / 4.0f, 0.25f, 1.0f);
			camShakeTimerEnd = shakeLen*timerWeight;
			camShakeTimeLeft -= camShakeTimerEnd;

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

				if (camShakeTarget.getPitch().valueRadians() > 0)
				{
					camShakeDirectionX *= -1;
					camShakeDirectionZ *= -1;
				}
				if (Ogre::Math::RangeRandom(-1, 1) < 0)
					camShakeDirectionY *= -1;

				makeNextTarget(camShakeDirectionX, camShakeDirectionY, camShakeDirectionZ, timerWeight);
			}
		}      
    }

	return shakeOr;
}

void CameraShaker::makeNextTarget(float targetX, float targetY, float targetZ, float timerWeight)
{
	float random = Ogre::Math::RangeRandom(0.5, 1);

	camShakePrev = camShakeTarget;
	camShakeTarget.FromAngleAxis(random*Ogre::Degree(std::min<float>(shakeSizeX*timerWeight * 50, 65)), Ogre::Vector3(targetX, targetY, targetZ));
}

void CameraShaker::startShaking(float duration, float initX, float initY, float powerX, float powerY, float freq)
{
    permanent = duration == 0;

    camShakeTimeLeft = duration;
	shakeSizeX = Ogre::Math::Clamp(powerX, 0.01f, 1.0f);
	shakeSizeY = Ogre::Math::Clamp(powerY, 0.01f, 1.0f);
    shakeFreq = Ogre::Math::Clamp(freq, 0.01f, 1.0f);
    camShakeTimer = 0;
	camShakeTimerEnd = std::max(shakeSizeX, shakeSizeY) / 5.0;

    if (!camShaking)
    {
        camShakePrev = Ogre::Quaternion::IDENTITY;
        
		makeNextTarget(initX, initY, initX, 1);
    }
}

void CameraShaker::stopShaking()
{
    camShaking = false;
}