#ifndef TREVENTR_H
#define TREVENTR_H

#include "stdafx.h"
#include <vector>
#include "EventTask.h"

class TriggerInfo
{
public:

    TriggerInfo(short pA,float cd)
    {
        cooldown=cd;
        timer=0;
        activationType=1;

        if(pA==0)
            pA=-1;

        playerAction=pA;

    };

    TriggerInfo()
    {
        cooldown=0;
        timer=0;
        activationType=1;
        playerAction=-1;
    };

    std::map<short,std::vector<EventTask*>> tasks;

    float timer;
    float cooldown;
    short playerAction;
    //0-switch,1-touch,2-volume
    char activationType;

};

#endif