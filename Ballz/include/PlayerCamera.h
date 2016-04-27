#pragma once

#include "stdafx.h"
#include "CameraShaker.h"

class Player;
class Rolling;

class PlayerCamera
{
	friend class PlayerClimbing;

	struct CamArrivalInfo
	{
		Ogre::SceneNode* tempNode = nullptr;
		Ogre::Vector3 pos;
		Ogre::Quaternion dir;
		float timer = 0;
		float duration = 0;
	};

public:

	PlayerCamera(Player* player, Ogre::SceneNode* headNode);
	~PlayerCamera();

	/*	power-0.1 weak, 0.5 strong;
	impulse-0.1 slow,0.4 strong  */
	void startCameraShake(float time, float power, float impulse);

	Ogre::Vector3 getPosition() const;
	Ogre::Quaternion getOrientation() const;
	Ogre::Quaternion getBaseOrientation() const;
	Ogre::Vector3 getFacingDirection() const;

	Ogre::Camera* detachCamera();
	void detachCamera(Ogre::SceneNode* target);

	void attachCamera(bool silent = false);
	void attachCameraWithTransition(float duration);

	void rotateCamera(Ogre::Real hybX, Ogre::Real hybY);
	void rotateCamera(Ogre::Quaternion or );

	Ogre::SceneNode* detachHead();
	void attachHead(Ogre::SceneNode* headNode = nullptr);

	void manageFall(float velocity);
	void nodHead(float velocity);

	bool ownsCamera;
	void update();
	void updateHead();

	float afterFall(float duration, bool doRoll);

	Ogre::SceneNode* getCamNode();

	Ogre::Camera* camera;
	CameraShaker shaker;

	float head_turning;

private:

	class Rolling
	{
	public:

		bool active()
		{
			return rollingLeft > 0;
		}
		void update(float time);
		float doRoll(float duration, bool changeOr);

		void setTargetNodes(Ogre::SceneNode* heightNode, Ogre::SceneNode* rollNode);

	private:

		bool changeOr;
		float rollingLeft = 0, rollingDuration;
		Ogre::SceneNode* rollNode;
		Ogre::SceneNode* heightNode;
	};

	Rolling rolling;

	Player* player;
	Ogre::SceneNode* camnode, *shakeNode, *necknode, *headnode;

	CamArrivalInfo cameraArrival;
	void updateCameraArrival();

	char fallPitch, cameraWalkFinisher;
	int walkCycle = 0;

	float camPitch, fallPitchTimer, cam_walking;

	float fallVelocity;
};
