#pragma once

#include "stdafx.h"

class Player;
class PlayerCamera;

class PlayerClimbing
{
	enum Direction
	{
		Left, Right, Up, Down
	};

	friend class Player;
	friend class PlayerCamera;

	Player* p;
	OgreNewt::Body* body;
	OgreNewt::Body* Gbody;

	void release();
	bool spacePressed();
	void updateClimbCamera(float moveX);

	float pullupSide = 1;
	void startPullup();
	void updatePullup(float tslf);
	bool makingPullup()
	{
		return climb_pullup!=0;
	}

	void startClimbing(BodyType type);
	void stopClimbing();

	inline void updateVerticalClimb(bool leftPhase);
	void updateClimbMovement(float tslf);
	void tryClimbToSide(Direction dir);
	bool canClimb(Direction direction, bool soundIfTrue = false, bool needSpeed = false, bool secondPhase = false);

	void updateClimbingStats();
	void updateClimbingPossibility();

	float climb_yaw, climb_move_side, climb_move_vert, climb_pullup, noClimbTimer, pullupPos;
	Ogre::Vector3 climb_normal, climbDir;

	OgreNewt::BallAndSocket* climbJoint = nullptr;
	Ogre::SceneNode* camnode, *headnode, *necknode;

public:

	PlayerClimbing(Player* player);

	void climb_callback(OgreNewt::Body* me, float timeStep, int threadIndex);

	void forcePullup(Ogre::Vector3 climbNormal, float startOffset = 0.0f);
	void update(float tslf);
};