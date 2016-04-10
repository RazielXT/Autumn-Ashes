#include "stdafx.h"
#include "PlayerHanging.h"
#include "Player.h"
#include "Pole.h"
#include "MUtils.h"
#include "GameStateManager.h"
#include "GUtils.h"
#include "GameUi.h"

void PlayerHanging::jump()
{
	if (player->autoTarget->targetPole)
	{
		jumpTo(player->autoTarget->targetPole);
	}
	else
	{
		player->body->setPositionOrientation(hBody->getPosition(), Ogre::Quaternion::IDENTITY);
		release();
		player->pCamera->shaker.nodHead(1.5f);
		player->pCamera->attachCameraWithTransition(0.3f);
		player->body->setVelocity(currentDir * 7 + Ogre::Vector3::UNIT_Y * 3);
	}
}

void PlayerHanging::attachTo(Pole* target)
{
	setCurrentPole(target);

	auto currentPos = player->camPosition;

	hBody = new OgreNewt::Body(Global::mWorld, Global::player->col_p);
	hBody->setPositionOrientation(currentPos, Ogre::Quaternion::IDENTITY);
	hBody->setMassMatrix(1, Ogre::Vector3(1, 1, 1));
	hBody->setLinearDamping(0.2f);
	hBody->setCustomForceAndTorqueCallback<PlayerHanging>(&PlayerHanging::moveCallback, this);
	hBody->setMaterialGroupID(Global::gameMgr->wMaterials.noCollide_mat);

	hBody->setVelocity(currentPole->direction);

	joints[0] = new OgreNewt::BallAndSocket(hBody, NULL, currentPole->position, 0);
	joints[1] = new OgreNewt::BallAndSocket(hBody, NULL, currentPole->position + currentPole->pinDirection, 0);

	state = Attaching;
	player->pCamera->shaker.nodHead(1);
	transition.initializeTransition(currentPos, 0.3f);
}

Ogre::Quaternion PlayerHanging::getOrientation()
{
	auto lookFront = direction.getRotationTo(Ogre::Vector3::NEGATIVE_UNIT_Y);
	auto upDir = currentPole->position - hBody->getPosition() ;
	currentDir = lookFront*upDir;

	auto xDir = currentPole->pinDirection;
	if (!inversePoleDir)
		xDir *= -1;

	Ogre::Quaternion hangOr(xDir, upDir, -currentDir);

	return hangOr;
}

void PlayerHanging::moveCallback(OgreNewt::Body* me, float timeStep, int threadIndex)
{
	Ogre::Vector3 force(0,0,0);
	float power = 5;

	if (forceDirection == Front)
		force = power * currentDir;
	else if (forceDirection == Back)
		force = -power * currentDir;

	me->addForce(force + Ogre::Vector3(0,-5,0));
}

PlayerHanging::PlayerHanging(Player* p) : player(p)
{
	camera = player->pCamera->camera;
}

void PlayerHanging::update(float tslf)
{
	if (currentPole)
	{
		direction = currentPole->direction;
		if (inversePoleDir)
			direction *= -1;

		if (state == Jumping)
		{
			if (transition.updateJump(tslf))
			{
				attachTo(currentPole);
			}
		}
		else
		{
			auto pos = hBody->getPosition();

			if (state == Attaching)
			{
				if (transition.updateTransition(tslf))
				{
					state = Hanging;
					camNode->attachObject(player->pCamera->camera);
				}

				transition.refreshTransition(getOrientation(), pos);
			}
			else
			{
				transition.transitionNode->setPosition(pos);
				transition.transitionNode->setOrientation(getOrientation());

				Quaternion qpitch = Quaternion(Degree(headState.pitch), Vector3(0, 1, 0));
				Quaternion qyaw = Quaternion(Degree(headState.yaw), Vector3(1, 0, 0));
				camNode->setOrientation(qpitch*qyaw);
			}
		}
	}
}

void PlayerHanging::jumpTo(Pole* target)
{
	if (state == Jumping)
		return;

	setCurrentPole(target);

	player->enableControl(false);

	forceDirection = None;
	state = Jumping;
	player->hanging = true;
	player->pCamera->shaker.nodHead(6);
	headState.pitch = headState.yaw = 0;

	transition.initializeJump(currentPole->position - Ogre::Vector3(0,1,0));

	if (!camNode)
		camNode = transition.transitionNode->createChildSceneNode();
}

void PlayerHanging::release()
{
	if (joints[0])
	{
		delete joints[0];
		delete joints[1];

		joints[0] = nullptr;
	}

	if (hBody)
	{
		delete hBody;
		hBody = nullptr;
	}

	state = NotHanging;
	currentPole = nullptr;
	player->hanging = false;
	player->enableControl(true);
}

void PlayerHanging::setCurrentPole(Pole* target)
{
	if (target != currentPole)
	{
		if (currentPole)
			release();

		currentPole = target;
		direction = currentPole->direction;
		inversePoleDir = direction.dotProduct(player->facingDir) < 0;
		if (inversePoleDir)
			direction *= -1;
	}
}

void PlayerHanging::movedMouse(const OIS::MouseEvent &e)
{
	if (state != Hanging)
		return;

	float mod = -0.1f;
	float mouseX = e.state.X.rel*mod;
	float mouseY = e.state.Y.rel*mod;

	const float maxAngle = 50;

	headState.pitch = Math::Clamp(headState.pitch + mouseX, -maxAngle, maxAngle);
	headState.yaw = Math::Clamp(headState.yaw + mouseY, -maxAngle, maxAngle);
}

bool PlayerHanging::pressedKey(const OIS::KeyEvent &arg)
{
	if (state == Hanging)
	{
		if (arg.key == OIS::KC_W)
		{
			forceDirection = Front;
			return true;
		}

		if (arg.key == OIS::KC_S)
		{
			forceDirection = Back;
			return true;
		}

		if (arg.key == OIS::KC_X)
		{
			direction *= -1;
			inversePoleDir = !inversePoleDir;
			return true;
		}

		if (arg.key == OIS::KC_SPACE)
		{
			jump();
			return true;
		}
	}

	return false;
}

bool PlayerHanging::releasedKey(const OIS::KeyEvent &arg)
{
	if (state == Hanging)
	{
		if (arg.key == OIS::KC_W || arg.key == OIS::KC_S)
		{
			forceDirection = None;
			return true;
		}

		if (arg.key == OIS::KC_C)
		{
			release();
			return true;
		}

		if (arg.key == OIS::KC_SPACE)
		{
			jump();
			return true;
		}
	}

	return false;
}

