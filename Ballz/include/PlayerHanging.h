#pragma once

#include "stdafx.h"
#include "HeadTransition.h"

class Player;
struct Pole;

class PlayerHanging
{
	HeadTransition transition;
	enum HangingState { Jumping, AfterJump, Hanging, NotHanging } state = NotHanging;
	Ogre::Vector3 direction;

	OgreNewt::BallAndSocket* joints[2] = { nullptr,nullptr };

	Player* player;
	Ogre::Camera* camera;

	void jump();
	void release();

	OgreNewt::Body* hBody;
	void attachBody();
	Ogre::Quaternion getOrientation();
	enum ForceDirection {Front, Back, None} forceDirection = None;

	void moveCallback(OgreNewt::Body* me, float timeStep, int threadIndex);

public:

	Pole* currentPole = nullptr;

	PlayerHanging(Player* player);

	void jumpTo(Pole* target);

	void update(float tslf);

	bool pressedKey(const OIS::KeyEvent &arg);
	bool releasedKey(const OIS::KeyEvent &arg);

};