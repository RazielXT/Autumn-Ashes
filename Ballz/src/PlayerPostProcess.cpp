#include "stdafx.h"
#include "PlayerPostProcess.h"
#include "Player.h"

PlayerPostProcess::PlayerPostProcess(Player* player) : p(player)
{
    injectPostProcess(&Global::mPPMgr->vars);
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

    auto mCamera = p->mCamera;

    float interpolationFactor = mPreviousFPS*0.03f*(vars->mbAmount);

    Ogre::Quaternion estimatedOrientation = Ogre::Quaternion::nlerp(interpolationFactor, mCamera->getDerivedOrientation(), prevOr);
    Ogre::Vector3    estimatedPosition = (1 - interpolationFactor)*mCamera->getDerivedPosition() + interpolationFactor*prevPos;
    Ogre::Matrix4 viewMatrix = Ogre::Math::makeViewMatrix(estimatedPosition, estimatedOrientation);
    Ogre::Matrix4 projectionMatrix = mCamera->getProjectionMatrix();
    /*
    auto wcorners = mCamera->getWorldSpaceCorners();// getCullingFrustum()->getWorldSpaceCorners();
    Ogre::Vector3 corners[8];

    for (int i = 0; i < 8; i++)
    {
        corners[i] = mCamera->getViewMatrix() * wcorners[i];
    }

    Ogre::Vector3 cornersDir[4];
    cornersDir[0] = corners[4] - corners[0];
    cornersDir[1] = corners[5] - corners[1];
    cornersDir[2] = corners[6] - corners[2];
    cornersDir[3] = corners[7] - corners[3];

    *iPM = Ogre::Matrix4(cornersDir[0].x, cornersDir[0].y, cornersDir[0].z, 1,
                         cornersDir[1].x, cornersDir[1].y, cornersDir[1].z, 1,
                         cornersDir[2].x, cornersDir[2].y, cornersDir[2].z, 1,
                         cornersDir[3].x, cornersDir[3].y, cornersDir[3].z, 1);*/

    vars->ipm = mCamera->getProjectionMatrix().inverse();
    vars->projm = mCamera->getProjectionMatrix();// mCamera->getViewMatrix().inverse();// *viewMatrix;

    vars->pvp = projectionMatrix*viewMatrix;
    vars->ivp = (projectionMatrix*mCamera->getViewMatrix()).inverse();

    mPreviousFPS = 1 / tslf;
    prevPos = mCamera->getDerivedPosition();
    prevOr = mCamera->getDerivedOrientation();
}