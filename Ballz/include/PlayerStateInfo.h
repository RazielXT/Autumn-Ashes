#pragma once

#include "stdafx.h"

struct PlayerStateInfo
{
    float timestamp;

    Ogre::Vector3 position;
    Ogre::Vector3 velocity;

    Ogre::Quaternion camOrientation;
    float camPitch;

    bool wallrunning;
};
