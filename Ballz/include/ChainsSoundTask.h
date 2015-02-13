#ifndef CHAINSSR_H
#define CHAINSSR_H

#include "stdafx.h"
#include "EventTask.h"

class ChainsSoundTask : public EventTask
{
public:

    ChainsSoundTask();

    void setUserData(void* data);
    bool start();
    bool update(Ogre::Real tslf);

private:

    OgreNewt::Body* mBody;
    irrklang::ISound* s;
    bool playing;
    bool running;
};

#endif