#ifndef PLAYER_H
#define PLAYER_H

#include "stdafx.h"
#include <irrKlang.h>

#include "EventTask.h"
#include "EventsManager.h"

class Shaker
{
public:
    Shaker(Ogre::SceneNode* node);
    ~Shaker();
    void updateCameraShake(float time);
    void startCameraShake(float time,float power,float impulse);
    float doRoll(float duration, Ogre::SceneNode* rNode, Ogre::SceneNode* hNode);

private:

    float rollingLeft = 0, rollingDuration;
    Ogre::SceneNode* rollNode;
    Ogre::SceneNode* heightNode;

    bool camShaking = false;
    Ogre::SceneNode* node;
    float camShakeTimer,camShakeTimerEnd,camShakePower,camShakeImpulse,camShakeTimeLeft;
    Ogre::Quaternion camShakePrev,camShakeTarget;
};

class Player
{
    enum Direction
    {
        Left, Right, Up, Down
    };

public:

    Player(WorldMaterials* wMaterials);
    ~Player ();

    void move_callback(OgreNewt::Body* me, float timeStep, int threadIndex );
    void move_callback_nothing(OgreNewt::Body* me, float timeStep, int threadIndex );
    void grabbed_callback(OgreNewt::Body* me, float timeStep, int threadIndex );
    void default_callback(OgreNewt::Body* me, float timeStep, int threadIndex );
    void climb_callback(OgreNewt::Body* me, float timeStep, int threadIndex );

    void injectPostProcess(Ogre::Real* postPFall,Ogre::Matrix4* invVP,Ogre::Matrix4* prevVP, Ogre::Real* mbAm)
    {
        ppFall=postPFall;
        iVP=invVP;
        pVP=prevVP;
        ppMotionBlur=mbAm;
    }

    void update(Ogre::Real time);
    void updateStats();
    void updateHead(Ogre::Real time);
    void manageFall();
    void pressedKey(const OIS::KeyEvent &arg);
    void releasedKey(const OIS::KeyEvent &arg);
    void pressedMouse(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
    void releasedMouse(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
    void movedMouse(const OIS::MouseEvent &e);
    void rotateCamera(Ogre::Real hybX,Ogre::Real hybY);
    void setPosition(Ogre::Vector3 pos)
    {
        pbody->setPositionOrientation(pos,pbody->getOrientation());
    }
    void setCrouch(char b);
    void startClimbing(char type);
    void stopClimbing();
    void stopMoving()
    {
        vlavo=false;
        vpravo=false;
        vpred=false;
        vzad=false;
        pbody->setVelocity(Ogre::Vector3(0,0,0));
    };

    void walkingSound(Ogre::Real time);
    char getCrouch()
    {
        return crouch;
    }
    float getParam()
    {
        return fallPitchTimer;
    }
    OgreNewt::Body* getBody()
    {
        return pbody;
    }
    void skoc();
    void die();
    void enableControl(bool enable);
    void enableMovement(bool enable);
    void attachCamera(Ogre::Camera* cam);
    Ogre::Vector3 getFacingDirection();
    bool canClimb(Direction direction, bool soundIfTrue = false, bool needSpeed = false, bool secondPhase = false);
    Ogre::SceneNode* getCamNode()
    {
        return camnode;
    }
    void tryToGrab();
    /*	power-0.1 weak, 0.5 strong;
    impulse-0.1 slow,0.4 strong  */
    void startCameraShake(float time,float power,float impulse);

    inline void updateVerticalClimb(bool leftPhase);
    bool immortal;

private:

    void initBody();
    void tryClimbToSide(Direction dir);

    float timestep;
    Shaker* shaker;
    OgreNewt::World* m_World;
    Ogre::SceneManager * mSceneMgr;
    irrklang::ISoundEngine* soundEngine;

    Ogre::Camera* mCamera;
    Ogre::SceneNode* camnode, *necknode, *headnode, *shakeNode;

    //physics
    OgreNewt::Body* pbody;
    OgreNewt::UpVector* uv;
    OgreNewt::UpVector* uv2;
    OgreNewt::ConvexCollisionPtr col_p;
	WorldMaterials* wmaterials;

    //state
    bool alive, stoji, vpravo, vlavo, vzad, vpred, onGround, visi, grabbed, camShaking, inControl, inMoveControl;
    Ogre::Real camPitch,fallVelocity,bodyVelocity,bodySpeedAccum,startMoveBoost,crouch_am,noClimbTimer,movespeed,walkSoundTimer;
    float climb_yaw, climb_move_side, climb_move_vert, climb_pullup;
    char fallPitch,crouch,cameraWalkFinisher,is_climbing;
    Ogre::Real fallPitchSize,fallPitchTimer,cam_walking,head_turning,pullupPos,rolling;
    int groundID, mouseX;
    Ogre::Real tslf, slowingDown;
    Ogre::Vector3 forceDirection, gNormal, climbDir, lastSpeed;

    //movement helpers
    Ogre::Vector3 climb_normal;
    OgreNewt::BallAndSocket* climbJoint;

    //grabbed obj
    Ogre::Vector3 gADT;
    Ogre::Real gLDT;
    OgreNewt::Body* Gbody;

    //motion blur
    Ogre::Real* ppFall, *ppMotionBlur;
    Ogre::Matrix4* iVP,*pVP;
    Ogre::Matrix4 prevVP;
    Ogre::Vector3 prevPos;
    Ogre::Quaternion prevOr;
    Ogre::Real mPreviousFPS;
};

#endif