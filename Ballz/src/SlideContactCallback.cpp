#include "stdafx.h"
#include "SlideContactCallback.h"
#include "Slide.h"

SlideContactCallback::SlideContactCallback(EventsManager* eventMgr) : OgreNewt::ContactCallback()
{
    mEventMgr = eventMgr;
}

SlideContactCallback::~SlideContactCallback(void)
{
}

int SlideContactCallback::onAABBOverlap(OgreNewt::Body* body0, OgreNewt::Body* body1, int threadIndex)
{
    return 1;
}

inline void processPlayerTouch(bodyUserData* a0, OgreNewt::Body* body0, EventsManager* mEventMgr, OgreNewt::ContactJoint &contactJoint)
{
    auto type = body0->getType();
    OgreNewt::Contact c = contactJoint.getFirstContact();
    Vector3 pos, nor;
    c.getPositionAndNormal(pos, nor);

    auto lineName = (std::string*) a0->customData;
    auto slide = (Slide*)(*Global::globalData)[*lineName];

    if (slide->start(pos))
    {
        mEventMgr->addCachedTask(slide);
    }
    else
    {
        contactJoint.getContactCount();

        while (contactJoint.getContactCount() > 1)
        {
            OgreNewt::Contact c2 = c.getNext();
            c.remove();
            c = c2;
        }

        c.remove();
    }

}

void SlideContactCallback::contactsProcess(OgreNewt::ContactJoint &contactJoint, Ogre::Real timeStep, int threadIndex)
{
    OgreNewt::Body* body0 = contactJoint.getBody0();
    OgreNewt::Body* body1 = contactJoint.getBody1();

    Ogre::Any any0 = body0->getUserData();
    Ogre::Any any1 = body1->getUserData();

    //player body doesnt have user data
    if (!any0.isEmpty())
    {
        bodyUserData* a0 = Ogre::any_cast<bodyUserData*>(any0);
        processPlayerTouch(a0, body0, mEventMgr, contactJoint);
    }
    else if (!any1.isEmpty())
    {
        bodyUserData* a1 = Ogre::any_cast<bodyUserData*>(any1);
        processPlayerTouch(a1, body1, mEventMgr, contactJoint);
    }

}

