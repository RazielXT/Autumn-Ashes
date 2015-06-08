#pragma once

#include "stdafx.h"
#include <vector>

class AnimationBlender
{
public:

	~AnimationBlender() { clear(); }

	void init(Ogre::Entity* targetEntity);

	void fadeTo(std::string animName, float fadeTime);
	void update(float tslf);

private:

	void clear();

	Ogre::Entity* targetEnt;

	float blendTime;
	bool blending = false;
	Ogre::AnimationState* currentState;
	std::vector<Ogre::AnimationState*> fadingStates;
};
