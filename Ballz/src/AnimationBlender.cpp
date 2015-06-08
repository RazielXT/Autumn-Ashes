#include "stdafx.h"
#include "AnimationBlender.h"

void AnimationBlender::init(Ogre::Entity* targetEntity)
{
	clear();
	targetEnt = targetEntity;
}

void AnimationBlender::fadeTo(std::string animName, float fadeTime)
{
	auto animState = targetEnt->getAnimationState(animName);

	if (currentState)
		fadingStates.push_back(currentState);

	fadingStates.erase(std::remove(fadingStates.begin(), fadingStates.end(), animState), fadingStates.end());

	blendTime = fadeTime;
	currentState = animState;
	blending = true;

	if (!currentState->getEnabled())
	{
		currentState->setEnabled(true);
		currentState->setWeight(0);
	}	

	if (fadeTime == 0)
	{
		currentState->setWeight(1);
		fadingStates.clear();
		blending = false;
	}
}

void AnimationBlender::update(float tslf)
{
	if (blending)
	{
		float wDiff = tslf / blendTime;

		currentState->setWeight(std::min(1.0f, currentState->getWeight() + wDiff));
		bool blended = currentState->getWeight() == 1.0f;

		for (auto s : fadingStates)
		{
			float fw = wDiff / fadingStates.size();
			s->setWeight(std::max(0.0f, s->getWeight() - fw));
			s->setEnabled(!blended);
		}	

		if (blended)
		{
			fadingStates.clear();
			blending = false;
		}
		else
		for (auto s : fadingStates)
		{
			s->addTime(tslf);
		}
	}

	currentState->addTime(tslf);	
}

void AnimationBlender::clear()
{
	targetEnt = nullptr;
	blending = false;

	currentState->setEnabled(false);
	currentState = nullptr;

	for (auto s : fadingStates)
	{
		s->setEnabled(false);
	}

	fadingStates.clear();

}