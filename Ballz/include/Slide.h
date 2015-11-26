#pragma once
#include "stdafx.h"
#include "InputListener.h"
#include "AnimatedTrack.h"
#include "HeadTransition.h"

using namespace Ogre;

class SlidesAutoTargetAsync;

class Slide : public AnimatedTrack
{

public:

    virtual ~Slide();

    virtual bool start(bool withJump = false);
    virtual bool start(Vector3& point, bool withJump = false);

    virtual bool start(float startOffset, bool withJump = false, float headArrivalTimer = 1.0f);
    void instantDetach(bool returnControl);

    virtual bool update(Ogre::Real tslf);

    virtual void pressedKey(const OIS::KeyEvent &arg);
    virtual void releasedKey(const OIS::KeyEvent &arg);
    virtual void movedMouse(const OIS::MouseEvent &e);

    bool bidirectional = false;

protected:

    Slide(const std::string& zipName, bool looped, bool isWalkable, float speed) : AnimatedTrack(zipName), loop(looped), walkable(isWalkable), avgSpeed(speed) {};

    virtual void resetHead() {};

    void removeControlFromPlayer();

    bool jumpingToSlide = false;

    void startJumpToSlide();
    void updateJumpToSlide(float time);

    struct HeadControlState
    {
        float yaw;
        float pitch;
    };

    void updateSlidingState(float time);
    void updateHeadArrival(float time);

    virtual void updateSlidingSpeed(float time);

    virtual void updateSlidingCamera(float time);

    void attach(bool retainDirection = false, float headArrivalTime = 1.0f);
    void release(bool returnControl = true, bool manualJump = false);

    float currentSpeed;
    float avgSpeed = 5;
    bool loop = false;
    bool walkable = false;

    bool enablePlayerControl = false;
    bool sliding = false;
    bool sprint = false;
    float unavailableTimer = 0;


    SceneNode* head;
    SceneNode* base;

    HeadTransition headArrival;
    HeadControlState headState;

    float realSpeed;
    Vector3 lastPos;
};