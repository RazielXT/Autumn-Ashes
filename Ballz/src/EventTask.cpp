#include "stdafx.h"
#include "EventTask.h"

bool EventTask::update(float tslf)
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

void DelayedEventTask::startDelay()
{
	timer = delay;
}

bool DelayedEventTask::checkDelay(float tslf)
{
	timer -= tslf;
	return timer <= 0;
}