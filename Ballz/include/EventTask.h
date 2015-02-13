#ifndef EVENTR_H
#define EVENTR_H

#include "stdafx.h"
#include <vector>
#include "GameStateManager.h"

class EventTask
{
public:

    EventTask() {};
    virtual ~EventTask() {};

    virtual void setUserData(void* data);
    virtual bool start();
    virtual bool update(Ogre::Real tslf);
    virtual void setIdentifier(short id);

    float taskDelay = 0;
	GameState stateExecution = PLAY;

protected:

    short id = 0;

};

#endif //EVENTR_H