#pragma once
#include "stdafx.h"
#include "EventTask.h"

class AnimationLoop : public EventTask
{
public:

	AnimationLoop();

	AnimationLoop(Ogre::Entity* e, std::string anim);

	bool update(Ogre::Real tslf);

	void start(Ogre::Entity* e, std::string anim);

	void reset();

	void stop();

private:
	Ogre::AnimationState* state = nullptr;
};