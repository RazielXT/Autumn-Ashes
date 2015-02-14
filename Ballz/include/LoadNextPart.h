#pragma once

#include "stdafx.h"
#include "EventTask.h"

class LoadNextPart : public EventTask
{
public:

    LoadNextPart(Ogre::String target);

    bool start();
    bool update(Ogre::Real tslf);

private:

    float timer;
    bool loadNext;
    Ogre::String target;
};