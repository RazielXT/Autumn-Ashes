#include "stdafx.h"
#include "Player.h"
#include "PostProcessMgr.h"
#include "Slide.h"

using namespace Ogre;


void Player::initBody()
{
    Ogre::Entity* ent = mSceneMgr->createEntity("name", "play2.mesh");
    SceneNode* node = mSceneMgr->getRootSceneNode()->createChildSceneNode("CenterNode");
    node->attachObject(ent);
    ent->setCastShadows(false);
    ent->setVisible(false);
    OgreNewt::ConvexCollisionPtr col = OgreNewt::ConvexCollisionPtr(new OgreNewt::CollisionPrimitives::ConvexHull(m_World, ent, 10));
    body = new OgreNewt::Body(m_World, col);

    uv = new OgreNewt::UpVector(body, Vector3::UNIT_Y);
    uv2 = new OgreNewt::UpVector(body, Vector3::UNIT_Z);

    Ogre::Vector3 inertia, offset;
    col->calculateInertialMatrix(inertia, offset);
#ifdef OGRENEWT_NO_COLLISION_SHAREDPTR
    //no longer need the collision shape object
    delete col;
#endif
    body->setMassMatrix(0.5, Vector3(20, 20, 20));
    body->setCenterOfMass(offset);
    body->setContinuousCollisionMode(1);
    body->setPositionOrientation(Ogre::Vector3(0, 100, 0), Ogre::Quaternion::IDENTITY);
    body->setLinearDamping(4);
    body->attachNode(node);
    body->setAutoSleep(0);
    //body->setMaterialGroupID(pmat);
    body->setCustomForceAndTorqueCallback<Player>(&Player::move_callback, this);


    necknode = node->createChildSceneNode("NeckNod");
    necknode->setPosition(Vector3(0, 1, 0));

    headnode = necknode->createChildSceneNode("HeadNod");
    headnode->setPosition(Vector3(0, 0, 0));

    shakeNode = headnode->createChildSceneNode("ShakeHeadNod");
    shakeNode->setPosition(Vector3(0, 0, 0));
    shaker = new Shaker(shakeNode);

    camnode = shakeNode->createChildSceneNode("CamNod");
    camnode->attachObject(mCamera);
    camnode->setPosition(Vector3(0, 0, 0));

    ent = mSceneMgr->createEntity("pl_base", "cone_p2.mesh");
    col_p = OgreNewt::ConvexCollisionPtr(new OgreNewt::CollisionPrimitives::ConvexHull(m_World, ent, 10));
}


void Player::jump()
{
	if (pClimbing->makingPullup())
        return;

	if(!pClimbing->spacePressed() && onGround)
    {
        if (!slidesAutoTarget->pressedAction())
        {
            Vector3 vel = body->getVelocity();
            body->setVelocity(vel + Vector3(0, 9, 0));
        }
    }
}

void Player::manageFall()
{
    fallVelocity = abs(bodyVelocityL) * 2;

    if (fallVelocity > 25)
    {
        if (!immortal && fallVelocity > 55)
        {
            die();
        }
        else
        {
            Vector3 vel = body->getVelocity();
            vel.normalise();
            vel.y = 0;

            Vector3 lookDirection = getFacingDirection();
            lookDirection.y = 0;

            Real dirAngleDiff = lookDirection.angleBetween(vel).valueDegrees();

            if (dirAngleDiff < 45 && vel.length()>0.25f)
            {
                rolling = shaker->doRoll(1.2f, headnode, necknode);
            }
        }

        if (fallVelocity > 35)
        {
			*(pPostProcess->ppFall) = std::min(fallVelocity / 7.0f, 8.0f);

            if (!fallPitch)
                fallVelocity = 80;

            slowingDown = 0;

            Global::audioLib->play3D("pad.wav", bodyPosition, 10);
        }
    }

    if (!fallPitch)
    {
        fallPitch = 1;
        fallPitchTimer = 0;
    }

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

	if (stoji)
		startMoveBoost = 1;

	body->setMaterialGroupID(wmaterials->ide_mat);
	stoji = false;

	Vector3 movedDir = Vector3::ZERO;
	if (vpred) movedDir.z--;
	if (vzad) movedDir.z++;
	if (vpravo) movedDir.x++;
	if (vlavo) movedDir.x--;

	forceDirection = mCamera->getDerivedOrientation()*movedDir;
	forceDirection.normalise();
	forceDirection.y = 0;

	if (onGround)
	{
		Vector3 lookDirection = getFacingDirection();
		lookDirection.y = 0;
		Vector3 vel = body->getVelocity();
		vel.y = 0;

		if (gNormal.y > 0.8f)
			forceDirection += (1 - gNormal.y)*forceDirection * 2;

		Real dirAngleDiff = lookDirection.angleBetween(vel).valueDegrees();

		if (dirAngleDiff > 45 && vpred && !vzad && !vpravo && !vlavo)
			forceDirection *= movespeed*dirAngleDiff / 30;
		else
		{
			Real ebd = 1.0f;
			//brzdi pri velkej rychlosti z kopca
			if (bodyVelocityL > 9)
			{
				ebd = 1 / (bodyVelocityL - 8);
			}

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
		forceDirection *= 5;// 3 / (1 + bodyVelocity);
}

void Player::updateHead()
{
    float time = tslf;

    shaker->updateCameraShake(time);

    if (bodyVelocityL < 0.05)
        bodyVelocityL = 0;
    if (bodySpeedAccum + 0.05 < bodyVelocityL)
    {
        bodySpeedAccum += time*3.5f;
        if (bodySpeedAccum>10)
            bodySpeedAccum = 10;
    }
    if (bodySpeedAccum > bodyVelocityL)
    {
        bodySpeedAccum -= time*1.5f;
        if (bodySpeedAccum < 0)
            bodySpeedAccum = 0;
    }

    mSceneMgr->setShadowColour(Ogre::ColourValue(bodySpeedAccum, bodyVelocityL / 3.0f));

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
        //walking camera
        if (!is_climbing && !stoji && onGround && (bodyVelocityL > 2))
        {
            cameraWalkFinisher = 1;
            cam_walking += time*bodyVelocityL;
            camnode->setPosition(0, -1 * abs(Ogre::Math::Sin(cam_walking)) / 7.0f, 0);
            camnode->setOrientation(Quaternion(Degree(Ogre::Math::Sin(cam_walking))*time*(bodyVelocityL + 1) * 4, Vector3(0, 0, 1)));
            //camnode->roll(Degree(Ogre::Math::Sin(cam_walking+Ogre::Math::PI/2))*time*9);
        }
        else if (cameraWalkFinisher)
        {
            if (onGround) cam_walking += time * 8;
            else cam_walking += time * 10;
            Real value = Ogre::Math::Sin(cam_walking);

            if (cameraWalkFinisher == 1)
            {
                if (value > 0)
                    cameraWalkFinisher = 2;
                else
                    cameraWalkFinisher = 3;
            }

            if ((value > 0 && cameraWalkFinisher == 3) || (value < 0 && cameraWalkFinisher == 2))
            {
                camnode->setPosition(0, 0, 0);
                camnode->resetOrientation();
                cameraWalkFinisher = 0;
                cam_walking = 0;
            }
            else
            {
                camnode->setPosition(0, -1 * abs(value) / 10, 0);
                camnode->setOrientation(Quaternion(Degree(Ogre::Math::Sin(cam_walking))*time*(bodyVelocityL + 1) * 4, Vector3(0, 0, 1)));
                //camnode->roll(Degree(Ogre::Math::Sin(cam_walking+Ogre::Math::PI/2))*time*9);
            }
        }

        //roll camera a bit while turning
        if (onGround && vpred && abs(mouseX) > 5)
        {
            head_turning += (bodyVelocityL / 9)*(mouseX - 5) / 250.0f;
            if (head_turning > 8)head_turning = 8;
            if (head_turning < -8)head_turning = -8;
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
            if (head_turning>0) head_turning = 0;
            headnode->setOrientation(Quaternion(Ogre::Radian(head_turning / 60), Vector3(0, 0, 1)));
        }

        mouseX = 0;
    }

    updateHeadArrival();
}

void Player::updateHeadArrival()
{
    if (cameraArrival.tempNode)
    {
        cameraArrival.timer -= tslf;

        if (cameraArrival.timer <= 0)
        {
            mCamera->detachFromParent();
            camnode->attachObject(mCamera);
            mSceneMgr->destroySceneNode(cameraArrival.tempNode);
            cameraArrival.tempNode = nullptr;
        }
        else
        {
            auto w = cameraArrival.timer/0.2f;
            auto pos = cameraArrival.pos*w + camnode->_getDerivedPosition()*(1 - w);
            auto or = Quaternion::Slerp(1-w, cameraArrival.dir, camnode->_getDerivedOrientation(), true);

            cameraArrival.tempNode->setPosition(pos);
            cameraArrival.tempNode->setOrientation(or);
        }
    }
}


void Player::updateUseGui()
{
    auto pos = necknode->_getDerivedPosition();
    OgreNewt::BasicRaycast ray(m_World, pos, pos + mCamera->getDerivedOrientation()*Vector3(0, 0, -4), true);
    OgreNewt::BasicRaycast::BasicRaycastInfo info = ray.getInfoAt(0);

    if (info.mBody)
    {
        //grabbable
        if (info.mBody->getType() == Grabbable)
        {
            Global::gameMgr->myMenu->showUseGui(Ui_Pickup);
        }
        //climbable
        else if (info.mBody->getType() == Pullup_old)
        {
            Global::gameMgr->myMenu->showUseGui(Ui_Climb);
        }
        //trigger
        else if (info.mBody->getType() == Trigger)
        {
            Ogre::Any any = info.mBody->getUserData();

            if (!any.isEmpty())
            {
                bodyUserData* a0 = Ogre::any_cast<bodyUserData*>(any);
                if (a0->enabledTrigger)
                {
                    Global::gameMgr->myMenu->showUseGui(Ui_Use);
                }
            }
        }
    }
}

void Player::updateGroundStats()
{
    OgreNewt::Body* groundBody = nullptr;
    OgreNewt::BasicRaycast ray(m_World, (bodyPosition - Vector3(0, 1.6, 0)), (bodyPosition - Vector3(0, 2.6, 0)), true);
    OgreNewt::BasicRaycast::BasicRaycastInfo info = ray.getInfoAt(0);

    if (info.mBody)
    {
        auto id = info.mBody->getMaterialGroupID();
        if (id == wmaterials->playerIgnore_mat || id == wmaterials->slide_mat)
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
            if (id == wmaterials->playerIgnore_mat || id == wmaterials->slide_mat)
            {
                if (infoc.mBody->getType() == TopSlidePart)
                {
                    // attachToSlide(infoc.mBody);
                }

                return;
            }

            groundBody = infoc.mBody;
            gNormal = 1;
        }
    }

    if (groundBody)
    {
        Ogre::Any any = groundBody->getUserData();

        if (!any.isEmpty())
            groundID = any_cast<bodyUserData*>(any)->material;
        else
            groundID = 3;

        if (!onGround) manageFall();
        onGround = true;
        body->setLinearDamping(4);
    }
    else if (onGround)
    {
        groundID = -1;
        onGround = false;
        body->setLinearDamping(0.0);
        gNormal = 0;
    }
}