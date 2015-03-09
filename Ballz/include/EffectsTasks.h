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
    short id;
};


class SwitchColorSchemeFx : public EventTask
{
public:

    /*
    P,P,P,P
    r,g,b,time
    float,float,float,float
    */
    SwitchColorSchemeFx(Ogre::String info);

    bool start();
    bool update(float tslf);

private:

    OgreNewt::Body* mBody;
    Ogre::Vector3 colorTarget;
    Ogre::Vector3 colorBase;
    float fulltime;
    float timer;
};

#endif