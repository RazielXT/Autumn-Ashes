#pragma once

#include "stdafx.h"
#include "HeadTransition.h"

class Player;
struct Pole;

class PlayerHanging
{
	HeadTransition transition;
	enum HangingState { Jumping, Attaching, Hanging, NotHanging } state = NotHanging;
	Ogre::Vector3 direction;
	bool inversePoleDir = false;
	Ogre::Vector3 currentDir;

	OgreNewt::BallAndSocket* joints[2] = { nullptr,nullptr };

	Player* player;
	Ogre::Camera* camera;
	Ogre::SceneNode* camNode = nullptr;
	struct
	{
		float yaw;
		float pitch;
	}
	headState;

	void jump();
	void release();
	void setCurrentPole(Pole* target);

	OgreNewt::Body* hBody;
	Ogre::Quaternion getOrientation();
	enum ForceDirection {Front, Back, None} forceDirection = None;

	void moveCallback(OgreNewt::Body* me, float timeStep, int threadIndex);

public:

	Pole* currentPole = nullptr;

	PlayerHanging(Player* player);

	void jumpTo(Pole* target);
	void attachTo(Pole* target);

	void update(float tslf);

	bool pressedKey(const OIS::KeyEvent &arg);
	bool releasedKey(const OIS::KeyEvent &arg);
	void movedMouse(const OIS::MouseEvent &e);
};