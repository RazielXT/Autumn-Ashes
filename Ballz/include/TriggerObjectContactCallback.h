#include "stdafx.h"
#include "EventsManager.h"

class TriggerObjectContactCallback :
    public OgreNewt::ContactCallback
{
public:
    TriggerObjectContactCallback(EventsManager* eventMgr);
    ~TriggerObjectContactCallback(void);

    int onAABBOverlap( OgreNewt::Body* body0, OgreNewt::Body* body1, int threadIndex );

    void contactsProcess( OgreNewt::ContactJoint &contactJoint, Ogre::Real timeStep, int threadIndex );

private:

    EventsManager* mEventMgr;
};
