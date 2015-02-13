#ifndef ENTMOVR_H
#define ENTMOVR_H

#include "stdafx.h"
#include "EventTask.h"

class EntityMovementReaction : public EventTask
{
public:

    EntityMovementReaction();
    ~EntityMovementReaction();

    void setUserData(void* data);
    bool start();
    bool update(Ogre::Real tslf);
    void setIdentifier(short id);

private:

    Ogre::SceneNode* node;
    Ogre::Vector3 dir;
    bool running;
    short id;
};

#endif