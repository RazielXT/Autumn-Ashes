#include "stdafx.h"
#include "TriggerPlayerContactCallback.h"
#include "TriggerInfo.h"

TriggerPlayerContactCallback::TriggerPlayerContactCallback(EventsManager* eventMgr) : OgreNewt::ContactCallback()
{
	mEventMgr=eventMgr;
}

TriggerPlayerContactCallback::~TriggerPlayerContactCallback(void)
{
}

int TriggerPlayerContactCallback::onAABBOverlap( OgreNewt::Body* body0, OgreNewt::Body* body1, int threadIndex )
{
	return 1;
}

inline void processPlayerTrigger(bodyUserData* a0, OgreNewt::Body* body0, EventsManager* mEventMgr, OgreNewt::ContactJoint &contactJoint)
{
	if (a0->enabledTrigger)
	{
		short a = a0->trigger->playerAction;

		std::map<short, std::vector<EventTask*>>::iterator it = a0->trigger->tasks.find(a);
		if (it != a0->trigger->tasks.end())
		{
			Ogre::LogManager::getSingleton().getLog("RuntimeEvents.log")->logMessage("Starting reactions id " + Ogre::StringConverter::toString((*it).first) + ", total " + Ogre::StringConverter::toString((*it).second.size()), Ogre::LML_NORMAL);

			for (unsigned int j = 0; j<(*it).second.size(); j++)
			{
				if ((*it).second.at(j)->taskDelay || (*it).second.at(j)->start())
				{
					mEventMgr->addCachedTask((*it).second.at(j));
				}
				else
					Ogre::LogManager::getSingleton().getLog("RuntimeEvents.log")->logMessage("Started non-updating reaction id " + Ogre::StringConverter::toString((*it).first), Ogre::LML_NORMAL);
			}

			if (a0->trigger->cooldown>0)
				mEventMgr->cooldownTrigger(a0);
		}
	}

	if (a0->trigger->activationType == 2)
	{
		OgreNewt::Contact c = contactJoint.getFirstContact();
		contactJoint.getContactCount();

		while (contactJoint.getContactCount()>1)
		{
			OgreNewt::Contact c2 = c.getNext();
			c.remove();
			c = c2;
		}

		c.remove();
		delete body0;
	}

}

void TriggerPlayerContactCallback::contactsProcess( OgreNewt::ContactJoint &contactJoint, Ogre::Real timeStep, int threadIndex )
{
	OgreNewt::Body* body0=contactJoint.getBody0();
	OgreNewt::Body* body1=contactJoint.getBody1();

	Ogre::Any any0 = body0->getUserData();
	Ogre::Any any1 = body1->getUserData();

	//player body doesnt have user data
	if(!any0.isEmpty())
	{
		bodyUserData* a0 = Ogre::any_cast<bodyUserData*>(any0);
		processPlayerTrigger(a0, body0, mEventMgr, contactJoint);
	}
	else if(!any1.isEmpty())
	{
		bodyUserData* a1 = Ogre::any_cast<bodyUserData*>(any1);
		processPlayerTrigger(a1, body1, mEventMgr, contactJoint);
	}

}

