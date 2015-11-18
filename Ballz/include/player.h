#ifndef PLAYER_H
#define PLAYER_H

#include "stdafx.h"
#include "SlidesAutoTarget.h"
#include "PlayerStateInfo.h"
#include "PlayerPostProcess.h"
#include "PlayerClimbing.h"
#include "PlayerGrab.h"
#include "PlayerParkour.h"
#include "CameraShaker.h"
#include "PlayerTransform.h"

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
    friend class PlayerPostProcess;
    friend class PlayerClimbing;
    friend class PlayerGrab;
    friend class PlayerParkour;
    friend class PlayerSwimming;
    friend class PlayerAbilities;
    friend class PlayerSliding;
    friend class PlayerTransform;

    struct CamArrivalInfo
    {
        Ogre::SceneNode* tempNode = nullptr;
        Ogre::Vector3 pos;
        Ogre::Quaternion dir;
        float timer = 0;
    };

public:

    Player(WorldMaterials* wMaterials);
    ~Player ();

    Ogre::Vector3 gravity;
    void move_callback(OgreNewt::Body* me, float timeStep, int threadIndex );
    void move_callback_nothing(OgreNewt::Body* me, float timeStep, int threadIndex );
    void default_callback(OgreNewt::Body* me, float timeStep, int threadIndex );

    void update(Ogre::Real time);

    void pressedKey(const OIS::KeyEvent &arg);
    void releasedKey(const OIS::KeyEvent &arg);
    void pressedMouse(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
    void releasedMouse(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
    void movedMouse(const OIS::MouseEvent &e);

    void setPosition(Ogre::Vector3 pos)
    {
        bodyPosition = pos;
        body->setPositionOrientation(pos,body->getOrientation());
    }

    void saveState(PlayerStateInfo& info);
    void loadState(PlayerStateInfo& info);

    void die();
    void enableControl(bool enable);
    void enableMovement(bool enable);

    /*	power-0.1 weak, 0.5 strong;
    impulse-0.1 slow,0.4 strong  */
    void startCameraShake(float time,float power,float impulse);
    Vector3 getCameraPosition() const;

    OgreNewt::Body* body = nullptr;

    void stopMoving()
    {
        body->setVelocity(Ogre::Vector3(0, 0, 0));
    };

    Ogre::Camera* detachCamera();
    void attachCamera(bool silent = false);
    void attachCameraWithTransition(float duration, Ogre::Quaternion targetOr);

    Ogre::Vector3 getFacingDirection() const;
    void rotateCamera(Ogre::Real hybX, Ogre::Real hybY);
    void rotateCamera(Ogre::Quaternion or);

    Ogre::SceneNode* detachHead();
    void attachHead(Ogre::SceneNode* headNode = nullptr);

    bool isInControl()
    {
        return inControl;
    }

    Ogre::Vector3 bodyPosition;
    float bodyVelocityL;

    PlayerPostProcess* pPostProcess;
    PlayerClimbing* pClimbing;
    PlayerGrab* pGrabbing;
    PlayerParkour* pParkour;
    PlayerSwimming* pSwimming;
    PlayerAbilities* pAbilities;
    PlayerSliding* pSliding;
    PlayerTransform* pTransform;

private:

    void updateUseGui();

    void updateStats();
    void updateMovement();
    void updateHead();
    void updateDirectionForce();
    void updateGroundStats();

    void jump();
    void manageFall();

    void initBody();
    void updateCameraArrival();

    inline void playWalkSound()
    {
        Global::audioLib->playWalkingSound(bodyPosition.x, bodyPosition.y - 2, bodyPosition.z, groundID, 0.3f);
    }

    Shaker* shaker;

    OgreNewt::World* m_World;
    Ogre::SceneManager * mSceneMgr;

    Ogre::Camera* mCamera;
    Ogre::SceneNode* camnode, *necknode, *headnode, *shakeNode;

    //physics
    OgreNewt::UpVector* uv;
    OgreNewt::UpVector* uv2;
    OgreNewt::ConvexCollisionPtr col_p;
    WorldMaterials* wmaterials;

    //basic state
    bool alive, immortal;
    bool moving, right_key, left_key, back_key, forw_key;
    bool onGround, sprinting, inControl, inMoveControl, ownsCamera;

    //extern state
    bool hanging, grabbedObj, wallrunning, sliding, transformed;
    char climbing;

    float camPitch, fallVelocity, bodySpeedAccum, startMoveBoost, movespeed, sprintmeter;
    char fallPitch, cameraWalkFinisher;
    int walkCycle = 0;

    float fallPitchTimer, cam_walking, head_turning;
    int groundID, mouseX;
    float tslf, slowingDown;
    Ogre::Vector3 forceDirection, gNormal;
    CamArrivalInfo cameraArrival;
    Ogre::Vector3 facingDir;
};

#endif