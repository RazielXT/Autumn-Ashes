#include "stdafx.h"
#include "PlayerGrab.h"
#include "Player.h"

PlayerGrab::PlayerGrab(Player* player) : p(player), body(player->body)
{
}

void PlayerGrab::grabbed_callback(OgreNewt::Body* obj, float timeStep, int threadIndex)
{
	Vector3 currentPos = obj->getPosition();

    Vector3 targetPos = p->bodyPosition;
	targetPos.y += 1;
    targetPos += p->getFacingDirection()*3;

    Vector3 targetVector = (targetPos - currentPos) ;

	//release if too far away
	if (targetVector.squaredLength() > 30)
	{
		releaseObj();
		return;
	}

    Vector3 objVel = obj->getVelocity();
    Real angleDiff = targetVector.angleBetween(objVel).valueDegrees();

    if (angleDiff > 45)
    {
        obj->setVelocity(obj->getVelocity() / 2);
        targetVector *= angleDiff / 45;
    }

	obj->setForce(targetVector * 10);
    obj->setPositionOrientation(obj->getPosition(), p->necknode->getOrientation());
}

void PlayerGrab::tryToGrab()
{
    Vector3 pos = p->necknode->_getDerivedPosition();
    OgreNewt::BasicRaycast ray(Global::mWorld, pos, pos + p->getFacingDirection()*4, false);
    OgreNewt::BasicRaycast::BasicRaycastInfo info = ray.getFirstHit();

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
            body->setMassMatrix(body->getMass() + Gbody->getMass(), body->getInertia());
            Gbody->setAngularDamping(Vector3(1, 1, 1) * 300);
            Gbody->setLinearDamping(300);
            p->grabbedObj = true;
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

void PlayerGrab::releaseObj()
{
    Gbody->setMaterialGroupID(Global::mWorld->getDefaultMaterialID());
    //Gbody->setMassMatrix(Gbody->getMass(),Gbody->getInertia()*20);
    Gbody->setCustomForceAndTorqueCallback<Player>(&Player::default_callback, p);
    Gbody->setAngularDamping(gADT);
    Gbody->setLinearDamping(gLDT);
    body->setMassMatrix(body->getMass() - Gbody->getMass(), body->getInertia());
    p->grabbedObj = false;
}
