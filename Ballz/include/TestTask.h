#ifndef TEVENTR_H
#define TEVENTR_H

#include "stdafx.h"
#include "EventTask.h"
#include "Volume.h"

class TestReaction : public EventTask
{
public:

	TestReaction();
	~TestReaction();

	void setUserData(void* data);
	bool start();
	bool update(Ogre::Real tslf);

private:

	Volume volume;
};

#endif //TEVENTR_H