#ifndef BODYUSERDATA_H
#define BODYUSERDATA_H

#include "stdafx.h"
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
    BaloonT = 4,
    Climb = 5,
    Climb_Pullup = 6,
    Rope = 7,
    Dynamic_Pullup = 8,
    Trigger = 9,
    Bridge = 10,
    SelfIgnore = 11,
    PlayerIgnore = 12
};

struct bodyUserData
{
    TriggerInfo* trigger;
    ObjectAudio* sounds;
    short material;
    short actions[4];
    bool makeActions;
    bool enabledTrigger;
    void* customData;

    bodyUserData()
    {
        actions[0]=0;
        actions[1]=0;
        actions[2]=0;
        actions[3]=0;
        trigger=NULL;
        sounds=NULL;
        customData=NULL;
        material=0;
        makeActions=false;
        enabledTrigger=false;
    };

    ~bodyUserData()
    {
        if(trigger!=NULL)
            delete trigger;

        if(customData!=NULL)
            delete customData;
    };
};

#endif
