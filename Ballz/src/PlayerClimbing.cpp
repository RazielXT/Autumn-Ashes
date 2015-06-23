#include "stdafx.h"

#include "Player.h"
#include "MathUtils.h"
#include "GameStateManager.h"

PlayerClimbing::PlayerClimbing(Player* player) : p(player), body(player->body)
{
    climb_pullup = 0;
    climb_yaw = 0;
    climb_move_side = 0;
    climb_move_vert = 0;
    noClimbTimer = 0;
    pullupPos = 0;

    Gbody = nullptr;
    climbJoint = nullptr;
}

void PlayerClimbing::update(float tslf)
{
    if (noClimbTimer > 0)
        noClimbTimer -= tslf;

    //making pullup
    if (climb_pullup)
    {
        updatePullup(tslf);
    }
    else if (p->climbing)
    {
        updateClimbMovement(tslf);
    }
}

void PlayerClimbing::climb_callback(OgreNewt::Body* me, float timeStep, int threadIndex)
{
    me->setVelocity(climbDir - climb_normal);
}

void PlayerClimbing::startPullup()
{
    delete climbJoint;

    OgreNewt::CollisionPtr col = body->getCollision();//OgreNewt::ConvexCollisionPtr(new OgreNewt::CollisionPrimitives::Box(GlobalPointer->mWorld,Ogre::Vector3(0.1,0.1,0.1),0));
    OgreNewt::Body* mHelpBody = new OgreNewt::Body(Global::mWorld, col);
    mHelpBody->setPositionOrientation(p->bodyPosition, Ogre::Quaternion::IDENTITY);
    mHelpBody->setMassMatrix(0.5, Ogre::Vector3(20, 20, 20));
    mHelpBody->setCustomForceAndTorqueCallback<Player>(&Player::default_callback, p);
    mHelpBody->setMaterialGroupID(Global::gameMgr->wMaterials->flag_mat);
    climbJoint = new OgreNewt::BallAndSocket(mHelpBody, Gbody, p->bodyPosition, 0);

    climb_normal.normalise();
    float climb_yaw_change = climb_yaw - Gbody->getOrientation().getYaw().valueRadians();
    Ogre::Quaternion q(Ogre::Radian(-climb_yaw_change), Ogre::Vector3(0, 1, 0));
    climb_normal = q*climb_normal;
    climb_normal *= -1;
    body->setVelocity(body->getVelocity() + Vector3(0, 10, 0));
    p->inMoveControl = false;
    Gbody = mHelpBody;

    noClimbTimer = 0.5;
    climb_pullup = 0.05;
}

void PlayerClimbing::pressedC()
{
    if (p->hanging)
    {
        delete climbJoint;
        p->hanging = false;
        noClimbTimer = 1;
    }
    else if (p->climbing)
    {
        stopClimbing();
        noClimbTimer = 1;
    }
}

void PlayerClimbing::forcePullup(Vector3 climbNormal, float startOffset)
{
    pullupSide = (MathUtils::getSideDotProduct(p->getFacingDirection(), climbNormal) < 0) ? -1.0f : 1.0f;
    Global::shaker->startShaking(1.5, 0.5, 0.5, 1, 1, 0.7, 0.25, 0.75, true);

    startClimbing(Climb_Pullup);

    Global::audioLib->play3D("pullup.wav", p->bodyPosition, 5, 0.7f);

    climb_normal = climbNormal;
    climb_pullup = 0.05f + startOffset;

    if (!p->fallPitch)
    {
        p->fallPitch = 1;
        p->fallPitchTimer = 0;
        p->fallVelocity = 50;
    }
}

bool PlayerClimbing::spacePressed()
{
    if (climb_pullup != 0)
        return true;

    if (p->hanging)
    {
        startPullup();
    }
    else if (p->climbing == 2 || p->climbing == 6)
    {
        Global::audioLib->play3D("pullup.wav", p->bodyPosition, 5, 0.7f);
        climb_pullup = 0.05f;
    }
    else if (p->climbing == 5)
    {
        Vector3 camDir = p->getFacingDirection();
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

            body->setVelocity(camDir * 12 + Vector3(0, 4, 0));
        }
    }
    else return false;

    return true;
}

void PlayerClimbing::updateClimbMovement(float tslf)
{
    if (!p->moving && !climb_move_side && !climb_move_vert && !climb_pullup)
    {
        body->setMaterialGroupID(p->wmaterials->stoji_mat);
    }
    else
        p->moving = true;

    auto wmaterials = p->wmaterials;

    //already on move to side
    if (climb_move_side)
    {
        body->setMaterialGroupID(p->wmaterials->ide_mat);

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

            p->camnode->setPosition((1 - abs(climb_move_side - 1)) / 2.0f, 0, 0);
            p->headnode->setOrientation(Quaternion(Ogre::Radian((1 - abs(climb_move_side - 1)) / 20.0f), Vector3(1, 0, 0)));
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
                p->camnode->setPosition((abs(climb_move_side + 1) - 1) / 2, 0, 0);
                p->headnode->setOrientation(Quaternion(Ogre::Radian((1 - abs(-climb_move_side - 1)) / 20), Vector3(1, 0, 0)));
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
                p->camnode->setOrientation(Quaternion(Ogre::Radian((1 - abs(climb_move_vert - 1)) / 20), Vector3(-0.5, 0, 1)));
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
                    p->camnode->setOrientation(Quaternion(Ogre::Radian((abs(climb_move_vert + 1) - 1) / 20), Vector3(0.5, 0, 1)));
            }
    }
    //not moving yet
    else
    {
        if (p->forw_key)
        {
            Vector3 pohlad = p->getFacingDirection();

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
        else if (p->left_key)
        {
            tryClimbToSide(Left);

        }
        else if (p->right_key)
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
                p->necknode->yaw(Ogre::Radian(climb_yaw), Node::TS_WORLD);
                climb_yaw = 0;
            }
            else
            {
                p->necknode->yaw(Ogre::Radian(-tslf), Node::TS_WORLD);
                climb_yaw += tslf;
            }
        }
        else
        {
            if ((climb_yaw - tslf) <= 0)
            {
                p->necknode->yaw(Ogre::Radian(climb_yaw), Node::TS_WORLD);
                climb_yaw = 0;
            }
            else
            {
                p->necknode->yaw(Ogre::Radian(tslf), Node::TS_WORLD);
                climb_yaw -= tslf;
            }
        }
    }
}

void PlayerClimbing::updateVerticalClimb(bool leftPhase)
{
    float diff = leftPhase ? -1.f : 1.f;
    auto wmaterials = p->wmaterials;

    if (p->forw_key)
    {
        Vector3 pohlad = p->getFacingDirection();
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
            p->camnode->setOrientation(Quaternion(Ogre::Radian(diff*(-1 + abs(climb_move_vert + diff)) / 20), Vector3(diff*-0.5f, 0, 1)));
        }
        else body->setMaterialGroupID(wmaterials->stoji_mat);
    }
    else
    {
        climb_move_vert = 0;
        p->camnode->setOrientation(Quaternion(Ogre::Radian(diff*(1 - abs(climb_move_vert - diff)) / 20), Vector3(diff*0.5f, 0, 1)));
        body->setMaterialGroupID(wmaterials->stoji_mat);
    }
}

void PlayerClimbing::tryClimbToSide(Direction dir)
{
    float f = (dir == Left) ? 1.f : -1.f;

    if (canClimb(dir, true, true))
    {
        body->setMaterialGroupID(p->wmaterials->ide_mat);
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
        body->setMaterialGroupID(p->wmaterials->stoji_mat);
}


void PlayerClimbing::updateClimbingPossibility()
{
    if (noClimbTimer > 0)
        return;

    auto pos = p->necknode->_getDerivedPosition() + Vector3(0, 0.25, 0);
    Vector3 predsebou = p->getFacingDirection();
    predsebou.y = 0;
    predsebou.normalise();
    predsebou *= 2;

    auto ray = OgreNewt::BasicRaycast(Global::mWorld , pos, pos + predsebou / 1.3f, false);
    auto info = ray.getFirstHit();
    if (info.mBody)
    {
        if (info.mBody->getType() == Dynamic_Pullup)
        {
            climbJoint = new OgreNewt::BallAndSocket(body, info.mBody, p->bodyPosition + Vector3(0, 2, 0), 0);
            p->hanging = true;
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
                Vector3 pos = p->bodyPosition;
                pos.y = pullupPos - 1.25f;
                body->setPositionOrientation(pos, body->getOrientation());
                climbDir = Vector3::ZERO;

                climb_pullup = p->bodyPosition.y + 0.25f - pullupPos;
                if (climb_pullup > 0) climb_pullup = 0;
            }

        }
        else if (info.mBody->getType() == ZipLinePart)
        {
            //attachToSlide(info.mBody);
        }
    }
}


void PlayerClimbing::updateClimbingStats()
{
    auto pos = p->necknode->_getDerivedPosition() + Vector3(0, 0.25, 0);
    auto ray = OgreNewt::BasicRaycast(Global::mWorld, pos, pos + climb_normal*-3, false);

    auto info = ray.getFirstHit();
    if (info.mBody)
    {
        if ((info.mBody->getType() == Climb || info.mBody->getType() == Pullup_old || info.mBody->getType() == Climb_Pullup) && !p->onGround)
        {
            p->climbing = info.mBody->getType();
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
    else if (!climb_pullup)
        stopClimbing();
}


bool PlayerClimbing::canClimb(Direction direction, bool soundIfTrue, bool needSpeed, bool secondPhase)
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

    Vector3 targetPos = p->bodyPosition + Vector3(0, 1.25, 0) + off / 3;
    //Ogre::LogManager::getSingleton().getLog("RuntimeEvents.log")->logMessage(Ogre::StringConverter::toString(smer),Ogre::LML_NORMAL);
    //Ogre::LogManager::getSingleton().getLog("RuntimeEvents.log")->logMessage(Ogre::StringConverter::toString(off),Ogre::LML_NORMAL);
    //Ogre::LogManager::getSingleton().getLog("RuntimeEvents.log")->logMessage(Ogre::StringConverter::toString(climb_normal),Ogre::LML_NORMAL);
    OgreNewt::BasicRaycast ray(Global::mWorld, targetPos, targetPos + climb_normal*-3, false);
    OgreNewt::BasicRaycast::BasicRaycastInfo info = ray.getFirstHit();

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
        //if current and big offset are ok, move anyway

        targetPos -= off / 3;
        ray = OgreNewt::BasicRaycast(Global::mWorld, targetPos, targetPos + climb_normal*-3, false);
        info = ray.getFirstHit();

        if (info.mBody && (info.mBody->getType() == Climb_Pullup || info.mBody->getType() == Climb || info.mBody->getType() == Pullup_old))
        {
            targetPos += off;
            ray = OgreNewt::BasicRaycast(Global::mWorld, targetPos, targetPos + climb_normal*-3, false);
            info = ray.getFirstHit();

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

void PlayerClimbing::startClimbing(char type)
{
    Ogre::Vector3 size(0.2, 0.2, 0.2);
    Ogre::Real mass = 0.3;
    Entity* ent;
    SceneNode* node;
    Ogre::String name("BodyChytac");

    ent = Global::mSceneMgr->createEntity(name, "boxEL.mesh");
    node = Global::mSceneMgr->getRootSceneNode()->createChildSceneNode(name);
    node->attachObject(ent);
    node->setScale(size);
    ent->setCastShadows(true);
    ent->setMaterialName("GreyWood");
    ent->setVisible(false);

    OgreNewt::ConvexCollisionPtr col = OgreNewt::ConvexCollisionPtr(new OgreNewt::CollisionPrimitives::Box(Global::mWorld, size, 0));
    OgreNewt::Body* hbody = new OgreNewt::Body(Global::mWorld, col);

    Ogre::Vector3 inertia, offset;
    col->calculateInertialMatrix(inertia, offset);

#ifdef OGRENEWT_NO_COLLISION_SHAREDPTR
    delete col;
#endif

    hbody->setMassMatrix(mass, mass*inertia);
    hbody->setCenterOfMass(offset);
    hbody->attachNode(node);
    hbody->setMaterialGroupID(p->wmaterials->flag_mat);
    hbody->setCustomForceAndTorqueCallback<PlayerClimbing>(&PlayerClimbing::climb_callback, this);
    hbody->setPositionOrientation(p->bodyPosition + Vector3(0, 5, 0), Ogre::Quaternion::IDENTITY);
    body->setCustomForceAndTorqueCallback<Player>(&Player::move_callback_nothing, p);
    climbJoint = new OgreNewt::BallAndSocket(hbody, body, p->bodyPosition + Vector3(0, 5, 0), 0);
    hbody->setMaterialGroupID(p->wmaterials->stoji_mat);

    p->climbing = type;

    /*if (type > 2)
    	startCameraShake(0.2f, 0.3f, 0.4f);
    else
    	startCameraShake(0.15f, 0.1f, 0.15f);
    */
    if (p->bodySpeedAccum > 5)
        p->bodySpeedAccum = 5;
}

void PlayerClimbing::stopClimbing()
{
    body->setCustomForceAndTorqueCallback<Player>(&Player::move_callback, p);
    OgreNewt::Body* b = climbJoint->getBody0();
    delete climbJoint;
    delete b;
    Global::mSceneMgr->destroyEntity("BodyChytac");
    Global::mSceneMgr->destroySceneNode("BodyChytac");
    p->climbing = 0;
    climb_yaw = 0;
}

void PlayerClimbing::updatePullup(float tslf)
{
    body->setMaterialGroupID(p->wmaterials->ide_mat);

    if (climb_pullup > 0)
    {
        if (p->hanging)
            p->forceDirection = -5 * climb_normal;

        climb_pullup = Math::Clamp( climb_pullup + tslf, 0.0f, 1.0f);
        climbDir = -climb_normal;
        climbDir.y=0;
        climbDir.normalise();

        Real v = 2 / (2.5f*climb_pullup);
        climbDir *= 2;
        climbDir.y =  v;

        p->head_turning += p->tslf*-15*pullupSide;

        if (climb_pullup == 1.0f)
        {
            climb_pullup = 0;

            if (p->climbing)
                stopClimbing();
            else if (p->hanging)
            {
                p->inMoveControl = true;
                p->hanging = false;
                delete climbJoint;
                delete Gbody;
            }

            noClimbTimer = 0;
        }
    }
    else
    {
        climb_pullup += 2 * tslf;
        if (climb_pullup >= 0) climb_pullup = 0;

        Vector3 pos = p->bodyPosition;
        pos.y = pullupPos - 1.25f;
        body->setPositionOrientation(pos, body->getOrientation());

    }
}

void PlayerClimbing::updateClimbCamera(float moveX)
{
    Vector3 camDir = p->getFacingDirection();
    camDir.y = 0;
    camDir.normalise();
    Real angle = climb_normal.angleBetween(camDir).valueDegrees();

    Real climbCam = climb_normal.getRotationTo(camDir).getYaw().valueDegrees();

    if ((climbCam > 0 && moveX > 0) || (climbCam < 0 && moveX < 0))
    {
        p->necknode->yaw(Degree(moveX), Node::TS_WORLD);
    }
    else
    {
        Real max_angle = 90;
        if (p->climbing) max_angle = 160;

        Real spomal;
        angle -= (180 - max_angle);
        if (angle < 0) spomal = 0;
        else spomal = angle / max_angle;
        p->necknode->yaw(Degree(moveX* spomal), Node::TS_WORLD);
    }
}

