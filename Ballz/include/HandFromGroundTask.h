#ifndef HANDFGEVENTR_H
#define HANDFGEVENTR_H

#include "stdafx.h"
#include "EventTask.h"

class HandFromGroundReaction : public EventTask
{
public:

	HandFromGroundReaction();
	~HandFromGroundReaction();

	void setUserData(void* data);
	bool start();
	bool update(Ogre::Real tslf);
	void reaction();

private:

	OgreNewt::Body* mBody;
	bool running;
	float timer;
};

#endif //HANDEVENTR_H