#ifndef PLAYER_H
#define PLAYER_H

#include "stdafx.h"
#include <irrKlang.h>
#include "EventTask.h"
#include "EventsManager.h"
#include "SlidesAutoTarget.h"
#include "PlayerPostProcess.h"
#include "PlayerClimbing.h"
#include "PlayerGrab.h"

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

public:

    Player(WorldMaterials* wMaterials);
    ~Player ();

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
        body->setPositionOrientation(pos,body->getOrientation());
    }

    void die();
    void enableControl(bool enable);
    void enableMovement(bool enable);

    /*	power-0.1 weak, 0.5 strong;
    impulse-0.1 slow,0.4 strong  */
    void startCameraShake(float time,float power,float impulse);

    OgreNewt::Body* body = nullptr;

    void stopMoving()
    {
        vlavo = false;
        vpravo = false;
        vpred = false;
        vzad = false;
        body->setVelocity(Ogre::Vector3(0, 0, 0));
    };

    void attachCamera();
    void attachCameraWithTransition();

    inline Ogre::Vector3 getFacingDirection();
    void rotateCamera(Ogre::Real hybX, Ogre::Real hybY);

    Ogre::SceneNode* detachHead();
    void attachHead(Ogre::SceneNode* headNode = nullptr);

    Ogre::Vector3 bodyPosition;
    float bodyVelocityL;

    SlidesAutoTargetAsync* slidesAutoTarget;

	PlayerPostProcess* pPostProcess;
	PlayerClimbing* pClimbing;
	PlayerGrab* pGrabbing;

protected:
   
    void updateUseGui();

	void updateStats();
	void updateMovement();
	void updateHead();
    void updateDirectionForce();
	void updateGroundStats();
	void walkingSound(Ogre::Real time);

	void jump();
    void manageFall();


    void initBody();

    inline void updateHeadArrival();

private:

    struct
    {
        Ogre::SceneNode* tempNode = nullptr;
        Ogre::Vector3 pos;
        Ogre::Quaternion dir;
        float timer = 0;
    } cameraArrival;

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

    //state
	bool alive, immortal;
	bool stoji, vpravo, vlavo, vzad, vpred;
    bool onGround, inControl, inMoveControl;
	float camPitch, fallVelocity, bodySpeedAccum, startMoveBoost, movespeed, walkSoundTimer;
	char fallPitch, cameraWalkFinisher;

	bool hanging, grabbed; 
    char is_climbing;

	float fallPitchTimer, cam_walking, head_turning, rolling;
    int groundID, mouseX;
	float tslf, slowingDown;
    Ogre::Vector3 forceDirection, gNormal;

};

#endif