#include "stdafx.h"
#include "Player.h"
#include "PostProcessMgr.h"
#include "ZipLine.h"

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
    body->setLinearDamping(0.8);
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
    if (climb_pullup)
        return;

    if (visi)
    {
        delete climbJoint;

        OgreNewt::CollisionPtr col = body->getCollision();//OgreNewt::ConvexCollisionPtr(new OgreNewt::CollisionPrimitives::Box(GlobalPointer->mWorld,Ogre::Vector3(0.1,0.1,0.1),0));
        OgreNewt::Body* mHelpBody = new OgreNewt::Body(m_World, col);
        mHelpBody->setPositionOrientation(body->getPosition(), Ogre::Quaternion::IDENTITY);
        mHelpBody->setMassMatrix(0.5, Ogre::Vector3(20, 20, 20));
        mHelpBody->setCustomForceAndTorqueCallback<Player>(&Player::default_callback, this);
        mHelpBody->setMaterialGroupID((OgreNewt::MaterialID*)Global::globalData->find("MatFlag")->second);
        climbJoint = new OgreNewt::BallAndSocket(mHelpBody, Gbody, body->getPosition(), 0);

        climb_normal.normalise();
        float climb_yaw_change = climb_yaw - Gbody->getOrientation().getYaw().valueRadians();
        Ogre::Quaternion q(Ogre::Radian(-climb_yaw_change), Ogre::Vector3(0, 1, 0));
        climb_normal = q*climb_normal;
        climb_normal *= -1;
        body->setVelocity(body->getVelocity() + Vector3(0, 15, 0));
        inMoveControl = false;
        Gbody = mHelpBody;

        noClimbTimer = 0.5;
        climb_pullup = 0.05;
    }
    else if (is_climbing == 2 || is_climbing == 6)
    {
        Vector3 pos = body->getPosition();
        Global::audioLib->play3D("pullup.wav", pos, 5, 0.7f);

        climb_pullup = 0.05;
    }
    else if (is_climbing == 5)
    {
        Vector3 camDir = getFacingDirection();
        Vector3 pohlad(-camDir);
        pohlad.y = 0;
        pohlad.normalise();
        Real angle = abs(climb_normal.angleBetween(pohlad).valueDegrees());

        //jump from wall
        if (angle > 50)
        {
            canClimb(Down, true);

            noClimbTimer = 0.1;
            stopClimbing();

            body->setVelocity(camDir * 10 + Vector3(0, 1, 0));
        }
    }
    else
        //if(nazemi)
    {
        Vector3 vel = body->getVelocity();
        body->setVelocity(vel + Vector3(0, 9, 0));
    }
}

void Player::manageFall()
{
    fallVelocity = abs(lastSpeed.length()) * 2;
    Vector3 ppos = body->getPosition();

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
                crouch = 0;
            }
        }

        if (fallVelocity > 35)
        {
            *ppFall = std::min(fallVelocity / 7.0f, 8.0f);

            if (!fallPitch)
                fallVelocity = 80;

            slowingDown = 0;

            Global::audioLib->play3D("pad.wav", ppos, 10);
        }
    }

    if (!fallPitch)
    {
        fallPitch = 1;
        fallPitchTimer = 0;
    }

    Global::audioLib->playFallSound(ppos.x, ppos.y - 2, ppos.z, groundID);
}


void Player::updateHead(Real time)
{
    shaker->updateCameraShake(time);

    if (noClimbTimer > 0)
        noClimbTimer -= time;

    if (bodyVelocity < 0.05)
        bodyVelocity = 0;
    if (bodySpeedAccum + 0.05 < bodyVelocity)
    {
        bodySpeedAccum += time*3.5f;
        if (bodySpeedAccum>10)
            bodySpeedAccum = 10;
    }
    if (bodySpeedAccum > bodyVelocity)
    {
        bodySpeedAccum -= time*1.5f;
        if (bodySpeedAccum < 0)
            bodySpeedAccum = 0;
    }

    mSceneMgr->setShadowColour(Ogre::ColourValue(bodySpeedAccum, bodyVelocity / 3.0f));

    if (fallPitch == 1)
    {
        fallPitchTimer += time;
        if (fallPitchTimer >= 0.1f)
        {
            fallPitch = 2;
            necknode->pitch(Degree(fallVelocity*(0.1f - fallPitchTimer + time)*-3), Node::TS_LOCAL);
            fallPitchTimer = 0.2f;
            fallPitchSize += fallVelocity*(0.1f - fallPitchTimer + time)*-3;
        }
        else
        {
            necknode->pitch(Degree(fallVelocity*time*-3), Node::TS_LOCAL);
            fallPitchSize += fallVelocity*time*-3;
        }
    }
    else if (fallPitch == 2)
    {
        fallPitchTimer -= time;
        if (fallPitchTimer <= 0)
        {
            fallPitch = 0;
            necknode->pitch(Degree(fallVelocity*(fallPitchTimer + time)*1.5f), Node::TS_LOCAL);
            fallPitchSize += fallVelocity*(fallPitchTimer + time)*1.5f;
        }
        else
        {
            necknode->pitch(Degree(fallVelocity*time*1.5f), Node::TS_LOCAL);
            fallPitchSize += fallVelocity*time*1.5f;
        }
    }

    if (crouch > 0)
    {
        if (crouch == 1)
        {
            if (crouch_am > -1.5f) crouch_am -= time * 8;
            if (crouch_am < -1.5f) crouch_am = -1.5f;
            necknode->setPosition(0, 1 + crouch_am, 0);
        }
        if (crouch == 2)
        {
            crouch_am += time * 8;
            if (crouch_am > 0)
            {
                crouch_am = 0;
                crouch = 0;
            }
            necknode->setPosition(0, 1 + crouch_am, 0);
        }
    }
    else
        //not crouching
    {
        //walking camera
        if (!is_climbing && !stoji && onGround && (bodyVelocity > 2))
        {
            cameraWalkFinisher = 1;
            cam_walking += time*bodyVelocity;
            camnode->setPosition(0, -1 * abs(Ogre::Math::Sin(cam_walking)) / 7.0f, 0);
            camnode->setOrientation(Quaternion(Degree(Ogre::Math::Sin(cam_walking))*time*(bodyVelocity + 1) * 4, Vector3(0, 0, 1)));
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
                camnode->setOrientation(Quaternion(Degree(Ogre::Math::Sin(cam_walking))*time*(bodyVelocity + 1) * 4, Vector3(0, 0, 1)));
                //camnode->roll(Degree(Ogre::Math::Sin(cam_walking+Ogre::Math::PI/2))*time*9);
            }
        }

        //roll camera a bit while turning
        if (onGround && vpred && abs(mouseX) > 5)
        {
            head_turning += (bodyVelocity / 9)*(mouseX - 5) / 250.0f;
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
}


void Player::updateClimbMovement()
{
    if (!vpravo && !vpred && !vzad && !vlavo && !climb_move_side && !climb_move_vert && !climb_pullup)
    {
        body->setMaterialGroupID(wmaterials->stoji_mat);
        stoji = true;
    }
    else
        stoji = false;

    //already on move to side
    if (climb_move_side)
    {
        body->setMaterialGroupID(wmaterials->ide_mat);

        Vector3 dir = body->getVelocity();
        dir.y = 0;
        body->setVelocity(dir);

        //right
        if (climb_move_side > 0)
        {
            if (canClimb(Right))
            {
                climbDir = climb_normal;
                climbDir.y = 0;
                climbDir.normalise();
                Real temp(climbDir.x);
                climbDir.x = 2 * climbDir.z;
                climbDir.z = -2 * temp;
            }
            else
            {
                climbDir = Ogre::Vector3::ZERO;
                body->setMaterialGroupID(wmaterials->stoji_mat);
            }

            climb_move_side -= tslf * 3;
            if (climb_move_side <= 0)
            {
                climb_move_side = 0;
                body->setMaterialGroupID(wmaterials->stoji_mat);
            }
            camnode->setPosition((1 - abs(climb_move_side - 1)) / 2.0f, 0, 0);
            headnode->setOrientation(Quaternion(Ogre::Radian((1 - abs(climb_move_side - 1)) / 20.0f), Vector3(1, 0, 0)));
        }
        else
            //left
            if (climb_move_side < 0)
            {
                if (canClimb(Left))
                {
                    climbDir = climb_normal;
                    climbDir.y = 0;
                    climbDir.normalise();
                    Real temp(climbDir.x);
                    climbDir.x = -2 * climbDir.z;
                    climbDir.z = 2 * temp;
                }
                else
                {
                    climbDir = Ogre::Vector3::ZERO;
                    body->setMaterialGroupID(wmaterials->stoji_mat);
                }

                climb_move_side += tslf * 3;
                if (climb_move_side >= 0)
                {
                    climb_move_side = 0;
                    body->setMaterialGroupID(wmaterials->stoji_mat);
                }
                camnode->setPosition((abs(climb_move_side + 1) - 1) / 2, 0, 0);
                headnode->setOrientation(Quaternion(Ogre::Radian((1 - abs(-climb_move_side - 1)) / 20), Vector3(1, 0, 0)));
            }
    }
    //already on move vertically
    else if (climb_move_vert)
    {
        body->setMaterialGroupID(wmaterials->ide_mat);

        if (climbDir.y >= 0)
        {
            if (!canClimb(Up))
            {
                climbDir = Vector3::ZERO;
                body->setMaterialGroupID(wmaterials->stoji_mat);
            }
        }
        else
        {
            if (!canClimb(Down))
            {
                climbDir = Vector3::ZERO;
                body->setMaterialGroupID(wmaterials->stoji_mat);
            }
            else
            {
                climbDir = -climb_normal;
                float slope = abs(climbDir.y);
                climbDir.y = 0;
                climbDir.normalise();
                climbDir *= slope;
                climbDir += Vector3(0, -2, 0);
                climbDir.normalise();
                climbDir *= 2;
            }
        }

        //if (climb_move_vert != 0)
        //   updateVerticalClimb(climb_move_vert < 0);

        //right phase
        if (climb_move_vert > 0)
        {
            climb_move_vert -= tslf * 4;
            if (climb_move_vert <= 0)
            {
                updateVerticalClimb(false);

            }
            else
                camnode->setOrientation(Quaternion(Ogre::Radian((1 - abs(climb_move_vert - 1)) / 20), Vector3(-0.5, 0, 1)));
        }
        else
            //left phase
            if (climb_move_vert < 0)
            {
                climb_move_vert += tslf * 4;
                if (climb_move_vert >= 0)
                {
                    updateVerticalClimb(true);
                }
                else
                    camnode->setOrientation(Quaternion(Ogre::Radian((abs(climb_move_vert + 1) - 1) / 20), Vector3(0.5, 0, 1)));
            }
    }
    //not moving yet
    else
    {
        if (vpred)
        {
            Vector3 pohlad = getFacingDirection();

            if (pohlad.y >= 0)
            {
                if (canClimb(Up, true))
                {
                    climbDir = Vector3(0, 2, 0);
                    climb_move_vert = 2;
                    body->setMaterialGroupID(wmaterials->ide_mat);
                }
                else
                    body->setMaterialGroupID(wmaterials->stoji_mat);
            }
            else
            {
                if (canClimb(Down, true))
                {
                    climbDir = -climb_normal;
                    float slope = abs(climbDir.y);
                    climbDir.y = 0;
                    climbDir.normalise();
                    climbDir *= slope;
                    climbDir += Vector3(0, -2, 0);
                    climbDir.normalise();
                    climbDir *= 2;
                    climb_move_vert = -2;
                    body->setMaterialGroupID(wmaterials->ide_mat);
                }
                else
                    body->setMaterialGroupID(wmaterials->stoji_mat);
            }
        }
        else if (vlavo)
        {
            tryClimbToSide(Left);

        }
        else if (vpravo)
        {
            tryClimbToSide(Right);
        }
        else
            climbDir = Vector3::ZERO;
    }

    //turning sides
    if (climb_yaw)
    {
        if (climb_yaw < 0)
        {
            if ((climb_yaw + tslf) >= 0)
            {
                necknode->yaw(Ogre::Radian(climb_yaw), Node::TS_WORLD);
                climb_yaw = 0;
            }
            else
            {
                necknode->yaw(Ogre::Radian(-tslf), Node::TS_WORLD);
                climb_yaw += tslf;
            }
        }
        else
        {
            if ((climb_yaw - tslf) <= 0)
            {
                necknode->yaw(Ogre::Radian(climb_yaw), Node::TS_WORLD);
                climb_yaw = 0;
            }
            else
            {
                necknode->yaw(Ogre::Radian(tslf), Node::TS_WORLD);
                climb_yaw -= tslf;
            }
        }
    }
}

void Player::updateVerticalClimb(bool leftPhase)
{
    float diff = leftPhase ? -1.f : 1.f;

    if (vpred)
    {
        Vector3 pohlad = getFacingDirection();
        bool con = false;

        if (pohlad.y > 0)
        {
            if (canClimb(Up, true, false, leftPhase))
            {
                climbDir = Vector3(0, 1.5, 0);
                body->setMaterialGroupID(wmaterials->ide_mat);
                con = true;
            }
        }
        else
        {
            if (canClimb(Down, true, false, leftPhase))
            {
                climbDir = Vector3(0, -1.5, 0);
                body->setMaterialGroupID(wmaterials->ide_mat);
                con = true;
            }
        }

        if (con)
        {
            climb_move_vert = diff*-2.0f - climb_move_vert;
            camnode->setOrientation(Quaternion(Ogre::Radian(diff*(-1 + abs(climb_move_vert + diff)) / 20), Vector3(diff*-0.5f, 0, 1)));
        }
        else body->setMaterialGroupID(wmaterials->stoji_mat);
    }
    else
    {
        climb_move_vert = 0;
        camnode->setOrientation(Quaternion(Ogre::Radian(diff*(1 - abs(climb_move_vert - diff)) / 20), Vector3(diff*0.5f, 0, 1)));
        body->setMaterialGroupID(wmaterials->stoji_mat);
    }
}

void Player::tryClimbToSide(Direction dir)
{
    float f = (dir == Left) ? 1.f : -1.f;

    if (canClimb(dir, true, true))
    {
        body->setMaterialGroupID(wmaterials->ide_mat);
        climbDir = climb_normal;
        climbDir.y = 0;
        climbDir.normalise();
        Real temp(climbDir.x);
        climbDir.x = f * -2 * climbDir.z;
        climbDir.z = f * 2 * temp;

        Vector3 movement = body->getVelocity();
        movement.y = 0;

        if (!climb_move_side && movement.squaredLength() > 0.01f)
            climb_move_side = f * -2;
    }
    else
        body->setMaterialGroupID(wmaterials->stoji_mat);
}


void Player::updateClimbingPossibility()
{
    auto pos = necknode->_getDerivedPosition() + Vector3(0, 0.25, 0);
    Vector3 predsebou = getFacingDirection();
    predsebou.y = 0;
    predsebou.normalise();
    predsebou *= 2;

    auto ray = OgreNewt::BasicRaycast(m_World, pos, pos + predsebou / 1.3f, true);
    auto info = ray.getInfoAt(0);
    if (info.mBody)
    {
        if (info.mBody->getType() == Dynamic_Pullup)
        {
            climbJoint = new OgreNewt::BallAndSocket(body, info.mBody, body->getPosition() + Vector3(0, 2, 0), 0);
            visi = true;
            Gbody = info.mBody;
            climb_normal = info.mNormal;
            climb_normal.y = 0;
            climb_normal.normalise();
            climb_yaw = Gbody->getOrientation().getYaw().valueRadians();
        }
        else if (info.mBody->getType() == Climb || info.mBody->getType() == Pullup_old || info.mBody->getType() == Climb_Pullup)
        {
            climb_normal = info.mNormal.normalisedCopy();

            Ogre::Any any = info.mBody->getUserData();
            ObjectAudio* a = any_cast<bodyUserData*>(any)->sounds;
            if (a)
            {
                Vector3 pos = pos + predsebou;
                Global::audioLib->playRandom3D(a->highHitAudio, pos);
            }

            startClimbing(info.mBody->getType());

            if (info.mBody->getType() == Pullup_old)
            {
                pullupPos = info.getBody()->getPosition().y - 0.15f;
                Vector3 pos = body->getPosition();
                pos.y = pullupPos - 1.25f;
                body->setPositionOrientation(pos, body->getOrientation());
                climbDir = Vector3::ZERO;

                climb_pullup = body->getPosition().y + 0.25f - pullupPos;
                if (climb_pullup > 0) climb_pullup = 0;
            }

        }
        else if (info.mBody->getType() == ZipLinePart)
        {
            attachToZipline(info.mBody);
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
            ((GuiOverlay*)Global::globalData->find("Gui")->second)->showUseGui(Ui_Pickup);
        }
        //climbable
        else if (info.mBody->getType() == Pullup_old)
        {
            ((GuiOverlay*)Global::globalData->find("Gui")->second)->showUseGui(Ui_Climb);
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
                    ((GuiOverlay*)Global::globalData->find("Gui")->second)->showUseGui(Ui_Use);
                }
            }
        }
    }
}

void Player::updateClimbingStats()
{
    auto pos = necknode->_getDerivedPosition() + Vector3(0, 0.25, 0);
    auto ray = OgreNewt::BasicRaycast(m_World, pos, pos + climb_normal*-3, true);
    auto info = ray.getInfoAt(0);
    if (info.mBody)
    {
        if ((info.mBody->getType() == Climb || info.mBody->getType() == Pullup_old || info.mBody->getType() == Climb_Pullup) && !onGround)
        {
            is_climbing = info.mBody->getType();
            Ogre::Vector3 c_normal = info.mNormal.normalisedCopy();

            if (c_normal != climb_normal)
            {
                if (!climb_yaw && (climb_move_vert || climb_move_side))
                    climb_yaw = -c_normal.getRotationTo(climb_normal).getYaw().valueRadians();

                climb_normal = c_normal;
            }
        }
        else
        {
            //if(!climb_pullup)
            //	stopClimbing();
        }
    }
    else
        stopClimbing();
}


bool Player::canClimb(Direction direction, bool soundIfTrue, bool needSpeed, bool secondPhase)
{
    Vector3 off = climb_normal;

    //0-left,1-right,2-up,3-down
    switch (direction)
    {
    case Left:
    {
        Real temp(off.x);
        off.x = -off.z;
        off.z = temp;
        break;
    }
    case Right:
    {
        Real temp(off.x);
        off.x = off.z;
        off.z = -temp;
        break;
    }
    case Up:
    {
        off.y = 0;
        off.y = off.length();
        off.x = 0;
        off.z = 0;
        break;
    }
    case Down:
    {
        off.y = 0;
        off.y = -off.length();
        off.x = 0;
        off.z = 0;
        break;
    }
    };

    Vector3 targetPos = necknode->_getDerivedPosition() + Vector3(0, 0.25, 0) + off / 3;
    //Ogre::LogManager::getSingleton().getLog("RuntimeEvents.log")->logMessage(Ogre::StringConverter::toString(smer),Ogre::LML_NORMAL);
    //Ogre::LogManager::getSingleton().getLog("RuntimeEvents.log")->logMessage(Ogre::StringConverter::toString(off),Ogre::LML_NORMAL);
    //Ogre::LogManager::getSingleton().getLog("RuntimeEvents.log")->logMessage(Ogre::StringConverter::toString(climb_normal),Ogre::LML_NORMAL);
    OgreNewt::BasicRaycast ray(m_World, targetPos, targetPos + climb_normal*-3, true);
    OgreNewt::BasicRaycast::BasicRaycastInfo info = ray.getInfoAt(0);

    if (info.mBody && (info.mBody->getType() == Climb_Pullup || info.mBody->getType() == Climb || info.mBody->getType() == Pullup_old))
    {
        if (soundIfTrue)
        {
            Vector3 movement = body->getVelocity();
            movement.y = 0;

            if (!needSpeed || movement.squaredLength() > 0.01f)
            {
                Ogre::Any any = info.mBody->getUserData();
                ObjectAudio* a = any_cast<bodyUserData*>(any)->sounds;
                if (a)
                    if (!secondPhase || a->climbDoubleFreq)
                    {
                        Global::audioLib->playRandom3D(a->lowHitAudio, targetPos);
                    }
            }
        }

        return true;
    }
    else
    {
        targetPos -= off / 3;
        ray = OgreNewt::BasicRaycast(m_World, targetPos, targetPos + climb_normal*-3, true);
        info = ray.getInfoAt(0);

        if (info.mBody && (info.mBody->getType() == Climb_Pullup || info.mBody->getType() == Climb || info.mBody->getType() == Pullup_old))
        {
            targetPos += off;
            ray = OgreNewt::BasicRaycast(m_World, targetPos, targetPos + climb_normal*-3, true);
            info = ray.getInfoAt(0);

            if (info.mBody && (info.mBody->getType() == Climb_Pullup || info.mBody->getType() == Climb || info.mBody->getType() == Pullup_old))
            {
                if (soundIfTrue)
                {
                    Vector3 movement = body->getVelocity();
                    movement.y = 0;

                    if (!needSpeed || movement.squaredLength() > 0.01f)
                    {
                        Ogre::Any any = info.mBody->getUserData();
                        ObjectAudio* a = any_cast<bodyUserData*>(any)->sounds;
                        if (a)
                            if (!secondPhase || a->climbDoubleFreq)
                            {
                                Global::audioLib->playRandom3D(a->lowHitAudio, targetPos);
                            }
                    }
                }

                return true;
            }
        }
    }

    return false;
}

void Player::startClimbing(char type)
{
    Ogre::Vector3 size(0.2, 0.2, 0.2);
    Ogre::Real mass = 0.3;
    Entity* ent;
    SceneNode* node;
    Ogre::String name("BodyChytac");

    ent = mSceneMgr->createEntity(name, "boxEL.mesh");
    node = mSceneMgr->getRootSceneNode()->createChildSceneNode(name);
    node->attachObject(ent);
    node->setScale(size);
    ent->setCastShadows(true);
    ent->setMaterialName("GreyWood");
    ent->setVisible(false);

    OgreNewt::ConvexCollisionPtr col = OgreNewt::ConvexCollisionPtr(new OgreNewt::CollisionPrimitives::Box(m_World, size, 0));
    OgreNewt::Body* body = new OgreNewt::Body(m_World, col);

    Ogre::Vector3 inertia, offset;
    col->calculateInertialMatrix(inertia, offset);

#ifdef OGRENEWT_NO_COLLISION_SHAREDPTR
    delete col;
#endif

    body->setMassMatrix(mass, mass*inertia);
    body->setCenterOfMass(offset);
    body->attachNode(node);
    body->setMaterialGroupID(wmaterials->flag_mat);
    body->setCustomForceAndTorqueCallback<Player>(&Player::climb_callback, this);
    body->setPositionOrientation(body->getPosition() + Vector3(0, 5, 0), Ogre::Quaternion::IDENTITY);
    body->setCustomForceAndTorqueCallback<Player>(&Player::move_callback_nothing, this);
    climbJoint = new OgreNewt::BallAndSocket(body, body, body->getPosition() + Vector3(0, 5, 0), 0);
    body->setMaterialGroupID(wmaterials->stoji_mat);

    is_climbing = type;

    if (type > 2)
        startCameraShake(0.2f, 0.3f, 0.4f);
    else
        startCameraShake(0.15f, 0.1f, 0.15f);

    if (bodySpeedAccum > 5)
        bodySpeedAccum = 5;
}

void Player::stopClimbing()
{
    body->setCustomForceAndTorqueCallback<Player>(&Player::move_callback, this);
    OgreNewt::Body* b = climbJoint->getBody0();
    delete climbJoint;
    delete b;
    mSceneMgr->destroyEntity("BodyChytac");
    mSceneMgr->destroySceneNode("BodyChytac");
    is_climbing = 0;
    climb_yaw = 0;
}

void Player::updateMovement()
{
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
            if (bodyVelocity > 9)
            {
                ebd = 1 / (bodyVelocity - 8);
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

        if (crouch > 0) forceDirection /= 3.0f;
    }
    //midair
    else
        forceDirection *= 3 / (1 + bodyVelocity);
}

void Player::updatePullup()
{
    body->setMaterialGroupID(wmaterials->ide_mat);

    if (climb_pullup > 0)
    {

        climb_pullup += 2.5f*tslf;
        if (climb_pullup > 2.5f) climb_pullup = 2.5f;
        if (visi) forceDirection = 5 * climb_normal;
        climbDir = -climb_normal;
        //climbDir.y=0;
        //climbDir.normalise();

        Real v = 1 / climb_pullup;
        climbDir *= climb_pullup;
        climbDir.y = v * 2;

        if (climb_pullup == 2.5f)
        {
            climb_pullup = 0;

            if (is_climbing)
                stopClimbing();

            if (visi)
            {
                visi = false;
                delete climbJoint;
                delete Gbody;
                inMoveControl = true;
            }

            noClimbTimer = 0;
        }
    }
    else
    {
        climb_pullup += 2 * tslf;
        if (climb_pullup >= 0) climb_pullup = 0;

        Vector3 pos = body->getPosition();
        pos.y = pullupPos - 1.25f;
        body->setPositionOrientation(pos, body->getOrientation());

    }
}

void Player::attachToZipline(OgreNewt::Body* zipLineBody)
{
    Ogre::Any any = zipLineBody->getUserData();
    auto lineName = (std::string*) any_cast<bodyUserData*>(any)->customData;
    auto zipLine = (ZipLine*)(*Global::globalData)[*lineName];

    if (zipLine->start())
        Global::mEventsMgr->addCachedTask(zipLine);
}

void Player::updateGroundStats()
{
    Vector3 pos = body->getPosition();

    OgreNewt::BasicRaycast ray(m_World, (pos - Vector3(0, 1.6, 0)), (pos - Vector3(0, 2.6, 0)), true);
    OgreNewt::BasicRaycast::BasicRaycastInfo info = ray.getInfoAt(0);

    if (info.mBody)
    {
        if (info.mBody->getMaterialGroupID() == wmaterials->playerIgnore_mat)
        {
            if (info.mBody->getType() == TopZipLinePart)
            {
                attachToZipline(info.mBody);
            }

            return;
        }

        Ogre::Any any = info.mBody->getUserData();

        if (!any.isEmpty())
            groundID = any_cast<bodyUserData*>(any)->material;
        else
            groundID = 3;

        if (!onGround) manageFall();
        onGround = true;
        gNormal = info.mNormal;
        body->setLinearDamping(4);
    }
    else
    {
        OgreNewt::BasicConvexcast rayc(m_World, col_p, (pos - Vector3(0, 2, 0)), Ogre::Quaternion::IDENTITY, (pos - Vector3(0, 2.5, 0)), 1, 1);
        OgreNewt::BasicConvexcast::ConvexcastContactInfo infoc = rayc.getInfoAt(0);
        if (infoc.mBody && infoc.mBody->getMaterialGroupID() != wmaterials->playerIgnore_mat)
        {
            if (infoc.mBody->getMaterialGroupID() == wmaterials->playerIgnore_mat)
            {
                if (infoc.mBody->getType() == TopZipLinePart)
                {
                    attachToZipline(infoc.mBody);
                }

                return;
            }

            Ogre::Any any = infoc.mBody->getUserData();

            if (!any.isEmpty())
                groundID = any_cast<bodyUserData*>(any)->material;
            else
                groundID = 3;

            if (!onGround) manageFall();
            onGround = true;
            gNormal = 1;
            body->setLinearDamping(4);
        }
        else
        {
            groundID = -1;
            lastSpeed = body->getVelocity();
            onGround = false;
            body->setLinearDamping(0.0);
            gNormal = 0;
        }
    }
}