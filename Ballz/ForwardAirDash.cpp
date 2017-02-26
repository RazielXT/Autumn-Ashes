#include "stdafx.h"
#include "ForwardAirDash.h"
#include "Player.h"

ForwardAirDash::ForwardAirDash()
{

}

void move_callback_float(OgreNewt::Body* me, float timeStep, int threadIndex)
{
	me->addForce(Ogre::Vector3(0, 5, 0));
}

bool ForwardAirDash::start()
{
	timer = 0;
	dashed = false;
	fovEffect.start(2, 0.8, 65, 1.0f, 100);

	Global::player->body->setVelocity(Global::player->body->getVelocity()*0.25f);
	Global::player->body->setCustomForceAndTorqueCallback(move_callback_float);

	return true;
}

bool ForwardAirDash::update(Ogre::Real tslf)
{
	timer += tslf;

	if (timer >= 1)
	{
		if (timer < 1.5 || !dashed)
			Global::player->body->setVelocity(Global::camera->direction * 30);

		if (!dashed)
		{
			dashed = true;
			Global::player->body->setCustomForceAndTorqueCallback<Player>(&Player::move_callback, Global::player);
		}
	}

	return fovEffect.update(tslf);
}
