#ifndef PLAYER_H
#define PLAYER_H

#include "stdafx.h"
#include <irrKlang.h>
#include "EventTask.h"
#include "EventsManager.h"
#include "SlidesAutoTarget.h"

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
    void pressedKey(const OIS::KeyEvent &arg);
    void releasedKey(const OIS::KeyEvent &arg);
    void pressedMouse(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
    void releasedMouse(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
    void movedMouse(const OIS::MouseEvent &e);

    void setPosition(Ogre::Vector3 pos)
    {
        body->setPositionOrientation(pos,body->getOrientation());
    }

    float getParam()
    {
        return fallPitchTimer;
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
    float bodyVelocity;

    SlidesAutoTargetAsync* slidesAutoTarget;

protected:

    void attachToSlide(OgreNewt::Body* slideBody);

    void updateStats();
    void updateMotionBlur();
    void updateClimbingStats();
    void updateUseGui();
    void updateClimbingPossibility();
    void updateGroundStats();
	void updateDirectionForce();

	void updateParkourPossibility();

    void updateHead();
    void manageFall();

    void pressedC(char b);
    void startClimbing(char type);
    void stopClimbing();

    void walkingSound(Ogre::Real time);

    inline void updateVerticalClimb(bool leftPhase);
    void updateClimbMovement();
    void tryClimbToSide(Direction dir);
    bool canClimb(Direction direction, bool soundIfTrue = false, bool needSpeed = false, bool secondPhase = false);
    void tryToGrab();
    void updatePullup();
    void updateMovement();
    void jump();

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
    bool alive, immortal, stoji, vpravo, vlavo, vzad, vpred;
    bool onGround, visi, grabbed, inControl, inMoveControl;
    Ogre::Real camPitch,fallVelocity,bodySpeedAccum,startMoveBoost,noClimbTimer,movespeed,walkSoundTimer;
    float climb_yaw, climb_move_side, climb_move_vert, climb_pullup;
    char fallPitch,cameraWalkFinisher,is_climbing;
    Ogre::Real fallPitchSize,fallPitchTimer,cam_walking,head_turning,pullupPos,rolling;
    int groundID, mouseX;
    Ogre::Real tslf, slowingDown;
    Ogre::Vector3 forceDirection, gNormal, climbDir, lastSpeed;

    //climb helpers
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