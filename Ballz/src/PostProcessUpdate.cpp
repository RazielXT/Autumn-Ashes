#include "stdafx.h"
#include "PostProcessUpdate.h"

PostProcessUpdate::PostProcessUpdate()
{
	cam = Global::camera->cam;
	stateExecution = UNDEFINED;
}

void PostProcessUpdate::injectPostProcess(PostProcessVariables* vars)
{
	this->vars = vars;
}

bool PostProcessUpdate::update(float tslf)
{
	//visual fall dmg
	if (vars->hurtEffect > 0)
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

	swimming.update(tslf);

	return true;
}