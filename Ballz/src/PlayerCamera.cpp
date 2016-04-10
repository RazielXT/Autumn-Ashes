#include "stdafx.h"

#include "Player.h"

using namespace Ogre;

PlayerCamera::PlayerCamera(Player* p, Ogre::SceneNode* base)
{
	player = p;
	cam_walking = 0;
	head_turning = 0;
	cameraWalkFinisher = 0;
	fallPitch = 0;
	camPitch = 0;
	fallVelocity = 0;
	ownsCamera = true;

	camera = Global::mSceneMgr->getCamera("Camera");
	camera->setPosition(0, 0, 0);
	camera->lookAt(Vector3(0, 0, -1));

	necknode = base->createChildSceneNode("NeckNod");
	necknode->setPosition(Vector3(0, 1, 0));

	headnode = necknode->createChildSceneNode("HeadNod");
	headnode->setPosition(Vector3(0, 0, 0));

	shakeNode = headnode->createChildSceneNode("ShakeHeadNod");
	shakeNode->setPosition(Vector3(0, 0, 0));

	camnode = shakeNode->createChildSceneNode("CamNod");
	camnode->attachObject(camera);
	camnode->setPosition(Vector3(0, 0, 0));

	rolling.setTargetNodes(camnode, headnode);

	Global::camera = this;
}

PlayerCamera::~PlayerCamera()
{
}

Ogre::Quaternion PlayerCamera::getOrientation() const
{
	return camera->getDerivedOrientation();
}

Ogre::Quaternion PlayerCamera::getBaseOrientation() const
{
	return necknode->_getDerivedOrientation();
}

Vector3 PlayerCamera::getPosition() const
{
	return player->camPosition;
}

void PlayerCamera::startCameraShake(float time, float power, float impulse)
{
	//shaker->startCameraShake(time,power,impulse);
	Global::camera->shaker.startShaking(power, impulse, time);
}

Ogre::Vector3 PlayerCamera::getFacingDirection() const
{
	return player->facingDir;
}

Ogre::SceneNode* PlayerCamera::detachHead()
{
	//mSceneMgr->getSceneNode("CenterNode")->removeChild(necknode);
	necknode->removeChild(headnode);

	return headnode;
}

void PlayerCamera::attachHead(Ogre::SceneNode* headNode)
{
	if (headNode == nullptr)
	{
		headNode = headnode;
		//necknode->setPosition(0, 1, 0);
	}

	necknode->addChild(headNode);
	//mSceneMgr->getSceneNode("CenterNode")->addChild(necknode);
}

void PlayerCamera::manageFall(float velocity)
{
	fallVelocity = velocity;

	if (!fallPitch && fallVelocity > 60)
		fallVelocity = 80;

	nodHead(fallVelocity);
}

void PlayerCamera::nodHead(float velocity)
{
	if (!fallPitch)
	{
		fallVelocity = velocity;
		fallPitch = 1;
		fallPitchTimer = 0;
	}
}

Ogre::Camera* PlayerCamera::detachCamera()
{
	ownsCamera = false;
	camnode->detachObject(camera);

	return camera;
}

void PlayerCamera::detachCamera(Ogre::SceneNode* target)
{
	target->setPosition(camnode->_getDerivedPosition());
	target->setOrientation(camnode->_getDerivedOrientation());
	target->attachObject(detachCamera());
}

void PlayerCamera::attachCamera(bool silent)
{
	ownsCamera = true;

	if (silent)
	{
		camera->detachFromParent();
		camera->setDirection(Ogre::Vector3(0, 0, -1));
		camnode->attachObject(camera);

		return;
	}

	camPitch = 0;
	fallPitch = 0;
	cam_walking = 0;
	head_turning = 0;
	//todo mouseX = 0;
	//todo moving = false;

	necknode->setOrientation(Ogre::Quaternion::IDENTITY);
	//necknode->setPosition(Vector3(0,1,0));

	headnode->setOrientation(Ogre::Quaternion::IDENTITY);
	//headnode->setPosition(Vector3(0,0,0));

	shakeNode->setOrientation(Ogre::Quaternion::IDENTITY);
	//shakeNode->setPosition(Vector3(0,0,0));

	camnode->setOrientation(Ogre::Quaternion::IDENTITY);
	//camnode->setPosition(Vector3(0,0,0));

	camera->detachFromParent();
	camera->setDirection(Ogre::Vector3(0, 0, -1));
	camnode->attachObject(camera);
}

void PlayerCamera::attachCameraWithTransition(float duration)
{
	cameraArrival.timer = cameraArrival.duration = duration;
	cameraArrival.tempNode = Global::mSceneMgr->getRootSceneNode()->createChildSceneNode();

	cameraArrival.pos = camera->getDerivedPosition();
	cameraArrival.dir = camera->getDerivedOrientation();

	cameraArrival.tempNode->setPosition(cameraArrival.pos);
	cameraArrival.tempNode->setOrientation(cameraArrival.dir);

	attachCamera();
	rotateCamera(cameraArrival.dir);

	camera->detachFromParent();
	cameraArrival.tempNode->attachObject(camera);
}

void PlayerCamera::updateCameraArrival()
{
	if (cameraArrival.tempNode)
	{
		cameraArrival.timer -= player->tslf;

		if (cameraArrival.timer <= 0)
		{
			camera->detachFromParent();
			camnode->attachObject(camera);
			Global::mSceneMgr->destroySceneNode(cameraArrival.tempNode);
			cameraArrival.tempNode = nullptr;
		}
		else
		{
			auto w = cameraArrival.timer / cameraArrival.duration;
			auto pos = cameraArrival.pos*w + camnode->_getDerivedPosition()*(1 - w);
			auto or = Quaternion::Slerp(1 - w, cameraArrival.dir, camnode->_getDerivedOrientation(), true);

			cameraArrival.tempNode->setPosition(pos);
			cameraArrival.tempNode->setOrientation(or );
		}
	}
}

void PlayerCamera::update()
{
	player->facingDir = camera->getDerivedOrientation()*Ogre::Vector3(0, 0, -1);
	player->camPosition = camera->getDerivedPosition();
}

void PlayerCamera::rotateCamera(Ogre::Quaternion or )
{
	rotateCamera(or .getYaw().valueDegrees(), or .getPitch().valueDegrees());
}

void PlayerCamera::rotateCamera(Real yaw, Real pitch)
{
	camPitch += (pitch);
	if (camPitch > -80 && camPitch < 80)
		necknode->pitch(Degree(pitch), Node::TS_LOCAL);
	else
	{
		camPitch = camPitch - pitch;
		if (camPitch < 0)
		{
			necknode->pitch(Degree(-80 - camPitch), Node::TS_LOCAL);
			camPitch = -80.0;
		}
		if (camPitch > 0)
		{
			necknode->pitch(Degree(80 - camPitch), Node::TS_LOCAL);
			camPitch = 80.0;
		}
	}

	if (player->climbing)
		player->pClimbing->updateClimbCamera(yaw);
	else
	{
		//damping of turning speed if moving quickly midair
		if (!player->onGround && player->bodyVelocityL > 10)
			yaw *= std::max(0.1f, (100 - player->bodyVelocityL) / 90.f);

		necknode->yaw(Degree(yaw), Node::TS_WORLD);
	}
}


void PlayerCamera::updateHead()
{
	float time = player->tslf;

	Global::camera->shaker.update(time);
	rolling.update(time);

	auto sQ = Global::camera->shaker.current;
	shakeNode->setOrientation(sQ);

	if (fallPitch == 1)
	{
		fallPitchTimer += time;
		if (fallPitchTimer >= 0.1f)
		{
			fallPitch = 2;
			necknode->pitch(Degree(fallVelocity*(0.1f - fallPitchTimer + time)*-3), Node::TS_LOCAL);
			fallPitchTimer = 0.2f;
		}
		else
		{
			necknode->pitch(Degree(fallVelocity*time*-3), Node::TS_LOCAL);
		}
	}
	else if (fallPitch == 2)
	{
		fallPitchTimer -= time;
		if (fallPitchTimer <= 0)
		{
			fallPitch = 0;
			necknode->pitch(Degree(fallVelocity*(fallPitchTimer + time)*1.5f), Node::TS_LOCAL);
		}
		else
		{
			necknode->pitch(Degree(fallVelocity*time*1.5f), Node::TS_LOCAL);
		}
	}

	{
		float walkAngleSize = 0.10f;

		//walking camera
		if ((player->moving && !player->climbing && !rolling.active() && player->onGround && (player->bodyVelocityL > 2)) || player->wallrunning)
		{
			float sprintFactor = player->sprinting ? 2.0f : 1.0f;
			float walkSize = player->wallrunning ? 1.15f : 1.0f;

			cameraWalkFinisher = 1;
			cam_walking += time*player->bodyVelocityL*walkSize*walkSize;

			float sinVal = Ogre::Math::Sin(cam_walking);
			camnode->setPosition(0, sprintFactor * -1.5f * abs(sinVal) / 7.0f, 0);
			auto rad = Degree(sinVal*(player->bodyVelocityL + 1) * walkSize * sprintFactor)*walkAngleSize;
			camnode->setOrientation(Quaternion(rad, Vector3(0, 0, 1)));

			int currentWalk = (int)(cam_walking / Math::PI);
			if (currentWalk != walkCycle)
			{
				walkCycle = currentWalk;
				float volume = Math::Clamp(player->bodyVelocityL / 35.0f, 0.25f, 0.8f);
				player->playWalkSound();
			}
		}
		else if (cameraWalkFinisher)
		{
			float acc = player->onGround ? 10.0f : 15.0f;
			cam_walking += time*acc;

			float sinVal = Ogre::Math::Sin(cam_walking);

			if (cameraWalkFinisher == 1)
			{
				if (sinVal > 0)
					cameraWalkFinisher = 2;
				else
					cameraWalkFinisher = 3;
			}

			if ((sinVal > 0 && cameraWalkFinisher == 3) || (sinVal < 0 && cameraWalkFinisher == 2))
			{
				camnode->setPosition(0, 0, 0);
				camnode->resetOrientation();
				cameraWalkFinisher = 0;
				cam_walking = 0;

				if (player->onGround || player->wallrunning)
					player->playWalkSound();
			}
			else
			{
				camnode->setPosition(0, -1.5f * abs(sinVal) / 7.0f, 0);
				auto rad = Degree(sinVal*(player->bodyVelocityL + 1))*walkAngleSize;
				camnode->setOrientation(Quaternion(rad, Vector3(0, 0, 1)));
			}
		}

		//roll camera a bit while turning
		if (player->onGround && player->forw_key && abs(player->mouseX) > 5)
		{
			head_turning += (player->bodyVelocityL / 9)*(player->mouseX) / 250.0f;
			head_turning = Math::Clamp(head_turning, -8.0f, 8.0f);
			headnode->setOrientation(Quaternion(Ogre::Radian(head_turning / 60), Vector3(0, 0, 1)));
		}
		else if (player->wallrunning || player->climbing)
		{
			head_turning = Math::Clamp(head_turning, -20.0f, 20.0f);
			headnode->setOrientation(Quaternion(Ogre::Radian(head_turning / 60), Vector3(0, 0, 1)));
		}
		else if (head_turning > 0)
		{
			head_turning -= time * 30;
			if (head_turning < 0) head_turning = 0;
			headnode->setOrientation(Quaternion(Ogre::Radian(head_turning / 60), Vector3(0, 0, 1)));
		}
		else if (head_turning < 0)
		{
			head_turning += time * 30;
			if (head_turning > 0) head_turning = 0;
			headnode->setOrientation(Quaternion(Ogre::Radian(head_turning / 60), Vector3(0, 0, 1)));
		}

		player->mouseX = 0;
	}

	updateCameraArrival();
}

Ogre::SceneNode* PlayerCamera::getCamNode()
{
	return camnode;
}

float PlayerCamera::rollCamera(float rollDuration)
{
	return rolling.doRoll(rollDuration);
}

void PlayerCamera::Rolling::update(float time)
{
	if (rollingLeft > 0)
	{
		rollingLeft -= time;
		Ogre::Radian roll(0);
		float heightDiff = 0;

		if (rollingLeft < 0)
		{
			rollingLeft = 0;
		}
		else
		{
			float rRoll = ((rollingDuration - rollingLeft) / rollingDuration);
			rRoll *= 1.5f;
			rRoll -= 0.05f;
			rRoll = Math::Clamp(rRoll, 0.f, 1.0f);
			rRoll *= -Ogre::Math::TWO_PI;

			roll = rRoll;
			heightDiff = std::max(-1.5f, -2 * std::min(rollingDuration - rollingLeft, rollingLeft));
		}

		Ogre::Quaternion q(roll, Vector3(1, 0, 0));

		heightNode->setPosition(0, 1 + heightDiff, 0);
		rollNode->setOrientation(q);
	}
}

void PlayerCamera::Rolling::setTargetNodes(Ogre::SceneNode* height, Ogre::SceneNode* roll)
{
	rollNode = roll;
	heightNode = height;
}

float PlayerCamera::Rolling::doRoll(float duration)
{
	if (rollingLeft > 0)
		return rollingLeft;

	rollingDuration = rollingLeft = duration;

	return duration;
}