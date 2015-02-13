#include "stdafx.h"
#include "TestTask.h"

TestReaction::TestReaction()
{
    running=false;
}

TestReaction::~TestReaction()
{
}

void TestReaction::setUserData(void* data)
{
    //get stuff
    mBody = (OgreNewt::Body*) data;
}

bool TestReaction::start()
{
    if(running)
        return 0;

    timer=0;
    running=true;
    update(0);
    return 1;
}
/*Ogre::Any any1 = body1->getUserData();
	if(any0.isEmpty() || any1.isEmpty())
	return;

	bodyUserData* a0=Ogre::any_cast<bodyUserData*>(any0);*/
bool TestReaction::update(Ogre::Real tslf)
{

    timer+=tslf;
    if(timer>1)
    {
        reaction();
        running=false;
        Ogre::LogManager::getSingleton().getLog("RuntimeEvents.log")->logMessage("JUMP!",Ogre::LML_NORMAL);
        return 0;
    }

    return 1;
}

void TestReaction::reaction()
{
    //stuff
    mBody->setVelocity(Ogre::Vector3(0,10,0));
}
