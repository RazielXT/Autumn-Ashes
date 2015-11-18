#pragma once
#include "stdafx.h"
#include "InputListener.h"
#include <future>

using namespace Ogre;

class SlidesAutoTargetAsync;

class Slide
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

    Vector3 getTrackPosition(float timeOffset);

    Ogre::Quaternion getDirectionState(float offset);
    Ogre::Quaternion getDirectionState();

    Ogre::TransformKeyFrame getCurrentState();

    struct SlidePoint
    {
        Vector3 pos;
        float startOffset;
    };

    std::vector<SlidePoint> slidePoints;
    bool bidirectional = false;

protected:

    NodeAnimationTrack* track;

    Slide(const std::string& zipName, bool looped, float speed) : animName(zipName), loop(looped), avgSpeed(speed) {};

    virtual void resetHead() {};

    void removeControlFromPlayer();
    void setCorrectDirection(float startOffset = -1);

    bool jumpingToSlide = false;

    void startJumpToSlide();
    void updateJumpToSlide(float time);

    struct HeadTransitionState
    {
        float dist;
        SceneNode* tempNode;
        Vector3 posTarget;
        Vector3 pos;
        Quaternion dir;
        Quaternion dirTarget;
        float timer;
    };

    struct HeadControlState
    {
        float yaw;
        float pitch;
    };

    void initSlide(const std::string& zipAnimName);
    void initSlide(const std::vector<Ogre::Vector3>& points);

    bool placePointOnLine(Vector3& point);

    void updateSlidingState(float time);
    void updateHeadArrival(float time);

    virtual void updateSlidingSpeed(float time);

    virtual void updateSlidingCamera(float time);

    void attach(bool retainDirection = false, float headArrivalTime = 1.0f);
    void release(bool returnControl = true, bool manualJump = false);

    float currentSpeed;
    float avgSpeed = 5;
    bool loop = false;

    void invertTrack();

    bool enablePlayerControl = false;
    bool sliding = false;
    bool sprint = false;
    float unavailableTimer = 0;

    std::string animName;

    AnimationState * mTrackerState = 0;
    SceneNode* tracker;
    SceneNode* head;
    SceneNode* base;

    HeadTransitionState headArrival;
    HeadControlState headState;

    float realSpeed;
    Vector3 lastPos;
};