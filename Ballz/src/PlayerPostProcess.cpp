#include "stdafx.h"
#include "PlayerPostProcess.h"
#include "Player.h"

PlayerPostProcess::PlayerPostProcess(Player* player) : p(player)
{
}

void PlayerPostProcess::update(float tslf)
{
	//visual fall dmg
	if (*ppFall > 0 && p->alive)
	{
		*ppFall -= tslf*2.5f;
		if (*ppFall < 0) *ppFall = 0;
	}

	auto mCamera = p->mCamera;

	float interpolationFactor = mPreviousFPS*0.03f*(*ppMotionBlur);

	Ogre::Quaternion estimatedOrientation = Ogre::Quaternion::nlerp(interpolationFactor, mCamera->getDerivedOrientation(), prevOr);
	Ogre::Vector3    estimatedPosition = (1 - interpolationFactor)*mCamera->getDerivedPosition() + interpolationFactor*prevPos;
	Ogre::Matrix4 viewMatrix = Ogre::Math::makeViewMatrix(estimatedPosition, estimatedOrientation);
	Ogre::Matrix4 projectionMatrix = mCamera->getProjectionMatrix();
	*pVP = projectionMatrix*viewMatrix;
	*iVP = (projectionMatrix*mCamera->getViewMatrix()).inverse();

	mPreviousFPS = 1 / tslf;
	prevPos = mCamera->getDerivedPosition();
	prevOr = mCamera->getDerivedOrientation();
}