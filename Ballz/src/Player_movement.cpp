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

    if (wallrunning)
        pClimbing->stopClimbing();
}

void Player::enableMovement(bool enable)
{
    inMoveControl = enable;

    if (!enable)
        stopMoving();
}

void Player::move_callback(OgreNewt::Body* me, float timeStep, int threadIndex)
{
    me->addForce(Ogre::Vector3(0, -9.0f, 0));
    me->addForce(forceDirection);
}

void Player::walkingSound(Ogre::Real time)
{
    walkSoundTimer += (time*bodyVelocityL / 6.6f);

    if (walkSoundTimer > 0.4)
    {
        Global::audioLib->playWalkingSound(bodyPosition.x, bodyPosition.y - 2, bodyPosition.z, groundID);

        walkSoundTimer = 0;
    }
}

void Player::updateDirectionForce()
{
    forceDirection = Vector3::ZERO;

    if (!wallrunning && !climbing && !pParkour->isRolling())
    {
        if (!moving)
        {
            body->setMaterialGroupID(wmaterials->stoji_mat);
            walkSoundTimer = 0.37;
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

    if (moving && onGround)
    {
        if (movespeed < 17)
            movespeed += tslf * 10;
        else
            movespeed = 17;

        walkingSound(tslf);
    }
    else movespeed = 7;
}

float Shaker::doRoll(float duration, Ogre::SceneNode* rNode, Ogre::SceneNode* hNode)
{
    if (rollingLeft > 0)
        return rollingLeft;

    heightNode = hNode;
    rollNode = rNode;
    rollingDuration = rollingLeft = duration;

    startCameraShake(duration, 0.4f, 0.6f);

    return duration;
}

void Player::jump()
{
    if (pClimbing->makingPullup())
        return;

    if (!pClimbing->spacePressed() && !pParkour->spacePressed() && onGround)
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
    pParkour->hitGround();

    if (fallVelocity > 35)
    {
        if (!immortal && fallVelocity > 65)
        {
            die();
        }
        else
        {
            pParkour->doRoll();
        }

        if (fallVelocity > 40)
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

    body->setMaterialGroupID(wmaterials->ide_mat);

    Vector3 movedDir = Vector3::ZERO;
    if (forw_key) movedDir.z--;
    if (back_key) movedDir.z++;
    if (right_key) movedDir.x++;
    if (left_key) movedDir.x--;

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

        if (dirAngleDiff > 45 && forw_key && !back_key && !right_key && !left_key)
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
        if ((moving && !climbing && onGround && (bodyVelocityL > 2)) || wallrunning)
        {
            cameraWalkFinisher = 1;
            cam_walking += time*bodyVelocityL;
            float walkSize = wallrunning ? 1.5f : 1.0f;
            auto sinVal = Ogre::Math::Sin(cam_walking);
            camnode->setPosition(0, -1.5f * abs(sinVal) / 7.0f, 0);
            camnode->setOrientation(Quaternion(Degree(sinVal)*time*(bodyVelocityL + 1) * 4 * walkSize, Vector3(0, 0, 1)));
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
        if (onGround && forw_key && abs(mouseX) > 5)
        {
            head_turning += (bodyVelocityL / 9)*(mouseX) / 250.0f;
            if (head_turning > 8)head_turning = 8;
            if (head_turning < -8)head_turning = -8;
            headnode->setOrientation(Quaternion(Ogre::Radian(head_turning / 60), Vector3(0, 0, 1)));
        }
        else if (wallrunning)
        {
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

    updateCameraArrival();
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