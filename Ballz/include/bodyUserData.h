#ifndef BODYUSERDATA_H
#define BODYUSERDATA_H

//#include "stdafx.h"
#include "TriggerInfo.h"

struct ObjectAudio
{
    std::vector<Ogre::String> lowHitAudio;
    std::vector<Ogre::String> highHitAudio;
    bool climbDoubleFreq;
};


enum BodyType
{
    Default = 0,
    Pullup_old = 2,
    Grabbable = 3,
    TopSlidePart = 4,
    Climb = 5,
    Climb_Pullup = 6,
    ZipLinePart = 7,
    Dynamic_Pullup = 8,
    Trigger = 9,
    Bridge = 10,
    SelfIgnore = 11,
    PlayerIgnore = 12
};

struct bodyUserData
{
    TriggerInfo* trigger = nullptr;
    ObjectAudio* sounds = nullptr;
    short material = 0;
    short actions[4];
    bool makeActions = false;
    bool enabledTrigger = false;
    void* customData = nullptr;

    bodyUserData()
    {
        actions[0] = 0;
    };

    ~bodyUserData()
    {
        if(trigger)
            delete trigger;

        if(customData)
            delete customData;
    };
};

#endif
