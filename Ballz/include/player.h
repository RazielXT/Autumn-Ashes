#pragma once

#include "stdafx.h"
#include "PlayerAudio.h"
#include "PlayerAutoTarget.h"
#include "PlayerStateInfo.h"
#include "PlayerPostProcess.h"
#include "PlayerClimbing.h"
#include "PlayerCamera.h"
#include "PlayerGrab.h"
#include "PlayerParkour.h"
#include "PlayerSliding.h"
#include "PlayerHanging.h"
#include "CameraShaker.h"

class GameUi;

class Player
{
	friend class PlayerPostProcess;
	friend class PlayerClimbing;
	friend class PlayerGrab;
	friend class PlayerParkour;
	friend class PlayerSwimming;
	friend class PlayerAbilities;
	friend class PlayerSliding;
	friend class PlayerTimeshift;
	friend class PlayerFlash;
	friend class PlayerCamera;
	friend class PlayerHanging;

public:

	Player(WorldMaterials* wMaterials);
	~Player ();

	Ogre::Vector3 gravity;
	void move_callback(OgreNewt::Body* me, float timeStep, int threadIndex );
	void move_callback_nothing(OgreNewt::Body* me, float timeStep, int threadIndex );
	void default_callback(OgreNewt::Body* me, float timeStep, int threadIndex );

	void update(Ogre::Real time);

	void pressedKey(const OIS::KeyEvent &arg);
	void releasedKey(const OIS::KeyEvent &arg);
	void pressedMouse(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	void releasedMouse(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	void movedMouse(const OIS::MouseEvent &e);

	void setPosition(Ogre::Vector3 pos)
	{
		bodyPosition = pos;
		body->setPositionOrientation(pos,body->getOrientation());
	}

	void saveState(PlayerStateInfo& info);
	void loadState(PlayerStateInfo& info);

	void die();
	void enableControl(bool enable);
	void enableMovement(bool enable);
	void hideBody();

	OgreNewt::Body* body = nullptr;

	void stopMoving()
	{
		body->setVelocity(Ogre::Vector3(0, 0, 0));
	};

	bool isInControl()
	{
		return inControl;
	}

	Ogre::Vector3 facingDir;
	Ogre::Vector3 camPosition;
	Ogre::Vector3 bodyPosition;
	float bodyVelocityL;
	float facingWallDistance;

	PlayerPostProcess* pPostProcess;
	PlayerClimbing* pClimbing;
	PlayerGrab* pGrabbing;
	PlayerParkour* pParkour;
	PlayerSwimming* pSwimming;
	PlayerAbilities* pAbilities;
	PlayerSliding* pSliding;
	PlayerCamera* pCamera;
	PlayerAutoTarget* autoTarget;
	PlayerHanging* pHanging;
	PlayerAudio pAudio;

private:

	void updateUseGui();

	void updateStats();
	void updateMovement();
	void updateSpeed();

	void updateDirectionForce();
	void updateGroundStats();

	void jump();
	void manageFall();

	void initBody();

	inline void playWalkSound()
	{
		pAudio.playWalkingSound(bodyPosition.x, bodyPosition.y - 2, bodyPosition.z, groundID, 0.3f);
	}

	GameUi* ui;

	OgreNewt::World* m_World;
	Ogre::SceneManager * mSceneMgr;

	//physics
	OgreNewt::UpVector* uv;
	OgreNewt::UpVector* uv2;
	OgreNewt::ConvexCollisionPtr col_p;
	WorldMaterials* wmaterials;

	//basic state
	bool alive, immortal;
	bool moving, right_key, left_key, back_key, forw_key;
	bool onGround, sprinting, inControl, inMoveControl;
	float sprintTimer = 0;

	//extern state
	bool hanging, grabbedObj, wallrunning, sliding, transformed, surfaceSliding;
	BodyType climbing;

	float bodySpeedAccum, startMoveBoost, movespeed, sprintmeter;

	int groundID, mouseX;
	float tslf, slowingDown;
	Ogre::Vector3 forceDirection, gNormal;
};
