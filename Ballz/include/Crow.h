#pragma once

#include "stdafx.h"
#include "CrowPath.h"

class CrowsPack;

class Crow
{
public:

    Crow();
    ~Crow();

    void update(Ogre::Real tslf);

    bool readyToFly() const;
    bool readyToLand() const;

    void flyTo(Ogre::Animation* flightAnim);
    void landTo(Ogre::Vector3 pos);

protected:

    float stateChangeTimer;

    Ogre::SceneNode* mNode;
    Ogre::Entity* mEntity;
    CrowPath path;
};