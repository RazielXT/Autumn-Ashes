#pragma once

#include "stdafx.h"
#include "CrowPath.h"
#include "AnimationBlender.h"

class CrowsPack;

class Crow
{
public:

    Crow(bool onGround);
    ~Crow();

    void update(Ogre::Real tslf);

    bool readyToFly() const;
    bool readyToLand() const;

    void flyTo(Ogre::Animation* flightAnim);
    void landTo(Ogre::Vector3 pos);

	bool readyToChangeFlyPath() const;
	void switchFlyTo(Ogre::Animation* flightAnim);

protected:

    void updateAnimationState();

	float flightNoChangeTimer;
    float stateChangeTimer;

    CrowState curAnimType;
    Ogre::SceneNode* mNode;
    Ogre::Entity* mEntity;
    CrowPath path;
    AnimationBlender animation;

    Ogre::Quaternion modelOr;
    Ogre::Vector3 modelOffset;
};