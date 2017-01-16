#include "stdafx.h"
#include "Player.h"
#include "PostProcessMgr.h"
#include "Slide.h"
#include "MUtils.h"

using namespace Ogre;


void Player::enableControl(bool enable)
{
	inControl = enable;
	inMoveControl = enable;

	if (!enable)
		hideBody();
	else
		body->unFreeze();

	if (climbing)
		pClimbing->stopClimbing();

	if (wallrunning)
		pParkour->stopWallrun();
}

void Player::enableMovement(bool enable)
{
	inMoveControl = enable;

	if (!enable)
		hideBody();
}

void Player::move_callback(OgreNewt::Body* me, float timeStep, int threadIndex)
{
	me->addForce(forceDirection + gravity);
}

void Player::updateDirectionForce()
{
	if (!wallrunning && !climbing)// && !pParkour->isRolling())
	{
		if (!moving)
		{
			body->setMaterialGroupID(wmaterials->plNoMove_mat);
			startMoveBoost = 1;
			forceDirection = Vector3::ZERO;
		}
		else
		{
			updateMovement();
		}
	}
	if (pParkour->isRolling())
	{
		pParkour->updateRolling(tslf);
	}

	if (moving && onGround)
	{
		float sprintFactor = sprinting ? 1.5f : 1.0f;

		if (movespeed < 17 * sprintFactor)
			movespeed += tslf * 10 * sprintFactor;
		else
			movespeed = 17 * sprintFactor;

		sprintFactor = sprinting ? 1.25f : 1.0f;
	}
	else
		movespeed = 7;
}

void Player::jump()
{
	if (pClimbing->makingPullup())
		return;

	if (!pClimbing->spacePressed() && !pParkour->spacePressed() && !pParkour->isRolling())
	{
		if (onGround && jumpCounter == 0)
		{
			Vector3 vel = body->getVelocity();

			if (surfaceSliding)
			{
				vel.y = 0;
				vel += 6*gNormal;
			}
			else
				vel += Vector3(0, 12, 0);

			body->setVelocity(vel);
			jumpCounter = 1;
		}
		else if (jumpCounter != 0)
		{
			body->setVelocity(pCamera->getOrientation()*Vector3(0, 0, -15));
			jumpCounter = 2;
		}
	}
}

void Player::manageFall()
{
	auto fallVelocity = bodyVelocityL * 3;
	pParkour->hitGround();
	jumpCounter = 0;

	if (fallVelocity > 50)
	{
		if (!immortal && fallVelocity > 75)
		{
			die();
		}

		bool controlled = sprintTimer < 0.3f;
		pParkour->afterFall(controlled);

		if(!controlled)
		{
			//pPostProcess->vars->hurtEffect = std::min(fallVelocity / 7.0f, 8.0f);
			//pAudio.playHurtSound(bodyPosition.x, bodyPosition.y, bodyPosition.z);

			pCamera->shaker.startShaking(1.5, 1.5, 0.5, 1, 1, 0.7, 0.35, 1, true);

			slowingDown = 0;
		}
	}
	else
	{
		if (fallVelocity < 30)
			return;

		if(sprintTimer < 0.3f)
			pParkour->afterFall(false);

		slowingDown = std::max(1.0f, fallVelocity / 20.0f);
	}

	pCamera->manageFall(fallVelocity);

	pAudio.playFallSound(bodyPosition.x, bodyPosition.y - 2, bodyPosition.z, groundID);
}

void Player::updateMovement()
{
	if (slowingDown > 1)
	{
		slowingDown -= tslf;
		if (slowingDown < 1) slowingDown = 1;
	}
	else if (slowingDown < 1)
	{
		slowingDown += tslf*0.4f;
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

	if (surfaceSliding)
	{
		forceDirection = gNormal;
		forceDirection.y = 0;

		forceDirection = MUtils::quaternionFromDir(forceDirection)*movedDir;

		forceDirection *= forw_key ? 1.0f : 5.0f;
	}
	else if (onGround)
	{
		forceDirection = pCamera->getOrientation()*movedDir;
		forceDirection.y = 0;
		forceDirection.normalise();

		//if (gNormal.y > 0.6f)
		//forceDirection += (1 - gNormal.y)*forceDirection * 2;

		if (gNormal.y > 0.6f && forw_key && !back_key && !right_key && !left_key)
		{
			auto vel = body->getVelocity();
			float yVel = vel.y;
			vel.y = 0;

			auto forcedVec = forceDirection*forceDirection.dotProduct(vel);
			forcedVec.y = yVel;

			body->setVelocity(forcedVec);
			forceDirection *= movespeed;
			//forceDirection += -vel * 5;
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

		if(forceDirection.dotProduct(gNormal)>=0)
			forceDirection += -gNormal.y*gNormal;

		if (gNormal.y > 0.55f)
		{
			Vector3 antiSlide = -gNormal;
			antiSlide.y *= -1;
			forceDirection += antiSlide * 40 * (1 - gNormal.y);
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
	{
		forceDirection = pCamera->getOrientation()*movedDir;
		forceDirection.normalise();
		forceDirection.y = 0;

		forceDirection *= 3;// 3 / (1 + bodyVelocity);
	}


}

void Player::updateGroundStats()
{
	updateUseGui();

	OgreNewt::Body* groundBody = nullptr;
	OgreNewt::BasicRaycast ray(m_World, (bodyPosition - Vector3(0, 0.8, 0)), (bodyPosition - Vector3(0, 1.6, 0)), false);
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
		OgreNewt::BasicConvexcast rayc(m_World, col_p, (bodyPosition - Vector3(0, 1, 0)), Ogre::Quaternion::IDENTITY, (bodyPosition - Vector3(0, 1.5, 0)), 1, 1);
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
		surfaceSliding = groundBody->getType() == Slidable || wantSurfaceSlide;
		body->setLinearDamping(surfaceSliding ? 0.2f : 4);
	}
	else
	{
		body->setLinearDamping(0.2f);

		if (onGround)
		{
			groundID = -1;
			onGround = false;
			gNormal = 0;
			surfaceSliding = false;
		}
	}

	pAudio.surfaceSliding(surfaceSliding ? groundID : -1);
}