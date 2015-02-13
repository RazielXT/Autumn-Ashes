#include "stdafx.h"
#include "EventTask.h"

void EventTask::setIdentifier(short id)
{
    this->id = id;
}

bool EventTask::update(Ogre::Real tslf)
{
    return false;
}

bool EventTask::start()
{
    return true;
}

void EventTask::setUserData(void* data)
{
}