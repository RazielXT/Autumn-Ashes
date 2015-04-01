#pragma once

#include "stdafx.h"

class Player;

class PlayerClimbing
{
	enum Direction
	{
		Left, Right, Up, Down
	};

	friend class Player;
	
	Player* p;
	OgreNewt::Body* body;
	OgreNewt::Body* Gbody;

	void pressedC();
	bool spacePressed();
	void updateClimbCamera(float moveX);

	void startPullup();
	void updatePullup(float tslf);
	bool makingPullup() { return climb_pullup!=0; }

	void startClimbing(char type);
	void stopClimbing();

	inline void updateVerticalClimb(bool leftPhase);
	void updateClimbMovement(float tslf);
	void tryClimbToSide(Direction dir);
	bool canClimb(Direction direction, bool soundIfTrue = false, bool needSpeed = false, bool secondPhase = false);

	void updateClimbingStats();
	void updateClimbingPossibility();

	float climb_yaw, climb_move_side, climb_move_vert, climb_pullup, noClimbTimer, pullupPos;
	Ogre::Vector3 climb_normal, climbDir;

	OgreNewt::BallAndSocket* climbJoint;

public:

	PlayerClimbing(Player* player);

	void climb_callback(OgreNewt::Body* me, float timeStep, int threadIndex);

	void update(float tslf);
};