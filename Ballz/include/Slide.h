#pragma once
#include "stdafx.h"
#include "InputListener.h"
#include <future>

using namespace Ogre;

class SlidesAutoTargetAsync;

class Slide : public EventTask, public InputListener
{

public:

    virtual ~Slide();

    virtual bool start();
    virtual bool start(Vector3& point);
    virtual bool update(Ogre::Real tslf);

    virtual void pressedKey(const OIS::KeyEvent &arg);
    virtual void movedMouse(const OIS::MouseEvent &e);

protected:

    Slide(const std::string& zipName, bool looped, float speed) : animName(zipName), loop(looped), avgSpeed(speed*5) {};

    virtual void resetHead() {};

    struct SlidePoint
    {
        Vector3 pos;
        Vector3 dir;
        float startOffset;
    };

    struct HeadTransitionState
    {
        float pitch;
        SceneNode* tempNode;
        Vector3 pos;
        Quaternion dir;
        float timer;
    };

    struct HeadControlState
    {
        float yaw;
        float pitch;
    };

    std::vector<SlidePoint> slidePoints;

    void initSlide(const std::string& zipAnimName);
    void initSlide(const std::vector<Ogre::Vector3>& points);

    bool placePointOnLine(Vector3& point);

    void updateSlidingState(float time);
    void updateHeadArrival(float time);

    SlidesAutoTargetAsync* slidesAutoTarget;
    inline void updateTargetSlide(float time);

    virtual void updateSlidingSpeed(float time);

    virtual void updateSlidingCamera(float time);

    void attach();
    void release(bool returnControl = true);

    float currentSpeed;
    float avgSpeed = 5;
    bool loop = false;

    bool enablePlayerControl = false;
    bool active = false;
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