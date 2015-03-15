#pragma once

#include "stdafx.h"

class SlideContactCallback :
    public OgreNewt::ContactCallback
{
public:
    SlideContactCallback(EventsManager* mEventMgr);
    ~SlideContactCallback(void);

    int onAABBOverlap(OgreNewt::Body* body0, OgreNewt::Body* body1, int threadIndex);

    void contactsProcess(OgreNewt::ContactJoint &contactJoint, Ogre::Real timeStep, int threadIndex);

private:
    EventsManager* mEventMgr;
};
