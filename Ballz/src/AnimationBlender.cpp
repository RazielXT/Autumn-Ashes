#include "stdafx.h"
#include "AnimationBlender.h"

void AnimationBlender::init(Ogre::Entity* targetEntity)
{
    currentState = nullptr;
    clear();
    targetEnt = targetEntity;
}

void AnimationBlender::fadeTo(std::string animName, float fadeTime)
{
    auto animState = targetEnt->getAnimationState(animName);
    animState->setLoop(true);

    if (currentState)
    {
        fadingStates.push_back(currentState);
        fadingStatesStartWeight.push_back(currentState->getWeight());
    }

    blendTargetTime = fadeTime;
    currentState = animState;
    blending = true;


    int fadedId = -1;
    for (size_t i = 0; i < fadingStates.size(); i++)
    {
        if (fadingStates[i] == animState)
        {
            fadedId = i;
        }
    }

    if (fadedId == -1)
    {
        currentState->setEnabled(true);
        currentState->setWeight(0);
        blendTimer = 0;
    }
    else
    {
        fadingStates.erase(fadingStates.begin() + fadedId);
        fadingStatesStartWeight.erase(fadingStatesStartWeight.begin() + fadedId);
        blendTimer = currentState->getWeight()*blendTargetTime;
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
        blendTimer += tslf;
        float weight = std::min(1.0f, blendTimer / blendTargetTime);
        currentState->setWeight(weight);

        bool blended = weight == 1.0f;

        for (size_t i = 0; i < fadingStates.size(); i++)
        {
            auto s = fadingStates[i];
            s->setWeight((1 - weight)*fadingStatesStartWeight[i]);
            s->setEnabled(!blended);
        }

        if (blended)
        {
            fadingStates.clear();
            fadingStatesStartWeight.clear();
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

    if (currentState)
        currentState->setEnabled(false);

    currentState = nullptr;

    for (auto s : fadingStates)
    {
        s->setEnabled(false);
    }

    fadingStates.clear();
    fadingStatesStartWeight.clear();
}