#pragma once
#include "stdafx.h"
#include "InputListener.h"

using namespace Ogre;

class ZipLine : public EventTask, public InputListener
{
public:

    ZipLine(const std::vector<Ogre::Vector3>& points);

    bool start();
    bool update(Ogre::Real tslf);

    virtual void pressedKey(const OIS::KeyEvent &arg);
    virtual void movedMouse(const OIS::MouseEvent &e);

protected:

    struct ZipLinePoint
    {
        Vector3 pos;
        Vector3 dir;
        float startOffset;
    };

    struct HeadTransitionState
    {
        Vector3 pos;
        Quaternion dir;
        float timer;
    };

	struct HeadState
	{
		float yaw;
		float pitch;
	};

private:

    std::vector<ZipLinePoint> zipLine;

    void initZipLine(const std::vector<Ogre::Vector3>& points);

    bool placePointOnLine(Vector3& point);

    void updateSlidingState(float time);
    void updateHeadArrival(float time);
    inline void updateSlidingCamera(float time);
    inline void updateTurningYaw(float time);

    inline void updateSlidingSpeed(float time);

    void attach();
    void release();

    bool firstYaw = true;
    Quaternion lastOr;
    float headRoll;

    float currentSpeed;
    float avgSpeed = 5;
    std::string name = "slide";
    bool active = false;
    SceneNode* tracker;
    SceneNode* head;
    SceneNode* base;
    AnimationState * mTrackerState = 0;
    bool loop = false;
    float unavailableTimer = 0;

    HeadTransitionState headArrival;
	HeadState headState;

    bool enablePlayerControl = false;
};