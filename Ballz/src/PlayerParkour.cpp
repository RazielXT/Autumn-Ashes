#include "stdafx.h"
#include "PlayerParkour.h"
#include "Player.h"

PlayerParkour::PlayerParkour(Player* player) : p(player), body(player->body)
{
}

void PlayerParkour::doWalljump()
{
    body->setVelocity(Vector3(0, 8, 0));
    allowWalljump = false;

    if (!p->fallPitch)
    {
        p->fallPitch = 1;
        p->fallPitchTimer = 0;
        p->fallVelocity = 30;
    }
    Global::DebugPrint("walljump");
}

bool PlayerParkour::spacePressed()
{
    Global::DebugPrint("Space pressed");

    if (p->wallrunning)
    {
        stopWallrun();

        if (tryWallJump())
        {
            if (possibleWalljump)
                doWalljump();
        }
        else
        {
            Global::DebugPrint("Normal jump");

            auto jumpDir = p->getFacingDirection();
            jumpDir.y = 0;
            jumpDir.normalise();
            jumpDir.y = 1;

            if (jumpDir.dotProduct(wall_normal) < 0.0f)
                jumpDir = wall_normal * 2;
            else
                jumpDir *= 8;

            reattachFixTimer = 0.5f;

            body->setVelocity(jumpDir);
        }

        return true;
    }
    else if (possibleWalljump)
    {
        doWalljump();
        return true;
    }

    Global::DebugPrint("Nothing");

    return false;
}

bool PlayerParkour::updateParkourPossibility()
{
    auto ret = false;

    if (p->forw_key)
        ret = tryWallrun();

    if (!ret && p->forw_key)
        ret = tryWallJump();

    return ret;
}

bool PlayerParkour::tryWallJump()
{
    auto rStart = p->bodyPosition;
    rStart.y += 0.3f;
    auto rDir = p->getFacingDirection();
    rDir.y = 0;
    rDir.normalise();
    auto rEnd = rDir*1.75f + rStart;

    OgreNewt::BasicRaycast ray(Global::mWorld, rStart, rEnd, false);

    rStart.y += 0.6f;
    rEnd.y += 0.6f;
    OgreNewt::BasicRaycast ray2(Global::mWorld, rStart, rEnd, false);

    rStart.y += 1.0f;
    rEnd = rDir*2.5f + rStart;
    OgreNewt::BasicRaycast ray3(Global::mWorld, rStart, rEnd, false);

    //wallclimb = 3- , 2y, 1?
    //walljumpclimb = 3-, 2-, 1y
    //walljump allowed = all y

    bool w1 = false, w2 = false, w3 = false;

    OgreNewt::BasicRaycast::BasicRaycastInfo info = ray.getFirstHit();
    if (info.mBody && info.mBody->getMass()==0)
    {
        auto id = info.mBody->getMaterialGroupID();
        if (id != p->wmaterials->playerIgnore_mat && id != p->wmaterials->slide_mat)
        {
            wall_normal = info.mNormal;
            w1 = true;
        }
    }

    info = ray2.getFirstHit();
    if (info.mBody && info.mBody->getMass() == 0)
    {
        auto id = info.mBody->getMaterialGroupID();
        if (id != p->wmaterials->playerIgnore_mat && id != p->wmaterials->slide_mat)
        {
            wall_normal = info.mNormal;
            w2 = true;
        }
    }

    info = ray3.getFirstHit();
    if (info.mBody && info.mBody->getMass() == 0)
    {
        auto id = info.mBody->getMaterialGroupID();
        if (id != p->wmaterials->playerIgnore_mat && id != p->wmaterials->slide_mat)
        {
            w3 = true;
        }
    }

    possibleWalljump = false;

    //wall jump up
    if (allowWalljump && !p->onGround && w1 && w2 && w3)
    {
        possibleWalljump = true;
    }
    else if (!w3 && (w2 || w1))
    {
        p->pClimbing->forcePullup(wall_normal);

        if (w2)
            Global::DebugPrint("pullup high");
        else
            Global::DebugPrint("pullup low");
    }

    return false;
}

void PlayerParkour::hitGround()
{
    allowWalljump = true;
    possibleWalljump = false;
}

void PlayerParkour::doRoll()
{
    Vector3 vel = body->getVelocity();
    vel.normalise();
    vel.y = 0;

    Vector3 lookDirection = p->getFacingDirection();
    lookDirection.y = 0;

    Real dirAngleDiff = lookDirection.angleBetween(vel).valueDegrees();

    if (dirAngleDiff < 45 && vel.length()>0.5f)
    {
        rolling = p->shaker->doRoll(1.2f, p->headnode, p->necknode);
    }
}

void PlayerParkour::updateRolling(float tslf)
{
    body->setMaterialGroupID(p->wmaterials->ide_mat);
    p->moving = true;
    p->walkSoundTimer = 0.2f;

    auto dirVec = p->necknode->_getDerivedOrientation()*Vector3(0, 0, -1);
    dirVec.y = 0;
    dirVec.normalise();
    p->forceDirection += dirVec * 10 * rolling;

    rolling -= tslf;
}

bool PlayerParkour::tryWallrun()
{
    reattachFixTimer -= p->tslf;

    wallrunSide = 0;
    auto frontDir = p->getFacingDirection();
    frontDir.y = 0;
    frontDir.normalise();


    if (getWallrunInfo(-1, frontDir, 70))
    {
        wallrunSide = -1;
    }
    else if (getWallrunInfo(1, frontDir, 70))
    {
        wallrunSide = 1;
    }

    float dotWall = frontDir.dotProduct(wall_normal);
    bool correctDot = reattachFixTimer > 0 ? dotWall < -0.25f : dotWall < 0.0f;

    if (wallrunSide && correctDot)
    {
        wallrunSpeed = 10;
        wallrunTimer = std::min(p->bodyVelocityL/wallrunSpeed, 1.0f);
        Global::DebugPrint("Start wallrun");
        wallrunCurrentDir = Quaternion(Degree(90 * wallrunSide), Vector3(0, 1, 0))*wall_normal;

        Ogre::Vector3 size(0.2, 0.2, 0.2);
        Ogre::Real mass = 0.3;

        OgreNewt::ConvexCollisionPtr col = OgreNewt::ConvexCollisionPtr(new OgreNewt::CollisionPrimitives::Box(Global::mWorld, size, 0));
        OgreNewt::Body* hbody = new OgreNewt::Body(Global::mWorld, col);

        Ogre::Vector3 inertia, offset;
        col->calculateInertialMatrix(inertia, offset);

        hbody->setMassMatrix(mass, mass*inertia);
        hbody->setCenterOfMass(offset);
        //hbody->attachNode(node);
        hbody->setMaterialGroupID(p->wmaterials->flag_mat);
        hbody->setCustomForceAndTorqueCallback<PlayerParkour>(&PlayerParkour::wallrun_callback, this);
        hbody->setPositionOrientation(p->bodyPosition + Vector3(0, 5, 0), Ogre::Quaternion::IDENTITY);
        body->setCustomForceAndTorqueCallback<Player>(&Player::move_callback_nothing, p);
        wallrunJoint = new OgreNewt::BallAndSocket(hbody, body, p->bodyPosition + Vector3(0, 5, 0), 0);
        hbody->setMaterialGroupID(p->wmaterials->stoji_mat);

        p->wallrunning = true;

        return true;
    }
    else
        return false;
}

bool PlayerParkour::getWallrunInfo(float side, Vector3 frontDir, float testDegree)
{
    auto rStart = p->bodyPosition;

    auto rEnd = Quaternion(Degree(testDegree * side), Vector3(0, 1, 0))*frontDir*1.75f + rStart;

    OgreNewt::BasicRaycast ray(Global::mWorld, rStart, rEnd, false);
    OgreNewt::BasicRaycast::BasicRaycastInfo info = ray.getFirstHit();

    if (info.mBody && info.mBody->getMass() == 0)
    {
        auto id = info.mBody->getMaterialGroupID();
        if (id != p->wmaterials->playerIgnore_mat && id != p->wmaterials->slide_mat)
        {
            wall_normal = info.mNormal;

            if (abs(wall_normal.y) < 0.5f)
            {
                wall_normal.y = 0;
                wall_normal.normalise();

                return true;
            }
        }
    }

    return false;
}

void PlayerParkour::stopWallrun()
{
    body->setCustomForceAndTorqueCallback<Player>(&Player::move_callback, p);
    OgreNewt::Body* b = wallrunJoint->getBody0();
    delete wallrunJoint;
    delete b;

    p->wallrunning = false;
    allowWalljump = true;
    wallrunSide = 0;
}

void PlayerParkour::updateWallrunning()
{
    if (wallrunSide)
    {
        auto distB = wallrunJoint->getBody0()->getPosition().distance(wallrunJoint->getBody1()->getPosition());
        //Global::DebugPrint(std::to_string(distB));

        if (!p->onGround && distB<5.15f && getWallrunInfo(wallrunSide, wallrunCurrentDir))
        {
            wallrunTimer = std::min(wallrunTimer + p->tslf, 1.0f);

            wallrunCurrentDir = Quaternion(Degree(90 * wallrunSide), Vector3(0, 1, 0))* wall_normal;
            p->head_turning += p->tslf*-10*wallrunSide;

            auto dirDiff = p->getFacingDirection().dotProduct(wallrunCurrentDir);

            if (dirDiff > -0.5f)
                return;
        }

        //release
        {
            Global::DebugPrint("wallrun release");

            p->wallrunning = false;
            wallrunSide = 0;
            stopWallrun();

            body->setVelocity(wallrunCurrentDir*wallrunSpeed*wallrunTimer - Ogre::Vector3(0, 1, 0)*wallrunTimer);
        }
    }
}
