#ifndef EVENTR_H
#define EVENTR_H

//#include "stdafx.h"
#include <vector>

class EventTask
{
public:

    EventTask() {};
    virtual ~EventTask() {};

    virtual void setUserData(void* data);
    virtual bool start();
    virtual bool update(Ogre::Real tslf);

    float taskDelay = 0;
    GameState stateExecution = GAME;

};

#endif //EVENTR_H