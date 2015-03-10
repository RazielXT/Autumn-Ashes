#pragma once
#include "stdafx.h"
#include "InputListener.h"

using namespace Ogre;

class ZipLine : public EventTask, public InputListener
{
public:

	ZipLine(SceneNode* node, const std::string& zipAnimName, bool looped, float speed);
	ZipLine(const std::vector<Ogre::Vector3>& points, const std::string& zipName, bool looped, float speed);

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

	struct HeadControlState
	{
		float yaw;
		float pitch;
	};

	struct TurnRollState
	{
		bool first = true;
		Quaternion lastOr;
		float torque;
		float curHeadRoll;
	};

    std::vector<ZipLinePoint> zipLine;

	void initZipLine();
    void initZipLine(const std::vector<Ogre::Vector3>& points);

    bool placePointOnLine(Vector3& point);

    void updateSlidingState(float time);
    void updateHeadArrival(float time);
    inline void updateSlidingCamera(float time);
    inline void updateTurningRoll(float time);

    inline void updateSlidingSpeed(float time);

    void attach();
    void release();


    float currentSpeed;
    float avgSpeed = 5;
	bool loop = false;

	bool enablePlayerControl = false;
	bool active = false;
	float unavailableTimer = 0;

    std::string name;
    
	AnimationState * mTrackerState = 0;
    SceneNode* tracker;
    SceneNode* head;
    SceneNode* base;

    HeadTransitionState headArrival;
	HeadControlState headState;
	TurnRollState turnRollState;
   
};