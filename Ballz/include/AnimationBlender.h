#pragma once

#include "stdafx.h"
#include <vector>

class AnimationBlender
{
public:

    ~AnimationBlender()
    {
        clear();
    }

    void init(Ogre::Entity* targetEntity);

    void fadeTo(std::string animName, float fadeTime);
    void update(float tslf);

private:

    void clear();

    Ogre::Entity* targetEnt;

    float blendTimer;
    float blendTargetTime;
    bool blending = false;

    Ogre::AnimationState* currentState = nullptr;

    std::vector<Ogre::AnimationState*> fadingStates;
    std::vector<float> fadingStatesStartWeight;
};
