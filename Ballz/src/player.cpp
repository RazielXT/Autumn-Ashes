#include "stdafx.h"
#include "player.h"
#include "PostProcessMgr.h"

using namespace Ogre;

Player::Player(WorldMaterials* wMaterials)
{
    tslf=0;
    bodySpeedAccum=0;
    slowingDown=1;
    startMoveBoost=0;
    cameraWalkFinisher=0;
    fallPitchSize=0;
    Gbody=NULL;
    walkSoundTimer=0;
    noClimbTimer=0;
    fallPitch=0;
    groundID=-1;
    movespeed=10;
    crouch=0;
    crouch_am=0;
    cam_walking=0;
    head_turning=0;
    mouseX=0;
    climb_pullup=0;
    climb_move_side=0;
    climb_move_vert=0;
    bodyVelocity=0;
    pullupPos=0;
    gNormal=Vector3(0,1,0);
    climbDir=Vector3::ZERO;
    climb_normal=Vector3::ZERO;
    climb_yaw=0;
    fallVelocity=0;
    forceDirection=Vector3::ZERO;
    mSceneMgr=Global::mSceneMgr;
    m_World=Global::mWorld;
    is_climbing=0;
    vpravo=false;
    grabbed=false;
    vlavo=false;
    vpred=false;
    vzad=false;
    stoji=true;
    visi=false;
    onGround=false;
    inControl=true;
    inMoveControl=true;
    immortal=true;
    alive=true;
    rolling = false;
    camPitch=0;
    lastSpeed=0;

    wmaterials = wMaterials;

    mCamera=mSceneMgr->getCamera("Camera");
    mCamera->setPosition(0,0,0);
    mCamera->lookAt(Vector3(0,0,-1));

    initBody();

    /*  Ogre::SceneNode* panode = mSceneMgr->getRootSceneNode()->createChildSceneNode(Vector3(0,0,0));
      Ogre::ParticleSystem* ps=mSceneMgr->createParticleSystem("Smoke", "Examples/Smoke");
      panode->attachObject(ps);
      mSceneMgr->getParticleSystem("Smoke")->setRenderQueueGroup(91);*/
}

Player::~Player ()
{
    delete shaker;
}

void Player::enableControl(bool enable)
{
    inControl = enable;
    inMoveControl = enable;

    if(!enable)
        stopMoving();
}

void Player::enableMovement(bool enable)
{
    inMoveControl = enable;

    if(!enable)
        stopMoving();
}

void Player::move_callback(OgreNewt::Body* me, float timeStep, int threadIndex )
{
    me->addForce(Ogre::Vector3(0,-10.0f,0));
    me->addForce(forceDirection);
}
void Player::climb_callback(OgreNewt::Body* me, float timeStep, int threadIndex )
{
    me->setVelocity(climbDir-climb_normal);
}
void Player::move_callback_nothing(OgreNewt::Body* me, float timeStep, int threadIndex )
{
    //me->addForce(Ogre::Vector3(0,0,0));
}
void Player::default_callback(OgreNewt::Body* me, float timeStep, int threadIndex )
{
    me->addForce(Ogre::Vector3(0,-6,0));
}
void Player::grabbed_callback(OgreNewt::Body* obj, float timeStep, int threadIndex )
{
    Vector3 p;
    p=obj->getOgreNode()->_getDerivedPosition();
    Vector3 p2;
    p2=necknode->_getDerivedPosition()+mCamera->getDerivedOrientation()*Vector3(0,0,-3);

    Vector3 s=(p2-p)*10;

    Vector3 o=obj->getVelocity();
    Real rad = s.angleBetween(o).valueDegrees();

    if(rad>45)
    {
        obj->setVelocity(obj->getVelocity()/2);
        s*=rad/40;
    }
    obj->setForce(s);
    obj->setPositionOrientation(obj->getPosition(),necknode->getOrientation());

    //release if too far away
    if(s.squaredLength()>2025)
    {
        Gbody->setMaterialGroupID(m_World->getDefaultMaterialID());
        //Gbody->setMassMatrix(Gbody->getMass(),Gbody->getInertia()*20);
        Gbody->setCustomForceAndTorqueCallback<Player>(&Player::default_callback, this);
        Gbody->setAngularDamping(gADT);
        Gbody->setLinearDamping(gLDT);
        body->setMassMatrix(body->getMass()-Gbody->getMass(),body->getInertia());
        grabbed=false;
    }
}

void Player::pressedKey(const OIS::KeyEvent &arg)
{
    for (auto l : listeners)
    {
        l->pressedKey(arg);
    }

    if(!inMoveControl)
        return;

    switch (arg.key)
    {
    case OIS::KC_D:
        vpravo=true;
        break;
    case OIS::KC_A:
        vlavo=true;
        break;
    case OIS::KC_W:
        vpred=true;
        break;
    case OIS::KC_S:
        vzad=true;
        break;

    case OIS::KC_C:
        setCrouch(1);
        break;

    case OIS::KC_SPACE:
    case OIS::KC_Q:
        jump();
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
        vpravo=false;
        break;
    case OIS::KC_A:
        vlavo=false;
        break;
    case OIS::KC_W:
        vpred=false;
        break;
    case OIS::KC_S:
        vzad=false;
        break;

    case OIS::KC_C:
        if(getCrouch()==1) setCrouch(2);
        break;
    }
}

void Player::pressedMouse(const OIS::MouseEvent &arg,OIS::MouseButtonID id)
{

    switch (id)
    {
    case OIS::MB_Right:
        if (!grabbed)
            tryToGrab();

        break;
    }
}
void Player::releasedMouse(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
    switch (id)
    {
    case OIS::MB_Right:
        if (grabbed)
        {
            Gbody->setMaterialGroupID(m_World->getDefaultMaterialID());
            //Gbody->setMassMatrix(Gbody->getMass(),Gbody->getInertia()*20);
            Gbody->setCustomForceAndTorqueCallback<Player>(&Player::default_callback, this);
            Gbody->setAngularDamping(gADT);
            Gbody->setLinearDamping(gLDT);
            body->setMassMatrix(body->getMass()-Gbody->getMass(),body->getInertia());
            grabbed=false;
        }
        break;
    }
}
void Player::movedMouse(const OIS::MouseEvent &e)
{
    if (rolling<=0)
        mouseX = (int)(-1 * e.state.X.rel*Global::timestep);

    int mouseY = (int)(-1 * e.state.Y.rel*Global::timestep);

    rotateCamera(mouseX/10.0f,mouseY/10.0f);
}

void Player::addListener(PlayerListener* l)
{
    listeners.push_back(l);
}

void Player::removeListener(PlayerListener* l)
{
    auto it = std::find(listeners.begin(), listeners.end(), l);

    if (it != listeners.end())
        listeners.erase(it);
}

void Player::setCrouch(char b)
{
    if(visi)
    {
        delete climbJoint;
        visi=false;
        noClimbTimer=1;
    }
    else if(is_climbing)
    {
        stopClimbing();
        noClimbTimer=1;
    }
    else if (rolling<=0)
        crouch=b;
}

void Player::walkingSound(Ogre::Real time)
{
    walkSoundTimer+=(time*bodyVelocity/6.6f);

    if(walkSoundTimer>0.4)
    {
        Vector3 ppos=body->getPosition();

        Global::audioLib->playWalkingSound(ppos.x, ppos.y - 2, ppos.z, groundID);

        walkSoundTimer=0;
    }
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
        body->setOmega(v*7);
    else
        body->setOmega(Ogre::Vector3(7,0,0));
    enableControl(false);

    alive=false;
}


Ogre::Vector3 Player::getFacingDirection()
{
    return mCamera->getDerivedOrientation()*Ogre::Vector3(0,0,-1);
}

Ogre::SceneNode* Player::detachHead()
{
    mSceneMgr->getSceneNode("CenterNode")->removeChild(necknode);
    //necknode->removeChild(headnode);

    return necknode;
}

void Player::attachHead(Ogre::SceneNode* headNode)
{
    if (headNode == nullptr)
    {
        headNode = necknode;
        necknode->setPosition(0, 1, 0);
    }

    // necknode->addChild(headNode);
    mSceneMgr->getSceneNode("CenterNode")->addChild(necknode);
}

void Player::attachCamera(Ogre::Camera* cam)
{
    camPitch = 0;
    fallPitch=0;
    crouch=0;
    crouch_am=0;
    cam_walking=0;
    head_turning=0;
    mouseX=0;
    camPitch=0;
    lastSpeed=0;
    stoji=true;

    necknode->setOrientation(Ogre::Quaternion::IDENTITY);
    //necknode->setPosition(Vector3(0,1,0));

    headnode->setOrientation(Ogre::Quaternion::IDENTITY);
    //headnode->setPosition(Vector3(0,0,0));

    shakeNode->setOrientation(Ogre::Quaternion::IDENTITY);
    //shakeNode->setPosition(Vector3(0,0,0));

    camnode->setOrientation(Ogre::Quaternion::IDENTITY);
    //camnode->setPosition(Vector3(0,0,0));

    mCamera = cam;

    Ogre::Quaternion q = cam->getDerivedOrientation();

    mCamera->detachFromParent();
    mCamera->setDirection(Ogre::Vector3(0,0,-1));
    camnode->attachObject(mCamera);
    rotateCamera(Ogre::Degree(q.getYaw()).valueDegrees(),Ogre::Degree(q.getPitch()).valueDegrees());
}

void Player::rotateCamera(Real hybX,Real hybY)
{
    if(!inControl)
        return;

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

    if(!is_climbing)
    {
        //damping of turning speed if moving quickly midair
        if (!onGround && bodyVelocity>10)
            hybX *= std::max(0.f, (100-bodyVelocity)/90.f);

        necknode->yaw(Degree(hybX), Node::TS_WORLD);
    }
    else
    {
        Vector3 camDir = getFacingDirection();
        camDir.y=0;
        camDir.normalise();
        Real angle=climb_normal.angleBetween(camDir).valueDegrees();

        Real climbCam= climb_normal.getRotationTo(camDir).getYaw().valueDegrees();

        if((climbCam>0 && hybX>0) || (climbCam<0 && hybX<0))
        {
            necknode->yaw(Degree(hybX), Node::TS_WORLD);
        }
        else
        {
            Real max_angle=90;
            if(is_climbing) max_angle=120;

            Real spomal;
            angle-=(180-max_angle);
            if(angle<0) spomal=0;
            else spomal=angle/max_angle;
            necknode->yaw(Degree(hybX* spomal), Node::TS_WORLD);
        }
    }
}

void Player::update(Real time)
{
    bodyVelocity = body->getVelocity().length();
    time*=Global::timestep;
    tslf=time;

    updateStats();

    if(!alive) return;

    forceDirection=Vector3::ZERO;

    if (!is_climbing && rolling<=0)
    {
        if (!vpravo && !vpred && !vzad && !vlavo)
        {
            body->setMaterialGroupID(wmaterials->stoji_mat);
            stoji = true;
            walkSoundTimer = 0.37;
        }
        else
        {
            updateMovement();
        }
    }
    else if (rolling>0)
    {
        body->setMaterialGroupID(wmaterials->ide_mat);
        stoji = false;
        walkSoundTimer = 0.2f;

        auto dirVec = necknode->_getDerivedOrientation()*Vector3(0, 0, -1);
        dirVec.y = 0;
        dirVec.normalise();
        forceDirection += dirVec * 10 * rolling;

        rolling -= tslf;
    }

    //making pullup
    if(climb_pullup)
    {
        updatePullup();
    }
    else if(is_climbing)
    {
        updateClimbMovement();
    }

    if (!stoji && onGround)
    {
        if (movespeed < 17)
            movespeed += time * 10;
        else
            movespeed = 17;

        walkingSound(time);
    }
    else movespeed = 7;

    updateHead(time);
}

void Player::updateMotionBlur()
{
    //visual fall dmg
    if (*ppFall > 0 && alive)
    {
        *ppFall -= tslf*2.5f;
        if (*ppFall < 0) *ppFall = 0;
    }

    float interpolationFactor = mPreviousFPS*0.03f*(*ppMotionBlur);

    Ogre::Quaternion estimatedOrientation = Ogre::Quaternion::nlerp(interpolationFactor, mCamera->getDerivedOrientation(), prevOr);
    Ogre::Vector3    estimatedPosition = (1 - interpolationFactor)*mCamera->getDerivedPosition() + interpolationFactor*prevPos;
    Ogre::Matrix4 viewMatrix = Ogre::Math::makeViewMatrix(estimatedPosition, estimatedOrientation);
    Ogre::Matrix4 projectionMatrix = mCamera->getProjectionMatrix();
    *pVP = projectionMatrix*viewMatrix;
    *iVP = (projectionMatrix*mCamera->getViewMatrix()).inverse();

    mPreviousFPS = 1 / tslf;
    prevPos = mCamera->getDerivedPosition();
    prevOr = mCamera->getDerivedOrientation();
}

void Player::updateStats()
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

    updateMotionBlur();
    updateGroundStats();

    if(!onGround && !visi && !is_climbing && noClimbTimer<=0)
    {
        updateClimbingPossibility();
    }
    else if(is_climbing)
    {
        updateClimbingStats();
    }

    if(!grabbed && !is_climbing && !visi)
    {
        updateUseGui();
    }
}

void Player::startCameraShake(float time,float power,float impulse)
{
    shaker->startCameraShake(time,power,impulse);
}

void Player::tryToGrab()
{
    Vector3 p=necknode->_getDerivedPosition();
    OgreNewt::BasicRaycast ray(m_World,p,p+mCamera->getDerivedOrientation()*Vector3(0,0,-4)  ,true);
    OgreNewt::BasicRaycast::BasicRaycastInfo info = ray.getInfoAt(0);

    if (info.mBody)
    {
        //grabbable
        if (info.mBody->getType() == Grabbable)
        {
            Gbody=info.mBody;
            Gbody->setMaterialGroupID(wmaterials->playerIgnore_mat);
            Gbody->unFreeze();
            Gbody->setCustomForceAndTorqueCallback<Player>(&Player::grabbed_callback, this);
            //Gbody->setMassMatrix(Gbody->getMass(),Gbody->getInertia()/20);
            gADT=Gbody->getAngularDamping();
            gLDT=Gbody->getLinearDamping();
            body->setMassMatrix(body->getMass()+Gbody->getMass(),body->getInertia());
            Gbody->setAngularDamping(Vector3(1,1,1)*300);
            Gbody->setLinearDamping(300);
            grabbed=true;
        }

        //trigger
        if (info.mBody->getType() == Trigger)
        {
            Ogre::Any any = info.mBody->getUserData();

            if(!any.isEmpty())
            {
                bodyUserData* a0=Ogre::any_cast<bodyUserData*>(any);
                if (a0->enabledTrigger)
                    Global::mEventsMgr->activatePlayerTrigger(a0);
            }
        }
    }
}


Shaker::Shaker(Ogre::SceneNode* node)
{
    this->node = node;
}

Shaker::~Shaker()
{
}

float Shaker::doRoll(float duration, Ogre::SceneNode* rNode, Ogre::SceneNode* hNode)
{
    if (rollingLeft>0)
        return rollingLeft;

    heightNode = hNode;
    rollNode = rNode;
    rollingDuration = rollingLeft = duration;

    startCameraShake(duration, 0.4f, 0.6f);

    return duration;
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
