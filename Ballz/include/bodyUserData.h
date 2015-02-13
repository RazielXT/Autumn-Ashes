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
