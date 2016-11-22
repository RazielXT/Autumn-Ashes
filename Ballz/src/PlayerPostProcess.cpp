#include "stdafx.h"
#include "PlayerPostProcess.h"
#include "Player.h"

PlayerPostProcess::PlayerPostProcess(Player* player) : p(player)
{
	injectPostProcess(&Global::ppMgr->vars);
	cam = Global::camera->cam;
}

void PlayerPostProcess::injectPostProcess(PostProcessVariables* vars)
{
	this->vars = vars;
}

void PlayerPostProcess::update(float tslf)
{
	//visual fall dmg
	if (vars->hurtEffect > 0 && p->alive)
	{
		vars->hurtEffect -= tslf*2.5f;
		if (vars->hurtEffect < 0) vars->hurtEffect = 0;
	}

	float interpolationFactor = mPreviousFPS*0.03f*(vars->mbAmount);

	Ogre::Quaternion estimatedOrientation = Ogre::Quaternion::nlerp(interpolationFactor, cam->getDerivedOrientation(), prevOr);
	Ogre::Vector3    estimatedPosition = (1 - interpolationFactor)*cam->getDerivedPosition() + interpolationFactor*prevPos;
	Ogre::Matrix4 viewMatrix = Ogre::Math::makeViewMatrix(estimatedPosition, estimatedOrientation);
	Ogre::Matrix4 projectionMatrix = cam->getProjectionMatrix();

	vars->ipm = cam->getProjectionMatrix().inverse();
	vars->projm = cam->getProjectionMatrix();// mCamera->getViewMatrix().inverse();// *viewMatrix;

	vars->pvp = projectionMatrix*viewMatrix;
	vars->ivp = (projectionMatrix*cam->getViewMatrix()).inverse();

	mPreviousFPS = 1 / tslf;
	prevPos = cam->getDerivedPosition();
	prevOr = cam->getDerivedOrientation();
}