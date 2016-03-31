#include "stdafx.h"
#include "Player.h"
#include "PostProcessMgr.h"
#include "Slide.h"

using namespace Ogre;


void Player::enableControl(bool enable)
{
	inControl = enable;
	inMoveControl = enable;

	if (!enable)
		stopMoving();

	if (climbing)
		pClimbing->stopClimbing();

	if (wallrunning)
		pParkour->stopWallrun();
}

void Player::enableMovement(bool enable)
{
	inMoveControl = enable;

	if (!enable)
		stopMoving();
}

void Player::move_callback(OgreNewt::Body* me, float timeStep, int threadIndex)
{
	me->addForce(gravity);
	me->addForce(forceDirection);
}

void Player::updateDirectionForce()
{
	forceDirection = Vector3::ZERO;

	if (!wallrunning && !climbing && !pParkour->isRolling())
	{
		if (!moving)
		{
			body->setMaterialGroupID(wmaterials->plNoMove_mat);
			startMoveBoost = 1;
		}
		else
		{
			updateMovement();
		}
	}
	else if (pParkour->isRolling())
	{
		pParkour->updateRolling(tslf);
	}

	if (levitating)
		forceDirection.y += 10;

	if (moving && onGround)
	{
		float sprintFactor = sprinting ? 1.5f : 1.0f;

		if (movespeed < 17 * sprintFactor)
			movespeed += tslf * 10 * sprintFactor;
		else
			movespeed = 17 * sprintFactor;

		sprintFactor = sprinting ? 1.25f : 1.0f;
	}
	else movespeed = 7;
}

void Player::jump()
{
	if (pClimbing->makingPullup())
		return;

	if (!pClimbing->spacePressed() && !pParkour->spacePressed() && !pParkour->isRolling())
	{
		if (onGround)
		{
			Vector3 vel = body->getVelocity();
			body->setVelocity(vel + Vector3(0, 9, 0));
		}
		else
			levitating = true;
	}
}

void Player::manageFall()
{
	auto fallVelocity = bodyVelocityL * 3;
	pParkour->hitGround();

	if (fallVelocity >= 60)
	{
		if (!immortal && fallVelocity > 75)
		{
			die();
		}
		else
		{
			pParkour->doRoll();
		}

		if (fallVelocity > 50)
		{
			pPostProcess->vars->hurtEffect = std::min(fallVelocity / 7.0f, 8.0f);

			slowingDown = 1;

			Global::audioLib->playHurtSound(bodyPosition.x, bodyPosition.y, bodyPosition.z);

			Global::shaker->startShaking(1.5, 1.5, 0.5, 1, 1, 0.7, 0.35, 1, true);
		}
		else
			slowingDown = 1;// -fallVelocity / 100.0f;
	}

	pCamera->manageFall(fallVelocity);

	Global::audioLib->playFallSound(bodyPosition.x, bodyPosition.y - 2, bodyPosition.z, groundID);
}

void Player::updateMovement()
{
	if (slowingDown < 1)
	{
		slowingDown += (tslf / 2);
		if (slowingDown > 1) slowingDown = 1;
	}

	if (startMoveBoost)
	{
		startMoveBoost -= (tslf * 2);
		if (startMoveBoost < 0) startMoveBoost = 0;
	}

	body->setMaterialGroupID(wmaterials->plMove_mat);

	Vector3 movedDir = Vector3::ZERO;
	if (forw_key) movedDir.z--;
	if (back_key) movedDir.z++;
	if (right_key) movedDir.x++;
	if (left_key) movedDir.x--;

	forceDirection = pCamera->getOrientation()*movedDir;
	forceDirection.normalise();
	forceDirection.y = 0;

	if (onGround)
	{
		Vector3 lookDirection = pCamera->getFacingDirection();
		lookDirection.y = 0;
		Vector3 vel = body->getVelocity();
		vel.y = 0;

		if (gNormal.y > 0.8f)
			forceDirection += (1 - gNormal.y)*forceDirection * 2;

		Real dirAngleDiff = lookDirection.angleBetween(vel).valueDegrees();

		if (dirAngleDiff > 25 && forw_key && !back_key && !right_key && !left_key)
		{
			forceDirection *= movespeed*dirAngleDiff / 25;
			forceDirection += -vel*5;
		}
		else
		{
			Real ebd = 1.0f;
			//brzdi pri velkej rychlosti z kopca
			if (bodyVelocityL > 9)
			{
				ebd = 1 / (bodyVelocityL - 8);
			}
			if (bodyVelocityL < 2)
				ebd = 3 - bodyVelocityL;

			forceDirection *= movespeed*ebd*slowingDown;
		}

		if (gNormal.y > 0.7f)
		{
			Vector3 antiSlide = -gNormal;
			antiSlide.y *= -1;
			forceDirection += antiSlide * 20 * (1 - gNormal.y);
		}
		else
		{
			//too steep
			forceDirection *= 0.25f;
		}

		forceDirection += forceDirection*startMoveBoost;
	}
	//midair
	else
		forceDirection *= 1.5;// 3 / (1 + bodyVelocity);
}

void Player::updateGroundStats()
{
	updateUseGui();

	OgreNewt::Body* groundBody = nullptr;
	OgreNewt::BasicRaycast ray(m_World, (bodyPosition - Vector3(0, 1.6, 0)), (bodyPosition - Vector3(0, 2.6, 0)), false);
	OgreNewt::BasicRaycast::BasicRaycastInfo info = ray.getFirstHit();

	if (info.mBody)
	{
		auto id = info.mBody->getMaterialGroupID();
		if (id == wmaterials->playerIgnore_mat || id == wmaterials->plBlock_mat)
		{
			if (info.mBody->getType() == TopSlidePart)
			{
				//attachToSlide(info.mBody);
			}

			return;
		}

		groundBody = info.mBody;
		gNormal = info.mNormal;
	}
	else
	{
		OgreNewt::BasicConvexcast rayc(m_World, col_p, (bodyPosition - Vector3(0, 2, 0)), Ogre::Quaternion::IDENTITY, (bodyPosition - Vector3(0, 2.5, 0)), 1, 1);
		OgreNewt::BasicConvexcast::ConvexcastContactInfo infoc = rayc.getInfoAt(0);
		if (infoc.mBody)
		{
			auto id = infoc.mBody->getMaterialGroupID();
			if (id == wmaterials->playerIgnore_mat || id == wmaterials->plBlock_mat)
			{
				if (infoc.mBody->getType() == TopSlidePart)
				{
					// attachToSlide(infoc.mBody);
				}

				return;
			}

			groundBody = infoc.mBody;
			gNormal = Vector3(0,1,0);
		}
	}

	if (groundBody)
	{
		Ogre::Any any = groundBody->getUserData();

		if (!any.isEmpty())
			groundID = any_cast<bodyUserData*>(any)->material;
		else
			groundID = 3;

		if (!onGround)
			manageFall();

		onGround = true;
		body->setLinearDamping(4);
	}
	else
	{
		if(levitating)
			body->setLinearDamping(0.15f);
		else
			body->setLinearDamping(0.0);

		if (onGround)
		{
			groundID = -1;
			onGround = false;
			gNormal = 0;
		}
	}
}