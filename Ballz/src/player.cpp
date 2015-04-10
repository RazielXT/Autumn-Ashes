#include "stdafx.h"
#include "Player.h"
#include "PostProcessMgr.h"
#include "PlayerPostProcess.h"
#include "PlayerParkour.h"

using namespace Ogre;

Player::Player(WorldMaterials* wMaterials)
{
    tslf=0;
    bodySpeedAccum=0;
    slowingDown=1;
    startMoveBoost=0;
    cameraWalkFinisher=0;
    walkSoundTimer=0;
    fallPitch=0;
    groundID=-1;
    movespeed=10;
    cam_walking=0;
    head_turning=0;
    mouseX=0;
    bodyVelocityL=0;
    gNormal=Vector3(0,1,0);
    fallVelocity=0;
    camPitch = 0;
    forceDirection=Vector3::ZERO;
    mSceneMgr=Global::mSceneMgr;
    m_World=Global::mWorld;

    right_key=false;
    left_key=false;
    forw_key=false;
    back_key=false;
    moving=false;
    onGround = false;

    hanging=false;
    climbing = 0;
    grabbedObj = false;
    wallrunning = false;

    inControl = true;
    inMoveControl = true;

    immortal = true;
    alive = true;


    wmaterials = wMaterials;

    mCamera=mSceneMgr->getCamera("Camera");
    mCamera->setPosition(0,0,0);
    mCamera->lookAt(Vector3(0,0,-1));

    cameraArrival.tempNode = nullptr;

    initBody();

    pPostProcess = new PlayerPostProcess(this);
    pClimbing = new PlayerClimbing(this);
    pGrabbing = new PlayerGrab(this);
    pParkour = new PlayerParkour(this);

    slidesAutoTarget = new SlidesAutoTargetAsync();

    /*  Ogre::SceneNode* panode = mSceneMgr->getRootSceneNode()->createChildSceneNode(Vector3(0,0,0));
      Ogre::ParticleSystem* ps=mSceneMgr->createParticleSystem("Smoke", "Examples/Smoke");
      panode->attachObject(ps);
      mSceneMgr->getParticleSystem("Smoke")->setRenderQueueGroup(91);*/
}

Player::~Player ()
{
    delete pPostProcess;
    delete pClimbing;
    delete pGrabbing;
    delete pParkour;
    delete slidesAutoTarget;
    delete shaker;
}
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

void Player::move_callback_nothing(OgreNewt::Body* me, float timeStep, int threadIndex )
{
    //me->addForce(Ogre::Vector3(0,0,0));
}
void Player::default_callback(OgreNewt::Body* me, float timeStep, int threadIndex )
{
    me->addForce(Ogre::Vector3(0,-6,0));
}

void Player::pressedKey(const OIS::KeyEvent &arg)
{
    if(!inMoveControl)
        return;

    switch (arg.key)
    {
    case OIS::KC_D:
        right_key=true;
        break;
    case OIS::KC_A:
        left_key=true;
        break;
    case OIS::KC_W:
        forw_key=true;
        break;
    case OIS::KC_S:
        back_key=true;
        break;

    case OIS::KC_C:
        pClimbing->pressedC();
        break;

    case OIS::KC_SPACE:
        jump();
        break;

    case OIS::KC_Q:
        body->setVelocity(body->getVelocity() + Vector3(0, 9, 0));
        break;

    case OIS::KC_G:
    {
        Ogre::LogManager::getSingleton().getLog("RuntimeEvents.log")->logMessage("Pbody: "+Ogre::StringConverter::toString(body->getPosition()),Ogre::LML_NORMAL);
        Ogre::LogManager::getSingleton().getLog("RuntimeEvents.log")->logMessage("Headnod: "+Ogre::StringConverter::toString(mSceneMgr->getSceneNode("HeadNod")->getPosition())+","+Ogre::StringConverter::toString(mSceneMgr->getSceneNode("HeadNod")->_getDerivedPosition()),Ogre::LML_NORMAL);
        Ogre::LogManager::getSingleton().getLog("RuntimeEvents.log")->logMessage("Necknod: "+Ogre::StringConverter::toString(mSceneMgr->getSceneNode("NeckNod")->getPosition())+","+Ogre::StringConverter::toString(mSceneMgr->getSceneNode("NeckNod")->_getDerivedPosition()),Ogre::LML_NORMAL);
        Ogre::LogManager::getSingleton().getLog("RuntimeEvents.log")->logMessage("Camnod: "+Ogre::StringConverter::toString(mSceneMgr->getSceneNode("CamNod")->getPosition())+","+Ogre::StringConverter::toString(mSceneMgr->getSceneNode("CamNod")->_getDerivedPosition()),Ogre::LML_NORMAL);
        Ogre::LogManager::getSingleton().getLog("RuntimeEvents.log")->logMessage("Camera: "+Ogre::StringConverter::toString(mSceneMgr->getCamera("Camera")->getPosition()),Ogre::LML_NORMAL);

        break;
    }
    }
}
void Player::releasedKey(const OIS::KeyEvent &arg)
{
    switch (arg.key)
    {
    case OIS::KC_D:
        right_key=false;
        break;
    case OIS::KC_A:
        left_key=false;
        break;
    case OIS::KC_W:
        forw_key=false;
        break;
    case OIS::KC_S:
        back_key=false;
        break;
    }
}

void Player::pressedMouse(const OIS::MouseEvent &arg,OIS::MouseButtonID id)
{
    switch (id)
    {
    case OIS::MB_Right:
        if (!grabbedObj)
            pGrabbing->tryToGrab();

        break;
    }
}
void Player::releasedMouse(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
    switch (id)
    {
    case OIS::MB_Right:
        if (grabbedObj)
            pGrabbing->releaseObj();

        break;
    }
}
void Player::movedMouse(const OIS::MouseEvent &e)
{
    if (!pParkour->isRolling())
        mouseX = (int)(-1 * e.state.X.rel*Global::timestep);

    int mouseY = (int)(-1 * e.state.Y.rel*Global::timestep);

    if (inControl)
        rotateCamera(mouseX/10.0f,mouseY/10.0f);
}


void Player::die()
{
    delete uv;
    delete uv2;
    body->setLinearDamping(2);
    body->setAngularDamping(Ogre::Vector3(2));
    body->setMassMatrix(5,Ogre::Vector3(1,1,1));

    Ogre::Vector3 v = body->getVelocity();
    v.y = 0;
    v.normalise();
    if(v!=Ogre::Vector3::ZERO)
        body->setOmega(v*bodyVelocityL);
    else
        body->setOmega(Ogre::Vector3(5,0,0));

    enableControl(false);

    alive=false;
}


Ogre::Vector3 Player::getFacingDirection() const
{
    return mCamera->getDerivedOrientation()*Ogre::Vector3(0,0,-1);
}

Ogre::SceneNode* Player::detachHead()
{
    //mSceneMgr->getSceneNode("CenterNode")->removeChild(necknode);
    necknode->removeChild(headnode);

    return headnode;
}

void Player::attachHead(Ogre::SceneNode* headNode)
{
    if (headNode == nullptr)
    {
        headNode = headnode;
        //necknode->setPosition(0, 1, 0);
    }

    necknode->addChild(headNode);
    //mSceneMgr->getSceneNode("CenterNode")->addChild(necknode);
}

void Player::attachCamera()
{
    camPitch = 0;
    fallPitch=0;
    cam_walking=0;
    head_turning=0;
    mouseX=0;
    camPitch=0;
    moving=false;

    necknode->setOrientation(Ogre::Quaternion::IDENTITY);
    //necknode->setPosition(Vector3(0,1,0));

    headnode->setOrientation(Ogre::Quaternion::IDENTITY);
    //headnode->setPosition(Vector3(0,0,0));

    shakeNode->setOrientation(Ogre::Quaternion::IDENTITY);
    //shakeNode->setPosition(Vector3(0,0,0));

    camnode->setOrientation(Ogre::Quaternion::IDENTITY);
    //camnode->setPosition(Vector3(0,0,0));

    Ogre::Quaternion q = mCamera->getDerivedOrientation();

    mCamera->detachFromParent();
    mCamera->setDirection(Ogre::Vector3(0,0,-1));
    camnode->attachObject(mCamera);
    rotateCamera(Ogre::Degree(q.getYaw()).valueDegrees(),Ogre::Degree(q.getPitch()).valueDegrees());
}

void Player::attachCameraWithTransition()
{
    cameraArrival.timer = 0.2f;
    cameraArrival.tempNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();

    cameraArrival.pos = mCamera->getDerivedPosition();
    cameraArrival.dir = mCamera->getDerivedOrientation();

    cameraArrival.tempNode->setPosition(cameraArrival.pos);
    cameraArrival.tempNode->setOrientation(cameraArrival.dir);

    attachCamera();

    mCamera->detachFromParent();
    cameraArrival.tempNode->attachObject(mCamera);

    fallPitch = 1;
    fallPitchTimer = 0;
}

void Player::updateCameraArrival()
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
            auto w = cameraArrival.timer / 0.2f;
            auto pos = cameraArrival.pos*w + camnode->_getDerivedPosition()*(1 - w);
            auto or = Quaternion::Slerp(1 - w, cameraArrival.dir, camnode->_getDerivedOrientation(), true);

            cameraArrival.tempNode->setPosition(pos);
            cameraArrival.tempNode->setOrientation(or);
        }
    }
}

void Player::rotateCamera(Real hybX,Real hybY)
{
    camPitch+=(hybY);
    if (camPitch>-80 && camPitch<80)
        necknode->pitch(Degree(hybY), Node::TS_LOCAL);
    else
    {
        camPitch=camPitch-hybY;
        if (camPitch<0)
        {
            necknode->pitch(Degree(-80-camPitch), Node::TS_LOCAL);
            camPitch=-80.0;
        }
        if (camPitch>0)
        {
            necknode->pitch(Degree(80-camPitch), Node::TS_LOCAL);
            camPitch=80.0;
        }
    }

    if(climbing)
        pClimbing->updateClimbCamera(hybX);
    else
    {
        //damping of turning speed if moving quickly midair
        if (!onGround && bodyVelocityL>10)
            hybX *= std::max(0.1f, (100-bodyVelocityL)/90.f);

        necknode->yaw(Degree(hybX), Node::TS_WORLD);
    }

}

void Player::update(Real time)
{
    tslf = time*Global::timestep;

    pPostProcess->update(tslf);

    if(!alive)
        return;

    updateStats();

    updateDirectionForce();

    pClimbing->update(tslf);

    updateHead();
}

void Player::updateStats()
{
    moving = right_key || forw_key || back_key || left_key;

    bodyPosition = body->getPosition();

    updateGroundStats();

    bodyVelocityL = body->getVelocity().length();

    if (!inControl)
        return;

    if (!onGround && !wallrunning && !hanging && !climbing && !pParkour->isRolling())
    {
        pClimbing->updateClimbingPossibility();

        if (!climbing && !hanging)
            pParkour->updateParkourPossibility();
    }
    else if(climbing)
    {
        pClimbing->updateClimbingStats();
    }
    else if (wallrunning)
    {
        pParkour->updateWallrunning();
    }

    if (!grabbedObj && !climbing && !hanging && !wallrunning)
    {
        updateUseGui();
    }

    if (onGround || wallrunning)
        slidesAutoTarget->updateAutoTarget(mCamera->getDerivedPosition(), getFacingDirection(), tslf, 9);
    else
        slidesAutoTarget->hideAutoTarget();
}

void Player::startCameraShake(float time,float power,float impulse)
{
    shaker->startCameraShake(time,power,impulse);
}


Shaker::Shaker(Ogre::SceneNode* node)
{
    this->node = node;
}

Shaker::~Shaker()
{
}

void Shaker::updateCameraShake(float time)
{
    if (rollingLeft>0)
    {
        rollingLeft -= time;
        Ogre::Radian roll(0);
        float heightDiff = 0;

        if (rollingLeft < 0)
        {
            rollingLeft = 0;
        }
        else
        {
            float rRoll = ((rollingDuration - rollingLeft) / rollingDuration);
            rRoll *= 1.5f;
            rRoll -= 0.05f;
            rRoll = Math::Clamp(rRoll, 0.f, 1.0f);
            rRoll *= -Ogre::Math::TWO_PI;

            roll = rRoll;
            heightDiff = std::max(-1.5f,-2 * std::min(rollingDuration - rollingLeft, rollingLeft));
        }

        Ogre::Quaternion q(roll, Vector3(1,0,0));

        heightNode->setPosition(0, 1 + heightDiff, 0);
        rollNode->setOrientation(q);
    }

    if (!camShaking)
        return;

    camShakeTimer+=time;
    Ogre::Quaternion shakeOr = Ogre::Quaternion::Slerp(camShakeTimer/camShakeTimerEnd,camShakePrev,camShakeTarget);

    node->setOrientation(shakeOr);

    if(camShakeTimer>=camShakeTimerEnd)
    {
        if(!camShakeTimeLeft)
        {
            node->resetOrientation();
            camShaking = false;
            return;
        }

        float timerVar = Ogre::Math::Clamp(camShakeTimeLeft/4.0f+0.5f,0.5f,1.0f);
        camShakeTimer=0;
        camShakeTimerEnd = Ogre::Math::Clamp<float>((camShakePower*(0.5f + camShakeImpulse)*50)/(timerVar+1),0.05f,camShakeImpulse);
        camShakeTimeLeft-=camShakeTimerEnd;

        if(camShakeTimeLeft<=0)
        {
            camShakeTimeLeft = 0;
            camShakePrev = camShakeTarget;
            camShakeTarget = Ogre::Quaternion::IDENTITY;
        }
        else
        {
            float camShakeDirectionX = Ogre::Math::RangeRandom(0.1f,0.45f);
            float camShakeDirectionY = Ogre::Math::RangeRandom(0.25f,0.6f);
            float camShakeDirectionZ = Ogre::Math::RangeRandom(0.1f,0.4f)*camShakePower;

            if(camShakeTarget.getPitch().valueRadians()>0)
            {
                camShakeDirectionX*=-1;
                camShakeDirectionZ*=-1;
            }
            if(Ogre::Math::RangeRandom(-1,1)<0)
                camShakeDirectionY*=-1;

            float random = Ogre::Math::RangeRandom(0.5,1);

            camShakePrev = camShakeTarget;
            camShakeTarget.FromAngleAxis(random*Ogre::Degree(std::min<float>(camShakePower*timerVar*50,65)),Ogre::Vector3(camShakeDirectionX,camShakeDirectionY,camShakeDirectionZ));
        }
    }
}

void Player::updateUseGui()
{
    auto pos = bodyPosition;
    pos.y += 1;
    OgreNewt::BasicRaycast ray(m_World, pos, pos + getFacingDirection()*2, false);
    OgreNewt::BasicRaycast::BasicRaycastInfo info = ray.getFirstHit();

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

void Shaker::startCameraShake(float time,float power,float impulse)
{
    if (!camShaking)
    {
        camShakeTimeLeft = time;
        camShakePower = power;
        camShakeImpulse = Ogre::Math::Clamp(0.5f - impulse, 0.01f, 0.5f);
        camShakeTimer = 0;
        camShakeTimerEnd = 0.01;

        camShakePrev = Ogre::Quaternion::IDENTITY;
        camShakeTarget = Ogre::Quaternion::IDENTITY;
    }
}
