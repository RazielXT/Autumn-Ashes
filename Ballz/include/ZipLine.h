#pragma once
#include "stdafx.h"
#include "InputListener.h"

using namespace Ogre;

class ZipLine : public EventTask, public InputListener
{
public:

    struct LineProjState
    {
        Vector3 projPos;
        float sqMinDistance;
    };

    struct ZipLinePoint
    {
        Vector3 pos;
        Vector3 dir;
        float startOffset;
    };

    ZipLine(const std::vector<Ogre::Vector3>& points);

    bool start();
    bool update(Ogre::Real tslf);

    virtual void pressedKey(const OIS::KeyEvent &arg);
    virtual void movedMouse(const OIS::MouseEvent &e);

private:

    std::vector<ZipLinePoint> zipLine;

    void initZipLine(const std::vector<Ogre::Vector3>& points);

    bool placePointOnLine(Vector3& point);

    inline LineProjState getProjectedState(Ogre::Vector3& point, Ogre::Vector3& start, Ogre::Vector3& end);

    void updateSlidingState(float time);
    inline void updateSlidingCamera(float time);
    inline void updateSlidingSpeed(float time);

    void attach();
    void release();

    float currentSpeed;
    float avgSpeed = 5;
    std::string name = "slide";
    bool active = false;
    SceneNode* tracker;
    SceneNode* head;
    AnimationState * mTrackerState = 0;
    bool loop = false;
    float unavailableTimer = 0;

    bool enablePlayerControl = false;
};