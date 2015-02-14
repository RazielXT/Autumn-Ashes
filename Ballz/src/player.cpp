#include "stdafx.h"
#include "player.h"
#include "baloon.h"
#include "PostProcessMgr.h"

using namespace Ogre;

Player::Player(WorldMaterials* wMaterials)
{
    timestep=1;
    tslf=0;
    bodySpeedAccum=0;
    slowingDown=1;
    acceleration=0;
    cameraWalkFinisher=0;
    fallPitchSize=0;
    Gbody=NULL;
    soundEngine = Global::soundEngine;
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
    gNormal=Vector3::ZERO;
    climbDir=Vector3::ZERO;
    climb_normal=Vector3::ZERO;
    climb_yaw=0;
    gNormal.y=1;
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
    leti=false;
    nazemi=false;
    onRope=false;
    camShaking=false;
    inControl=true;
    inMoveControl=true;
    immortal=false;
    alive=true;
    camPitch=0;
    lastSpeed=0;
    stoji_mat = wMaterials->stoji_mat;
    ide_mat = wMaterials->ide_mat;
    flag_mat = wMaterials->flag_mat;
    grab_mat = wMaterials->playerIgnore_mat;
    mCamera=mSceneMgr->getCamera("Camera");
    mCamera->setPosition(0,0,0);
    mCamera->lookAt(Vector3(0,0,-1));
    Ogre::Entity* ent = mSceneMgr->createEntity( "name", "play2.mesh" );
    SceneNode* node = mSceneMgr->getRootSceneNode()->createChildSceneNode( "CenterNode" );
    node->attachObject( ent );
    ent->setCastShadows( false );
    ent->setVisible(false);
    OgreNewt::ConvexCollisionPtr col = OgreNewt::ConvexCollisionPtr(new OgreNewt::CollisionPrimitives::ConvexHull(m_World,ent,1));
    pbody = new OgreNewt::Body( m_World, col );
    pbody->setContinuousCollisionMode(1);
    uv = new OgreNewt::UpVector(pbody, Vector3::UNIT_Y);
    uv2= new OgreNewt::UpVector(pbody, Vector3::UNIT_Z);

    Ogre::Vector3 inertia, offset;
    col->calculateInertialMatrix(inertia, offset);
#ifdef OGRENEWT_NO_COLLISION_SHAREDPTR
    //no longer need the collision shape object
    delete col;
#endif
    pbody->setMassMatrix( 0.5, Vector3(20,20,20));
    pbody->setCenterOfMass(offset);
    pbody->setContinuousCollisionMode(1);
    pbody->setPositionOrientation( Ogre::Vector3(0,100,0), Ogre::Quaternion::IDENTITY );
    pbody->setLinearDamping(0.8);
    pbody->attachNode( node );
    pbody->setAutoSleep(0);
    //body->setMaterialGroupID(pmat);
    pbody->setCustomForceAndTorqueCallback<Player>(&Player::move_callback, this);


    necknode = node->createChildSceneNode( "NeckNod" );
    necknode->setPosition(Vector3(0,1,0));

    headnode = necknode->createChildSceneNode( "HeadNod" );
    headnode->setPosition(Vector3(0,0,0));

    shakeNode = headnode->createChildSceneNode( "ShakeHeadNod" );
    shakeNode->setPosition(Vector3(0,0,0));
    shaker = new Shaker(shakeNode);

    camnode = shakeNode->createChildSceneNode( "CamNod" );
    camnode->attachObject(mCamera);
    camnode->setPosition(Vector3(0,0,0));

    /*  Ogre::SceneNode* panode = mSceneMgr->getRootSceneNode()->createChildSceneNode(Vector3(0,0,0));
      Ogre::ParticleSystem* ps=mSceneMgr->createParticleSystem("Smoke", "Examples/Smoke");
      panode->attachObject(ps);
      mSceneMgr->getParticleSystem("Smoke")->setRenderQueueGroup(91);*/

    ent = mSceneMgr->createEntity( "ent_podstava", "cone_p2.mesh" );
    col_p = OgreNewt::ConvexCollisionPtr(new OgreNewt::CollisionPrimitives::ConvexHull(m_World,ent,1));
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
void Player::grabbed_callback(OgreNewt::Body* me, float timeStep, int threadIndex )
{
    Vector3 p;
    p=me->getOgreNode()->_getDerivedPosition();
    Vector3 p2;
    p2=necknode->_getDerivedPosition()+mCamera->getDerivedOrientation()*Vector3(0,0,-3);

    Vector3 s=(p2-p)*10;

    Vector3 o=me->getVelocity();
    Radian r=s.angleBetween(o);
    Real rad=r.valueDegrees();

    if(rad>45)
    {
        me->setVelocity(me->getVelocity()/2);
        s*=rad/40;
    }
    me->setForce(s);
    //me->setTorque(Ogre::Vector3(0,10,0));
    me->setPositionOrientation(me->getPosition(),necknode->getOrientation());
    if(s.squaredLength()>2025)
    {
        Gbody->setMaterialGroupID(m_World->getDefaultMaterialID());
        //Gbody->setMassMatrix(Gbody->getMass(),Gbody->getInertia()*20);
        Gbody->setCustomForceAndTorqueCallback<Player>(&Player::default_callback, this);
        Gbody->setAngularDamping(gADT);
        Gbody->setLinearDamping(gLDT);
        pbody->setMassMatrix(pbody->getMass()-Gbody->getMass(),pbody->getInertia());
        grabbed=false;
    }
}

void Player::pressedKey(const OIS::KeyEvent &arg)
{

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
        skoc();
        break;

    case OIS::KC_G:
    {
        Ogre::LogManager::getSingleton().getLog("RuntimeEvents.log")->logMessage("Pbody: "+Ogre::StringConverter::toString(pbody->getPosition()),Ogre::LML_NORMAL);
        Ogre::LogManager::getSingleton().getLog("RuntimeEvents.log")->logMessage("Headnod: "+Ogre::StringConverter::toString(mSceneMgr->getSceneNode("HeadNod")->getPosition())+","+Ogre::StringConverter::toString(mSceneMgr->getSceneNode("HeadNod")->_getDerivedPosition()),Ogre::LML_NORMAL);
        Ogre::LogManager::getSingleton().getLog("RuntimeEvents.log")->logMessage("Necknod: "+Ogre::StringConverter::toString(mSceneMgr->getSceneNode("NeckNod")->getPosition())+","+Ogre::StringConverter::toString(mSceneMgr->getSceneNode("NeckNod")->_getDerivedPosition()),Ogre::LML_NORMAL);
        Ogre::LogManager::getSingleton().getLog("RuntimeEvents.log")->logMessage("Camnod: "+Ogre::StringConverter::toString(mSceneMgr->getSceneNode("CamNod")->getPosition())+","+Ogre::StringConverter::toString(mSceneMgr->getSceneNode("CamNod")->_getDerivedPosition()),Ogre::LML_NORMAL);
        Ogre::LogManager::getSingleton().getLog("RuntimeEvents.log")->logMessage("Camera: "+Ogre::StringConverter::toString(mSceneMgr->getCamera("Camera")->getPosition()),Ogre::LML_NORMAL);

        break;
    }
    default:
        break;
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

    default:
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
            pbody->setMassMatrix(pbody->getMass()-Gbody->getMass(),pbody->getInertia());
            grabbed=false;
        }
        break;
    }
}
void Player::movedMouse(const OIS::MouseEvent &e)
{
    mouseX = (int) (-1*e.state.X.rel*timestep);
    int mouseY = (int) (-1*e.state.Y.rel*timestep);
    rotateCamera(mouseX/10.0f,mouseY/10.0f);
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
    else
        crouch=b;
}

void Player::walkingSound(Ogre::Real time)
{
    walkSoundTimer+=(time*bodyVelocity/6.6f);

    if(walkSoundTimer>0.4)
    {
        Vector3 ppos=pbody->getPosition();

        Global::audioLib->playWalkingSound(ppos.x, ppos.y - 2, ppos.z, groundID);

        walkSoundTimer=0;
    }
}

void Player::skoc()
{
    if(climb_pullup)
        return;

    if(onRope)
    {
        delete climbJoint;
        onRope=false;
        noClimbTimer=0.35;

        Vector3 vel=pbody->getVelocity();
        pbody->setVelocity(vel+Vector3(0,9,0));
    }
    else if(visi)
    {
        delete climbJoint;

        //visi = false;
        OgreNewt::CollisionPtr col = pbody->getCollision();//OgreNewt::ConvexCollisionPtr(new OgreNewt::CollisionPrimitives::Box(GlobalPointer->mWorld,Ogre::Vector3(0.1,0.1,0.1),0));
        OgreNewt::Body* mHelpBody = new OgreNewt::Body( m_World, col );
        mHelpBody->setPositionOrientation( pbody->getPosition(), Ogre::Quaternion::IDENTITY );
        mHelpBody->setMassMatrix(0.5,Ogre::Vector3(20,20,20));
        mHelpBody->setCustomForceAndTorqueCallback<Player>(&Player::default_callback, this);
        mHelpBody->setMaterialGroupID((OgreNewt::MaterialID*)Global::globalData->find("MatFlag")->second);
        climbJoint = new OgreNewt::BallAndSocket(mHelpBody,Gbody, pbody->getPosition(),0 );


        climb_normal.normalise();
        float climb_yaw_change = climb_yaw - Gbody->getOrientation().getYaw().valueRadians();
        Ogre::Quaternion q(Ogre::Radian(-climb_yaw_change),Ogre::Vector3(0,1,0));
        climb_normal=q*climb_normal;
        climb_normal*=-1;
        pbody->setVelocity(pbody->getVelocity()+Vector3(0,15,0));
        inMoveControl = false;
        Gbody = mHelpBody;

        if(leti)
        {
            pbody->setCustomForceAndTorqueCallback<Player>(&Player::move_callback, this);
            leti=false;
        }

        noClimbTimer=0.5;
        climb_pullup=0.05;
    }
    else if(is_climbing==2 || is_climbing==6)
    {
        Vector3 pos=pbody->getPosition();
        irrklang::ISound* s = soundEngine->play3D(AudioLibrary::getPath("pullup.wav").c_str(), irrklang::vec3df(pos.x, pos.y + 2, pos.z), false, false, true, irrklang::ESM_AUTO_DETECT, true);
        s->setMaxDistance(5);
        s->setVolume(0.7);
        climb_pullup=0.05;

        if(timestep<1)
        {
            s->setPlaybackSpeed(timestep);
            s->getSoundEffectControl()->enableWavesReverbSoundEffect(0,-10*timestep,2600,0.5);
        }

        s->drop();
    }
    else if(is_climbing==5)
    {
        Vector3 camDir=mCamera->getDerivedOrientation()*Vector3(0,0,-1);
        Vector3 pohlad(-camDir);
        pohlad.y=0;
        pohlad.normalise();
        Real angle=abs(climb_normal.angleBetween(pohlad).valueDegrees());

        if(angle>50)
        {
            canClimb(3,true);

            noClimbTimer=0.1;
            stopClimbing();

            pbody->setVelocity(camDir*10+Vector3(0,1,0));
        }
    }
    else
        //if(nazemi)
    {
        Vector3 vel=pbody->getVelocity();
        pbody->setVelocity(vel+Vector3(0,9,0));
    }
}

void Player::die()
{
    delete uv;
    delete uv2;
    pbody->setLinearDamping(2);
    pbody->setAngularDamping(Ogre::Vector3(2));
    pbody->setMassMatrix(5,Ogre::Vector3(1,1,1));

    Ogre::Vector3 v = pbody->getVelocity();
    v.y = 0;
    v.normalise();
    if(v!=Ogre::Vector3::ZERO)
        pbody->setOmega(v*7);
    else
        pbody->setOmega(Ogre::Vector3(7,0,0));
    enableControl(false);

    alive=false;
}

void Player::manageFall()
{
    fallVelocity=abs(lastSpeed.length())*2;

    Vector3 ppos=pbody->getPosition();
    irrklang::ISound* music;

    if(fallVelocity>35)
    {
        if(!immortal && fallVelocity>55)
        {
            die();
        }

        *ppFall=std::min(fallVelocity/7.0f,8.0f);

        if(!fallPitch)
            fallVelocity=80;

        slowingDown=0;
        music = soundEngine->play3D(AudioLibrary::getPath("pad.wav").c_str(),irrklang::vec3df(ppos.x,ppos.y,ppos.z), false, false, true, irrklang::ESM_AUTO_DETECT, true);
        music->setMaxDistance(10);

        music->setPlaybackSpeed(timestep);

        if(timestep<1)
            music->getSoundEffectControl()->enableWavesReverbSoundEffect(0,-10*timestep,2600,0.5);

    }

    if(!fallPitch)
    {
        fallPitch=1;
        fallPitchTimer=0;
    }

    Global::audioLib->playFallSound(ppos.x, ppos.y - 2, ppos.z, groundID);
}

Ogre::Vector3 Player::getFacingDirection()
{
    return mCamera->getDerivedOrientation()*Ogre::Vector3(0,0,-1);
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
    camShaking=false;
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
        necknode->yaw(Degree(hybX), Node::TS_WORLD);
    }
    else
    {
        Vector3 pohlad=mCamera->getDerivedOrientation()*Vector3(0,0,-1);
        pohlad.y=0;
        pohlad.normalise();
        Real angle=climb_normal.angleBetween(pohlad).valueDegrees();

        Real climbCam= climb_normal.getRotationTo(pohlad).getYaw().valueDegrees();

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

void Player::updateHead(Real time)
{

    if(camShaking)
        camShaking=shaker->updateCameraShake(time);

    if(!stoji && nazemi)
    {
        if (movespeed<17)
            movespeed+=time*10;
        else
            movespeed=17;

        walkingSound(time);
    }
    else movespeed=7;

    if(noClimbTimer>0)
        noClimbTimer-=time;

    if(bodyVelocity<0.05)
        bodyVelocity = 0;
    if(bodySpeedAccum+0.05<bodyVelocity)
    {
        bodySpeedAccum+=time*3.5f;
        if(bodySpeedAccum>10)
            bodySpeedAccum=10;
    }
    if(bodySpeedAccum>bodyVelocity)
    {
        bodySpeedAccum-=time*1.5f;
        if(bodySpeedAccum<0)
            bodySpeedAccum=0;
    }

    mSceneMgr->setShadowColour(Ogre::ColourValue(bodySpeedAccum,bodyVelocity/3.0f));

    if(fallPitch==1)
    {
        fallPitchTimer+=time;
        if(fallPitchTimer>=0.1f)
        {
            fallPitch=2;
            necknode->pitch(Degree(fallVelocity*(0.1f-fallPitchTimer+time)*-3), Node::TS_LOCAL);
            fallPitchTimer=0.2f;
            fallPitchSize+=fallVelocity*(0.1f-fallPitchTimer+time)*-3;

        }
        else
        {
            necknode->pitch(Degree(fallVelocity*time*-3), Node::TS_LOCAL);
            fallPitchSize+=fallVelocity*time*-3;
        }
    }
    else if(fallPitch==2)
    {
        fallPitchTimer-=time;
        if(fallPitchTimer<=0)
        {
            fallPitch=0;
            necknode->pitch(Degree(fallVelocity*(fallPitchTimer+time)*1.5f), Node::TS_LOCAL);
            fallPitchSize+=fallVelocity*(fallPitchTimer+time)*1.5f;
        }
        else
        {
            necknode->pitch(Degree(fallVelocity*time*1.5f), Node::TS_LOCAL);
            fallPitchSize+=fallVelocity*time*1.5f;
        }
    }


    if(crouch>0)
    {
        if(crouch==1)
        {
            if(crouch_am>-1.5f) crouch_am-=time*8;
            if(crouch_am<-1.5f) crouch_am=-1.5f;
            necknode->setPosition(0,1+crouch_am,0);
        }
        if(crouch==2)
        {
            crouch_am+=time*8;
            if(crouch_am>0)
            {
                crouch_am=0;
                crouch=0;
            }
            necknode->setPosition(0,1+crouch_am,0);
        }
    }
    else
        //not crouching
    {

        //walking camera
        if(!is_climbing && !stoji && nazemi && (bodyVelocity>2))
        {
            cameraWalkFinisher=1;
            cam_walking+=time*bodyVelocity;
            camnode->setPosition(0,-1*abs(Ogre::Math::Sin(cam_walking))/7.0f,0);
            camnode->setOrientation(Quaternion(Degree(Ogre::Math::Sin(cam_walking))*time*(bodyVelocity+1)*4,Vector3(0,0,1)));
            //camnode->roll(Degree(Ogre::Math::Sin(cam_walking+Ogre::Math::PI/2))*time*9);
        }
        else if (cameraWalkFinisher)
        {
            if(nazemi) cam_walking+=time*8;
            else cam_walking+=time*10;
            Real value=Ogre::Math::Sin(cam_walking);

            if(cameraWalkFinisher==1)
            {
                if(value>0)
                    cameraWalkFinisher=2;
                else
                    cameraWalkFinisher=3;
            }


            if ((value>0 && cameraWalkFinisher==3) || (value<0 && cameraWalkFinisher==2))
            {
                camnode->setPosition(0,0,0);
                camnode->resetOrientation();
                cameraWalkFinisher=0;
                cam_walking=0;
            }
            else
            {
                camnode->setPosition(0,-1*abs(value)/10,0);
                camnode->setOrientation(Quaternion(Degree(Ogre::Math::Sin(cam_walking))*time*(bodyVelocity+1)*4,Vector3(0,0,1)));
                //camnode->roll(Degree(Ogre::Math::Sin(cam_walking+Ogre::Math::PI/2))*time*9);
            }
        }

        //roll camera a bit while turning
        if(nazemi && vpred && abs(mouseX)>5)
        {
            head_turning+=(bodyVelocity/9)*(mouseX-5)/250.0f;
            if(head_turning>8)head_turning=8;
            if(head_turning<-8)head_turning=-8;
            headnode->setOrientation(Quaternion(Ogre::Radian(head_turning/60),Vector3(0,0,1)));
        }
        else if(head_turning>0)
        {
            head_turning-=time*30;
            if(head_turning<0) head_turning=0;
            headnode->setOrientation(Quaternion(Ogre::Radian(head_turning/60),Vector3(0,0,1)));
        }
        else if(head_turning<0)
        {
            head_turning+=time*30;
            if(head_turning>0) head_turning=0;
            headnode->setOrientation(Quaternion(Ogre::Radian(head_turning/60),Vector3(0,0,1)));
        }


        mouseX=0;

    }

}

void Player::update(Real time)
{

    bodyVelocity = pbody->getVelocity().length();
    timestep = Global::timestep;
    time*=timestep;
    tslf=time;

    updateStats();

    if(!alive) return;

    if(slowingDown<1)
    {
        slowingDown+=(tslf/2);
        if(slowingDown>1) slowingDown=1;
    }

    if(acceleration)
    {
        acceleration-=(tslf*2);
        if(acceleration<0) acceleration=0;
    }

    forceDirection=Vector3::ZERO;

    if(!is_climbing)
        if (!vpravo && !vpred && !vzad && !vlavo)
        {
            pbody->setMaterialGroupID(stoji_mat);
            stoji=true;
            walkSoundTimer=0.37;
        }
        else
        {
            if(stoji)
                acceleration=1;

            pbody->setMaterialGroupID(ide_mat);
            stoji=false;

            if (!vlavo && !vpravo)
            {
                if (vpred)
                    forceDirection=mCamera->getDerivedOrientation()*Vector3(0,0,-1);
                else
                    forceDirection=mCamera->getDerivedOrientation()*Vector3(0,0,1);
            }
            else
            {
                if (vpravo)
                {
                    if (!vpred && !vzad)
                        forceDirection=mCamera->getDerivedOrientation()*Vector3(1,0,0);
                    else if (vpred)
                        forceDirection=mCamera->getDerivedOrientation()*Vector3(1,0,-1);
                    else
                        forceDirection=mCamera->getDerivedOrientation()*Vector3(1,0,1);
                }
                else
                {
                    if (!vpred && !vzad)
                        forceDirection=mCamera->getDerivedOrientation()*Vector3(-1,0,0);
                    else if (vpred)
                        forceDirection=mCamera->getDerivedOrientation()*Vector3(-1,0,-1);
                    else
                        forceDirection=mCamera->getDerivedOrientation()*Vector3(-1,0,1);
                }
            }

            forceDirection.y=0;
            forceDirection.normalise();

            if (nazemi)
            {
                Vector3 look=mCamera->getDerivedOrientation()*Vector3(0,0,-1);
                look.y=0;
                Vector3 vel=pbody->getVelocity();
                vel.y=0;

                if(gNormal.y>0.8f)
                    forceDirection+=(1-gNormal.y)*forceDirection*2;

                Real rad=look.angleBetween(vel).valueDegrees();

                if(rad>45 && vpred && !vzad && !vpravo && !vlavo)
                    forceDirection*=movespeed*rad/30;
                else
                {
                    //brzda pri velkej rychlosti z kopca
                    Real ebd=1.0f;
                    Real l=bodyVelocity;
                    if(l>9)
                    {
                        ebd=1/(l-8);
                    }
                    forceDirection*=movespeed*ebd*slowingDown;
                }

                if(gNormal.y>0.7f)
                {
                    Vector3 antiSlide=-gNormal;
                    antiSlide.y*=-1;
                    forceDirection+=antiSlide*20*(1-gNormal.y);
                }
                else
                {
                    forceDirection*=0.25f;
                }

                forceDirection+=forceDirection*acceleration;

                if(crouch>0) forceDirection/=3.0f;
            }

            else forceDirection*=3/(1+bodyVelocity);
        }


    //making pullup
    if(climb_pullup)
    {
        pbody->setMaterialGroupID(ide_mat);

        if(climb_pullup>0)
        {

            climb_pullup+=2.5f*tslf;
            if(climb_pullup>2.5f) climb_pullup=2.5f;
            if(visi) forceDirection = 5*climb_normal;
            climbDir=-climb_normal;
            //climbDir.y=0;
            //climbDir.normalise();

            Real v=1/climb_pullup;
            climbDir*=climb_pullup;
            climbDir.y=v*2;

            if(climb_pullup==2.5f)
            {
                climb_pullup=0;

                if(is_climbing)
                    stopClimbing();

                if(visi)
                {
                    visi = false;
                    delete climbJoint;
                    delete Gbody;
                    inMoveControl = true;
                }

                noClimbTimer=0;
            }
        }
        else
        {
            climb_pullup+=2*tslf;
            if(climb_pullup>=0) climb_pullup=0;

            Vector3 pos=pbody->getPosition();
            pos.y=pullupPos-1.25f;
            pbody->setPositionOrientation(pos,pbody->getOrientation());

        }
    }
    else if(is_climbing)
    {


        if (!vpravo && !vpred && !vzad && !vlavo && !climb_move_side && !climb_move_vert && !climb_pullup)
        {
            pbody->setMaterialGroupID(stoji_mat);
            stoji=true;
        }
        else
            stoji=false;


        //already on move to side
        if(climb_move_side)
        {
            pbody->setMaterialGroupID(ide_mat);

            Vector3 dir=pbody->getVelocity();
            dir.y=0;
            pbody->setVelocity(dir);

            //right
            if(climb_move_side>0)
            {
                if(canClimb(1))
                {
                    climbDir=climb_normal;
                    climbDir.y=0;
                    climbDir.normalise();
                    Real temp(climbDir.x);
                    climbDir.x=2*climbDir.z;
                    climbDir.z=-2*temp;
                }
                else
                {
                    climbDir=Ogre::Vector3::ZERO;
                    pbody->setMaterialGroupID(stoji_mat);
                }

                climb_move_side-=tslf*3;
                if(climb_move_side<=0)
                {
                    climb_move_side=0;
                    pbody->setMaterialGroupID(stoji_mat);
                }
                camnode->setPosition((1-abs(climb_move_side-1))/2.0f,0,0);
                headnode->setOrientation(Quaternion(Ogre::Radian((1-abs(climb_move_side-1))/20.0f),Vector3(1,0,0)));
            }
            else
                //left
                if(climb_move_side<0)
                {
                    if(canClimb(0))
                    {
                        climbDir=climb_normal;
                        climbDir.y=0;
                        climbDir.normalise();
                        Real temp(climbDir.x);
                        climbDir.x=-2*climbDir.z;
                        climbDir.z=2*temp;
                    }
                    else
                    {
                        climbDir=Ogre::Vector3::ZERO;
                        pbody->setMaterialGroupID(stoji_mat);
                    }

                    climb_move_side+=tslf*3;
                    if(climb_move_side>=0)
                    {
                        climb_move_side=0;
                        pbody->setMaterialGroupID(stoji_mat);
                    }
                    camnode->setPosition((abs(climb_move_side+1)-1)/2,0,0);
                    headnode->setOrientation(Quaternion(Ogre::Radian((1-abs(-climb_move_side-1))/20),Vector3(1,0,0)));
                }
        }
        else
            //already on move vertically
            if(climb_move_vert)
            {
                pbody->setMaterialGroupID(ide_mat);
                //Vector3 dir=pbody->getVelocity();
                //dir.x=0;
                //dir.z=0;
                //pbody->setVelocity(dir);

                if(climbDir.y>=0)
                {
                    if(!canClimb(2))
                    {
                        climbDir=Vector3::ZERO;
                        pbody->setMaterialGroupID(stoji_mat);
                    }
                }
                else
                {
                    if(!canClimb(3))
                    {
                        climbDir=Vector3::ZERO;
                        pbody->setMaterialGroupID(stoji_mat);
                    }
                    else
                    {
                        climbDir=-climb_normal;
                        float slope = abs(climbDir.y);
                        climbDir.y = 0;
                        climbDir.normalise();
                        climbDir*=slope;
                        climbDir+=Vector3(0,-2,0);
                        climbDir.normalise();
                        climbDir*=2;
                    }
                }

                //right phase
                if(climb_move_vert>0)
                {
                    climb_move_vert-=tslf*4;
                    if(climb_move_vert<=0)
                    {
                        if(vpred)
                        {
                            Vector3 pohlad=mCamera->getDerivedOrientation()*Vector3(0,0,-1);
                            bool con=false;

                            if(pohlad.y>0)
                            {
                                if(canClimb(2,true))
                                {
                                    climbDir=Vector3(0,1.5,0);
                                    pbody->setMaterialGroupID(ide_mat);
                                    con=true;
                                }
                            }
                            else
                            {
                                if(canClimb(3,true))
                                {
                                    climbDir=Vector3(0,-1.5,0);
                                    pbody->setMaterialGroupID(ide_mat);
                                    con=true;
                                }
                            }

                            if(con)
                            {
                                climb_move_vert=-2-climb_move_vert;
                                camnode->setOrientation(Quaternion(Ogre::Radian((abs(climb_move_vert+1)-1)/20),Vector3(-0.5,0,1)));
                            }
                            else pbody->setMaterialGroupID(stoji_mat);
                        }
                        else
                        {
                            climb_move_vert=0;
                            camnode->setOrientation(Quaternion(Ogre::Radian((1-abs(climb_move_vert-1))/20),Vector3(0.5,0,1)));
                            pbody->setMaterialGroupID(stoji_mat);
                        }
                    }
                    else
                        camnode->setOrientation(Quaternion(Ogre::Radian((1-abs(climb_move_vert-1))/20),Vector3(-0.5,0,1)));
                }
                else
                    //left phase
                    if(climb_move_vert<0)
                    {
                        climb_move_vert+=tslf*4;
                        if(climb_move_vert>=0)
                        {
                            if(vpred)
                            {
                                Vector3 pohlad=mCamera->getDerivedOrientation()*Vector3(0,0,-1);
                                bool con=false;

                                if(pohlad.y>0)
                                {
                                    if(canClimb(2,true,false,true))
                                    {
                                        climbDir=Vector3(0,1.5,0);
                                        pbody->setMaterialGroupID(ide_mat);
                                        con=true;
                                    }
                                }
                                else
                                {
                                    if(canClimb(3,true,false,true))
                                    {
                                        climbDir=Vector3(0,-1.5,0);
                                        pbody->setMaterialGroupID(ide_mat);
                                        con=true;
                                    }
                                }

                                if(con)
                                {
                                    climb_move_vert=2-climb_move_vert;
                                    camnode->setOrientation(Quaternion(Ogre::Radian((1-abs(climb_move_vert-1))/20),Vector3(0.5,0,1)));
                                }
                                else pbody->setMaterialGroupID(stoji_mat);
                            }
                            else
                            {
                                camnode->setOrientation(Quaternion(Ogre::Radian((abs(climb_move_vert+1)-1)/20),Vector3(-0.5,0,1)));
                                climb_move_vert=0;
                                pbody->setMaterialGroupID(stoji_mat);
                            }
                        }
                        else
                            camnode->setOrientation(Quaternion(Ogre::Radian((abs(climb_move_vert+1)-1)/20),Vector3(0.5,0,1)));
                    }
            }
            else
                //not moving yet
            {
                if(vpred)
                {
                    Vector3 pohlad=mCamera->getDerivedOrientation()*Vector3(0,0,-1);

                    if(pohlad.y>=0)
                    {
                        if(canClimb(2, true))
                        {
                            climbDir=Vector3(0,2,0);
                            climb_move_vert=2;
                            pbody->setMaterialGroupID(ide_mat);
                        }
                        else
                            pbody->setMaterialGroupID(stoji_mat);
                    }
                    else
                    {
                        if(canClimb(3, true))
                        {
                            climbDir=-climb_normal;
                            float slope = abs(climbDir.y);
                            climbDir.y = 0;
                            climbDir.normalise();
                            climbDir*=slope;
                            climbDir+=Vector3(0,-2,0);
                            climbDir.normalise();
                            climbDir*=2;
                            climb_move_vert=-2;
                            pbody->setMaterialGroupID(ide_mat);
                        }
                        else
                            pbody->setMaterialGroupID(stoji_mat);
                    }
                }
                else if(vlavo)
                {
                    if(canClimb(0, true, true))
                    {
                        pbody->setMaterialGroupID(ide_mat);
                        climbDir=climb_normal;
                        climbDir.y=0;
                        climbDir.normalise();
                        Real temp(climbDir.x);
                        climbDir.x=-2*climbDir.z;
                        climbDir.z=2*temp;

                        Vector3 movement=pbody->getVelocity();
                        movement.y=0;

                        if(!climb_move_side && movement.squaredLength()>0.01f)
                            climb_move_side=-2;
                    }
                    else
                        pbody->setMaterialGroupID(stoji_mat);
                }
                else if(vpravo)
                {
                    if(canClimb(1, true, true))
                    {
                        pbody->setMaterialGroupID(ide_mat);
                        climbDir=climb_normal;
                        climbDir.y=0;
                        climbDir.normalise();
                        Real temp(climbDir.x);
                        climbDir.x=2*climbDir.z;
                        climbDir.z=-2*temp;

                        Vector3 movement=pbody->getVelocity();
                        movement.y=0;

                        if(!climb_move_side && movement.squaredLength()>0.01f)
                            climb_move_side=2;
                    }
                    else
                        pbody->setMaterialGroupID(stoji_mat);

                }
                else
                    climbDir=Vector3::ZERO;
            }

        //turning sides
        if(climb_yaw)
        {
            if(climb_yaw<0)
            {
                if((climb_yaw+tslf)>=0)
                {
                    necknode->yaw(Ogre::Radian(climb_yaw), Node::TS_WORLD);
                    climb_yaw=0;
                }
                else
                {
                    necknode->yaw(Ogre::Radian(-tslf), Node::TS_WORLD);
                    climb_yaw+=tslf;
                }
            }
            else
            {
                if((climb_yaw-tslf)<=0)
                {
                    necknode->yaw(Ogre::Radian(climb_yaw), Node::TS_WORLD);
                    climb_yaw=0;
                }
                else
                {
                    necknode->yaw(Ogre::Radian(tslf), Node::TS_WORLD);
                    climb_yaw-=tslf;
                }
            }

        }
    }


    updateHead(time);

}



void Player::updateStats()
{

    //visual fall dmg
    if(*ppFall>0 && alive)
    {
        *ppFall-=tslf*2.5f;
        if(*ppFall<0) *ppFall=0;
    }

    float interpolationFactor = mPreviousFPS*0.03f*(*ppMotionBlur);

    Ogre::Quaternion estimatedOrientation = Ogre::Quaternion::nlerp(interpolationFactor,mCamera->getDerivedOrientation(),prevOr);
    Ogre::Vector3    estimatedPosition    = (1-interpolationFactor)*mCamera->getDerivedPosition()+interpolationFactor*prevPos;
    Ogre::Matrix4 viewMatrix =Ogre::Math::makeViewMatrix(estimatedPosition,estimatedOrientation);
    Ogre::Matrix4 projectionMatrix   = mCamera->getProjectionMatrix();
    *pVP = projectionMatrix*viewMatrix;
    *iVP = (projectionMatrix*mCamera->getViewMatrix()).inverse();

    mPreviousFPS=1/tslf;
    prevPos=mCamera->getDerivedPosition();
    prevOr=mCamera->getDerivedOrientation();


    Vector3 p=pbody->getPosition();

    OgreNewt::BasicRaycast ray( m_World,(p-Vector3(0,1.6,0)),(p-Vector3(0,2.6,0)) ,true);
    OgreNewt::BasicRaycast::BasicRaycastInfo info = ray.getInfoAt(0);

    if (info.mBody)
    {
        Ogre::Any any = info.mBody->getUserData();

        if(!any.isEmpty())
            groundID=any_cast<bodyUserData*>(any)->material;
        else
            groundID=3;

        if(!nazemi) manageFall();
        nazemi=true;
        gNormal=info.mNormal;
        pbody->setLinearDamping(4);
    }
    else
    {

        OgreNewt::BasicConvexcast rayc( m_World,col_p,(p-Vector3(0,2,0)),Ogre::Quaternion::IDENTITY,(p-Vector3(0,2.5,0)),1,1);
        OgreNewt::BasicConvexcast::ConvexcastContactInfo infoc = rayc.getInfoAt(0);
        if (infoc.mBody)
        {
            Ogre::Any any = infoc.mBody->getUserData();

            if(!any.isEmpty())
                groundID=any_cast<bodyUserData*>(any)->material;
            else
                groundID=3;


            if(!nazemi) manageFall();
            nazemi=true;
            gNormal=1;
            pbody->setLinearDamping(4);

        }
        else
        {
            groundID=-1;
            lastSpeed=pbody->getVelocity();
            nazemi=false;
            pbody->setLinearDamping(0.0);
            gNormal=0;
        }
    }


    p=necknode->_getDerivedPosition()+Vector3(0,0.25,0);
    Vector3 predsebou=mCamera->getDerivedOrientation()*Vector3(0,0,-1);
    predsebou.y=0;
    predsebou.normalise();
    predsebou*=2;


    if(!nazemi)
    {
        ray = OgreNewt::BasicRaycast( m_World,p,predsebou+p ,true);
        info = ray.getInfoAt(0);
        if (info.mBody)
        {
            if(!visi && noClimbTimer<=0)
            {
                if(info.mBody->getType()==4)
                {
                    Ogre::Any any = info.mBody->getOgreNode()->getUserAny();

                    any_cast<Baloon*>(any)->setTracker(track);
                    climbJoint = new OgreNewt::BallAndSocket(info.mBody, pbody, p,0 );
                    visi=true;
                    leti=true;
                    pbody->setCustomForceAndTorqueCallback<Player>(&Player::move_callback_nothing, this);
                }
            }

            if(info.mBody->getType()==7 && !onRope && noClimbTimer<=0.25)
            {
                climbJoint = new OgreNewt::BallAndSocket(info.mBody, pbody, p ,0);
                onRope=true;
            }
        }
    }

    if(!onRope && !nazemi && !visi && !is_climbing && noClimbTimer<=0)
    {
        ray = OgreNewt::BasicRaycast( m_World,p,p+predsebou/1.3f ,true);
        info = ray.getInfoAt(0);
        if (info.mBody)
        {
            if (info.mBody->getType()==8)
            {
                climbJoint = new OgreNewt::BallAndSocket(pbody, info.mBody,  pbody->getPosition()+Vector3(0,2,0) ,0);
                visi=true;
                Gbody = info.mBody;
                climb_normal=info.mNormal;
                climb_normal.y=0;
                climb_normal.normalise();
                climb_yaw = Gbody->getOrientation().getYaw().valueRadians();
            }
            else if (info.mBody->getType()==5 || info.mBody->getType()==2 || info.mBody->getType()==6)
            {
                climb_normal=info.mNormal.normalisedCopy();

                Ogre::Any any = info.mBody->getUserData();
                ObjectAudio* a=any_cast<bodyUserData*>(any)->sounds;
                if(a)
                {
                    int rand = (int) Ogre::Math::RangeRandom(0,a->highHitAudio.size()-0.01f);
                    String sound = a->highHitAudio.at(rand);
                    Vector3 pos = p + predsebou;
                    Global::soundEngine->play3D(sound.c_str(),irrklang::vec3df(pos.x,pos.y,pos.z), false, false, false, irrklang::ESM_AUTO_DETECT, false);
                }

                startClimbing(info.mBody->getType());

                if(info.mBody->getType()==2)
                {
                    pullupPos=info.getBody()->getPosition().y-0.15f;
                    Vector3 pos=pbody->getPosition();
                    pos.y=pullupPos-1.25f;
                    pbody->setPositionOrientation(pos,pbody->getOrientation());
                    climbDir=Vector3::ZERO;

                    climb_pullup=pbody->getPosition().y+0.25f-pullupPos;
                    if(climb_pullup>0) climb_pullup=0;
                }

            }
        }

    }
    else if(is_climbing)
    {
        ray = OgreNewt::BasicRaycast( m_World,p,p+climb_normal*-3 ,true);
        info = ray.getInfoAt(0);
        if (info.mBody)
        {
            if ((info.mBody->getType()==5 || info.mBody->getType()==2 || info.mBody->getType()==6)&& !nazemi)
            {
                is_climbing=info.mBody->getType();
                Ogre::Vector3 c_normal=info.mNormal.normalisedCopy();

                if(c_normal!=climb_normal)
                {
                    if(!climb_yaw && (climb_move_vert || climb_move_side))
                        climb_yaw=-c_normal.getRotationTo(climb_normal).getYaw().valueRadians();

                    climb_normal=c_normal;
                }
            }
            else
            {
                //if(!climb_pullup)
                //	stopClimbing();
            }
        }
    }


    if(!grabbed && !is_climbing && !visi)
    {
        p-=Vector3(0,0.25,0);
        OgreNewt::BasicRaycast ray(m_World,p,p+mCamera->getDerivedOrientation()*Vector3(0,0,-4)  ,true);
        OgreNewt::BasicRaycast::BasicRaycastInfo info = ray.getInfoAt(0);

        if (info.mBody)
        {
            //grabbable
            if (info.mBody->getType()==3)
            {
                ((GuiOverlay*)Global::globalData->find("Gui")->second)->showUseGui(0);
            }
            else
                //climbable
                if (info.mBody->getType()==2)
                {
                    ((GuiOverlay*)Global::globalData->find("Gui")->second)->showUseGui(2);
                }
                else
                    //trigger
                    if(info.mBody->getType()==9)
                    {
                        Ogre::Any any = info.mBody->getUserData();

                        if(!any.isEmpty())
                        {
                            bodyUserData* a0=Ogre::any_cast<bodyUserData*>(any);
                            if(a0->enabledTrigger)
                            {
                                ((GuiOverlay*)Global::globalData->find("Gui")->second)->showUseGui(1);
                            }
                        }
                    }
        }
    }
}

void Player::startClimbing(char type)
{
    Ogre::Vector3 size( 0.2, 0.2,0.2 );
    Ogre::Real mass = 0.3;
    Entity* ent;
    SceneNode* node;
    Ogre::String name("BodyChytac");

    ent = mSceneMgr->createEntity( name, "boxEL.mesh" );
    node = mSceneMgr->getRootSceneNode()->createChildSceneNode( name );
    node->attachObject( ent );
    node->setScale( size );
    ent->setCastShadows(true);
    ent->setMaterialName( "GreyWood" );
    ent->setVisible(false);

    OgreNewt::ConvexCollisionPtr col = OgreNewt::ConvexCollisionPtr(new OgreNewt::CollisionPrimitives::Box( m_World, size, 0 ));
    OgreNewt::Body* body = new OgreNewt::Body( m_World, col );

    Ogre::Vector3 inertia, offset;
    col->calculateInertialMatrix(inertia, offset);

#ifdef OGRENEWT_NO_COLLISION_SHAREDPTR
    delete col;
#endif

    body->setMassMatrix( mass, mass*inertia );
    body->setCenterOfMass(offset);
    body->attachNode( node );
    body->setMaterialGroupID(flag_mat);
    body->setCustomForceAndTorqueCallback<Player>(&Player::climb_callback, this);
    body->setPositionOrientation( pbody->getPosition()+Vector3(0,5,0), Ogre::Quaternion::IDENTITY );
    pbody->setCustomForceAndTorqueCallback<Player>(&Player::move_callback_nothing, this);
    climbJoint = new OgreNewt::BallAndSocket(body, pbody,  pbody->getPosition()+Vector3(0,5,0) ,0);
    pbody->setMaterialGroupID(stoji_mat);

    is_climbing=type;

    if(!camShaking)
    {
        if(type>2)
            startCameraShake(0.2f,0.3f,0.4f);
        else
            startCameraShake(0.15f,0.1f,0.15f);
    }

    if(bodySpeedAccum>5)
        bodySpeedAccum = 5;
}

void Player::stopClimbing()
{
    pbody->setCustomForceAndTorqueCallback<Player>(&Player::move_callback, this);
    OgreNewt::Body* b=climbJoint->getBody0();
    delete climbJoint;
    delete b;
    mSceneMgr->destroyEntity("BodyChytac");
    mSceneMgr->destroySceneNode("BodyChytac");
    is_climbing=0;
    climb_yaw=0;
}


void Player::startCameraShake(float time,float power,float impulse)
{
    camShaking = true;
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
        if (info.mBody->getType()==3)
        {
            Gbody=info.mBody;
            Gbody->setMaterialGroupID(grab_mat);
            Gbody->unFreeze();
            Gbody->setCustomForceAndTorqueCallback<Player>(&Player::grabbed_callback, this);
            //Gbody->setMassMatrix(Gbody->getMass(),Gbody->getInertia()/20);
            gADT=Gbody->getAngularDamping();
            gLDT=Gbody->getLinearDamping();
            pbody->setMassMatrix(pbody->getMass()+Gbody->getMass(),pbody->getInertia());
            Gbody->setAngularDamping(Vector3(1,1,1)*300);
            Gbody->setLinearDamping(300);
            grabbed=true;
        }

        //trigger
        if(info.mBody->getType()==9)
        {
            Ogre::Any any = info.mBody->getUserData();

            if(!any.isEmpty())
            {
                bodyUserData* a0=Ogre::any_cast<bodyUserData*>(any);
                if(a0->enabledTrigger)
                {
                    short a=a0->trigger->playerAction;

                    std::map<short,std::vector<EventTask*>>::iterator it=a0->trigger->tasks.find(a);
                    if(it!=a0->trigger->tasks.end())
                    {
                        for(unsigned int j=0; j<(*it).second.size(); j++)
                        {
                            if((*it).second.at(j)->taskDelay || (*it).second.at(j)->start())
                            {
                                Global::mEventsMgr->addCachedTask((*it).second.at(j));
                            }
                        }

                        if(a0->trigger->cooldown>0)
                            Global::mEventsMgr->cooldownTrigger(a0);
                    }
                }
            }
        }
    }
}

bool Player::canClimb(char direction, bool soundIfTrue, bool needSpeed, bool secondTerm)
{
    Vector3 off=climb_normal;

    //0-left,1-right,2-up,3-down
    switch (direction)
    {
    case 0:
    {
        Real temp(off.x);
        off.x = -off.z;
        off.z = temp;
        break;
    }
    case 1:
    {
        Real temp(off.x);
        off.x = off.z;
        off.z = -temp;
        break;
    }
    case 2:
    {
        off.y = 0;
        off.y = off.length();
        off.x = 0;
        off.z = 0;
        break;
    }
    case 3:
    {
        off.y = 0;
        off.y = -off.length();
        off.x = 0;
        off.z = 0;
        break;
    }
    };

    Vector3 targetPos=necknode->_getDerivedPosition()+Vector3(0,0.25,0)+off/3;
    //Ogre::LogManager::getSingleton().getLog("RuntimeEvents.log")->logMessage(Ogre::StringConverter::toString(smer),Ogre::LML_NORMAL);
    //Ogre::LogManager::getSingleton().getLog("RuntimeEvents.log")->logMessage(Ogre::StringConverter::toString(off),Ogre::LML_NORMAL);
    //Ogre::LogManager::getSingleton().getLog("RuntimeEvents.log")->logMessage(Ogre::StringConverter::toString(climb_normal),Ogre::LML_NORMAL);
    OgreNewt::BasicRaycast ray(m_World,targetPos,targetPos+climb_normal*-3 ,true);
    OgreNewt::BasicRaycast::BasicRaycastInfo info = ray.getInfoAt(0);

    if (info.mBody && (info.mBody->getType()==6 || info.mBody->getType()==5 || info.mBody->getType()==2))
    {
        if(soundIfTrue)
        {
            Vector3 movement=pbody->getVelocity();
            movement.y=0;

            if(!needSpeed || movement.squaredLength()>0.01f)
            {
                Ogre::Any any = info.mBody->getUserData();
                ObjectAudio* a=any_cast<bodyUserData*>(any)->sounds;
                if(a)
                    if(!secondTerm || a->climbDoubleFreq)
                    {
                        int rand = (int) Ogre::Math::RangeRandom(0,a->lowHitAudio.size()-0.01f);
                        String sound = a->lowHitAudio.at(rand);
                        Global::soundEngine->play3D(sound.c_str(),irrklang::vec3df(targetPos.x,targetPos.y,targetPos.z), false, false, false, irrklang::ESM_AUTO_DETECT, false);
                    }
            }
        }

        return true;
    }
    else
    {
        targetPos-=off/3;
        ray = OgreNewt::BasicRaycast(m_World,targetPos,targetPos+climb_normal*-3 ,true);
        info = ray.getInfoAt(0);

        if (info.mBody && (info.mBody->getType()==6 || info.mBody->getType()==5 || info.mBody->getType()==2))
        {
            targetPos+=off;
            ray = OgreNewt::BasicRaycast(m_World,targetPos,targetPos+climb_normal*-3 ,true);
            info = ray.getInfoAt(0);

            if (info.mBody && (info.mBody->getType()==6 || info.mBody->getType()==5 || info.mBody->getType()==2))
            {
                if(soundIfTrue)
                {
                    Vector3 movement=pbody->getVelocity();
                    movement.y=0;

                    if(!needSpeed || movement.squaredLength()>0.01f)
                    {
                        Ogre::Any any = info.mBody->getUserData();
                        ObjectAudio* a=any_cast<bodyUserData*>(any)->sounds;
                        if(a)
                            if(!secondTerm || a->climbDoubleFreq)
                            {
                                int rand = (int) Ogre::Math::RangeRandom(0,a->lowHitAudio.size()-0.01f);
                                String sound = a->lowHitAudio.at(rand);
                                Global::soundEngine->play3D(sound.c_str(),irrklang::vec3df(targetPos.x,targetPos.y,targetPos.z), false, false, false, irrklang::ESM_AUTO_DETECT, false);
                            }
                    }
                }

                return true;
            }
        }
    }

    return false;
}


Shaker::Shaker(Ogre::SceneNode* node)
{
    this->node = node;
}

Shaker::~Shaker()
{
}

bool Shaker::updateCameraShake(float time)
{
    camShakeTimer+=time;
    Ogre::Quaternion shakeOr = Ogre::Quaternion::Slerp(camShakeTimer/camShakeTimerEnd,camShakePrev,camShakeTarget);

    node->setOrientation(shakeOr);

    if(camShakeTimer>=camShakeTimerEnd)
    {
        if(!camShakeTimeLeft)
        {
            node->resetOrientation();
            return false;
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

    return true;
}

void Shaker::startCameraShake(float time,float power,float impulse)
{
    camShakeTimeLeft = time;
    camShakePower = power;
    camShakeImpulse = Ogre::Math::Clamp(0.5f - impulse,0.01f,0.5f);
    camShakeTimer = 0;
    camShakeTimerEnd = 0.01;

    camShakePrev = Ogre::Quaternion::IDENTITY;
    camShakeTarget = Ogre::Quaternion::IDENTITY;
}
