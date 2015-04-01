#include "stdafx.h"
#include "PlayerGrab.h"
#include "Player.h"

PlayerGrab::PlayerGrab(Player* player) : p(player)
{
}

void PlayerGrab::grabbed_callback(OgreNewt::Body* obj, float timeStep, int threadIndex)
{
	Vector3 pos;
	pos = obj->getOgreNode()->_getDerivedPosition();
	Vector3 p2;
	p2 = p->necknode->_getDerivedPosition() + p->mCamera->getDerivedOrientation()*Vector3(0, 0, -3);

	Vector3 s = (p2 - pos) * 10;

	Vector3 o = obj->getVelocity();
	Real rad = s.angleBetween(o).valueDegrees();

	if (rad > 45)
	{
		obj->setVelocity(obj->getVelocity() / 2);
		s *= rad / 40;
	}
	obj->setForce(s);
	obj->setPositionOrientation(obj->getPosition(), p->necknode->getOrientation());

	//release if too far away
	if (s.squaredLength() > 2025)
	{
		Gbody->setMaterialGroupID(Global::mWorld->getDefaultMaterialID());
		//Gbody->setMassMatrix(Gbody->getMass(),Gbody->getInertia()*20);
		Gbody->setCustomForceAndTorqueCallback<Player>(&Player::default_callback, p);
		Gbody->setAngularDamping(gADT);
		Gbody->setLinearDamping(gLDT);
		p->body->setMassMatrix(p->body->getMass() - Gbody->getMass(), p->body->getInertia());
		p->grabbed = false;
	}
}

void PlayerGrab::tryToGrab()
{
	Vector3 pos = p->necknode->_getDerivedPosition();
	OgreNewt::BasicRaycast ray(Global::mWorld, pos, pos + p->mCamera->getDerivedOrientation()*Vector3(0, 0, -4), true);
	OgreNewt::BasicRaycast::BasicRaycastInfo info = ray.getInfoAt(0);

	if (info.mBody)
	{
		//grabbable
		if (info.mBody->getType() == Grabbable)
		{
			Gbody = info.mBody;
			Gbody->setMaterialGroupID(p->wmaterials->playerIgnore_mat);
			Gbody->unFreeze();
			Gbody->setCustomForceAndTorqueCallback<PlayerGrab>(&PlayerGrab::grabbed_callback, this);
			//Gbody->setMassMatrix(Gbody->getMass(),Gbody->getInertia()/20);
			gADT = Gbody->getAngularDamping();
			gLDT = Gbody->getLinearDamping();
			p->body->setMassMatrix(p->body->getMass() + Gbody->getMass(), p->body->getInertia());
			Gbody->setAngularDamping(Vector3(1, 1, 1) * 300);
			Gbody->setLinearDamping(300);
			p->grabbed = true;
		}

		//trigger
		if (info.mBody->getType() == Trigger)
		{
			Ogre::Any any = info.mBody->getUserData();

			if (!any.isEmpty())
			{
				bodyUserData* a0 = Ogre::any_cast<bodyUserData*>(any);
				if (a0->enabledTrigger)
					Global::mEventsMgr->activatePlayerTrigger(a0);
			}
		}
	}
}