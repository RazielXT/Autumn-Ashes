#pragma once
#include "stdafx.h"

struct HeadTransition
{
    Ogre::Vector3 pos;
    Ogre::Vector3 posTarget;

    Ogre::Quaternion dir;
    Ogre::Quaternion dirTarget;

    float timer;
    float dist;
    Ogre::SceneNode* transitionNode = nullptr;

    void initializeJump(Ogre::Camera* cam, Ogre::Vector3 target);
    bool updateJump(float tslf);

    void initializeTransition(Ogre::Camera* cam, Ogre::Vector3 target, float transitionTime);
    bool updateTransition(float tslf);
    void refreshTransition(Ogre::Quaternion actualOr, Ogre::Vector3 actualPos);

private:

    float transformHeightFuncTime(float time, float hd);
    float heightFunc(float time, float hd);
};