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
		player->body->setVelocity(direction * 7 + Ogre::Vector3::UNIT_Y * 3);
	}
}

void PlayerHanging::attachBody()
{
	auto currentPos = transition.transitionNode->getPosition();

	hBody = new OgreNewt::Body(Global::mWorld, Global::player->col_p);
	hBody->setPositionOrientation(currentPos, Ogre::Quaternion::IDENTITY);
	hBody->setMassMatrix(1, Ogre::Vector3(1, 1, 1));
	hBody->setLinearDamping(0.2f);
	hBody->setCustomForceAndTorqueCallback<PlayerHanging>(&PlayerHanging::moveCallback, this);
	hBody->setMaterialGroupID(Global::gameMgr->wMaterials.noCollide_mat);

	hBody->setVelocity(direction);

	joints[0] = new OgreNewt::BallAndSocket(hBody, NULL, currentPole->position, 0);
	joints[1] = new OgreNewt::BallAndSocket(hBody, NULL, currentPole->position + currentPole->pinDirection, 0);
}

Ogre::Quaternion PlayerHanging::getOrientation()
{
	auto dirToJoint = currentPole->position - hBody->getPosition();
	auto rot = dirToJoint.angleBetween(Ogre::Vector3::NEGATIVE_UNIT_Y);

	if (dirToJoint.dotProduct(direction) < 0)
		rot *= -1;

	rot += Ogre::Degree(180);

	Ogre::Quaternion q;
	q.FromAngleAxis(rot, currentPole->pinDirection);

	return MUtils::quaternionFromDir(direction);
}

void PlayerHanging::moveCallback(OgreNewt::Body* me, float timeStep, int threadIndex)
{
	Ogre::Vector3 force;
	float power = 5;

	if (forceDirection == Front)
		force = power * direction;
	else if (forceDirection == Back)
		force = -power * direction;

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
		if (state == Jumping)
		{
			if (transition.updateJump(tslf))
			{
				state = AfterJump;
				attachBody();
				player->pCamera->shaker.nodHead(1);
				transition.initializeTransition(transition.transitionNode->getPosition(), 0.3f);
			}
		}
		else
		{
			auto pos = hBody->getPosition();

			if (state == AfterJump)
			{
				if (transition.updateTransition(tslf))
				{
					state = Hanging;
					transition.transitionNode->attachObject(player->pCamera->camera);
				}

				transition.refreshTransition(getOrientation(), pos);
			}
			else
			{
				transition.transitionNode->setPosition(pos);
				transition.transitionNode->setOrientation(getOrientation());
			}
		}
	}
}

void PlayerHanging::jumpTo(Pole* target)
{
	if (state == Jumping)
		return;

	if (currentPole)
		release();

	player->enableControl(false);

	direction = target->direction;
	if (direction.dotProduct(player->facingDir) < 0)
		direction *= -1;

	currentPole = target;
	forceDirection = None;
	state = Jumping;
	player->hanging = true;
	player->pCamera->shaker.nodHead(6);

	transition.initializeJump(target->position - Ogre::Vector3(0,1,0));
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

