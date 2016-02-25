#include "stdafx.h"

class EventsManager;

class TriggerPlayerContactCallback :
	public OgreNewt::ContactCallback
{
public:
	TriggerPlayerContactCallback(EventsManager* eventMgr);
	~TriggerPlayerContactCallback(void);

	int onAABBOverlap( OgreNewt::Body* body0, OgreNewt::Body* body1, int threadIndex );

	void contactsProcess( OgreNewt::ContactJoint &contactJoint, Ogre::Real timeStep, int threadIndex );

private:

	EventsManager* mEventMgr;
};
