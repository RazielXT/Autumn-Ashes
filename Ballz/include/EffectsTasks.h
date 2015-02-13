#ifndef EFFR_H
#define EFFR_H

#include "stdafx.h"
#include "EventTask.h"
#include "PostProcessMgr.h"

class QuickScaryBlink : public EventTask
{
public:

    QuickScaryBlink();

    bool start();
    bool update(Ogre::Real tslf);

private:

    float timer;
    float totalTime, effectStr;
    bool doneBlink;
    PostProcessMgr* mgr;
};

#endif