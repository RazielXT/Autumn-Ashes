#include "stdafx.h"
#include "AnimationLoop.h"

AnimationLoop::AnimationLoop()
{
}

AnimationLoop::AnimationLoop(Ogre::Entity* e, std::string anim)
{
	start(e, anim);
}

bool AnimationLoop::update(Ogre::Real tslf)
{
	if (state)
		state->addTime(tslf);

	return state != nullptr;
}

void AnimationLoop::start(Ogre::Entity* e, std::string anim)
{
	if (!state)
		Global::eventsMgr->addCachedTask(this);

	state = e->getAnimationState(anim);
	state->setLoop(true);
	state->setEnabled(true);
	state->setTimePosition(0);
}

void AnimationLoop::reset()
{
	state = nullptr;
}

void AnimationLoop::stop()
{
	if (state)
		state->setEnabled(false);

	reset();
}
