#include "stdafx.h"
#include "TriggerObjectContactCallback.h"
#include "TriggerInfo.h"

TriggerObjectContactCallback::TriggerObjectContactCallback(EventsManager* eventMgr) : OgreNewt::ContactCallback()
{
	mEventMgr=eventMgr;
}

TriggerObjectContactCallback::~TriggerObjectContactCallback(void)
{
}

int TriggerObjectContactCallback::onAABBOverlap( OgreNewt::Body* body0, OgreNewt::Body* body1, int threadIndex )
{
	return 1;
}

inline void processTrigger(bodyUserData* a0, bodyUserData* a1, EventsManager* mEventMgr)
{
	short i = 0;
	TriggerInfo* tr = a1->trigger;

	while (i<4)
	{
		short a = a0->actions[i];
		if (a == 0)
			break;

		std::map<short, std::vector<EventTask*>>::iterator it = tr->tasks.find(a);
		if (it != tr->tasks.end())
		{
			for (unsigned int j = 0; j<(*it).second.size(); j++)
			{
				if ((*it).second.at(j)->start())
				{
					mEventMgr->addCachedTask((*it).second.at(j));
				}
			}
			i = 4;

			if (a1->trigger->cooldown>0)
				mEventMgr->cooldownTrigger(a1);
		}

	}
}
void TriggerObjectContactCallback::contactsProcess( OgreNewt::ContactJoint &contactJoint, Ogre::Real timeStep, int threadIndex )
{

	Ogre::Any any0 = contactJoint.getBody0()->getUserData();
	Ogre::Any any1 = contactJoint.getBody1()->getUserData();

	if(any0.isEmpty() || any1.isEmpty())
		return;

	bodyUserData* a0=Ogre::any_cast<bodyUserData*>(any0);
	bodyUserData* a1=Ogre::any_cast<bodyUserData*>(any1);

	if(a0->makeActions && a1->enabledTrigger)
	{
		processTrigger(a0, a1, mEventMgr);
	}

	if(a1->makeActions && a0->enabledTrigger)
	{
		processTrigger(a1, a0, mEventMgr);
	}

}

