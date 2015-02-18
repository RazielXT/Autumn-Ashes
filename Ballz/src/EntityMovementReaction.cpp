#include "stdafx.h"
#include "EntityMovementTask.h"

EntityMovementReaction::EntityMovementReaction()
{
    running=false;
}

EntityMovementReaction::~EntityMovementReaction()
{
}

void EntityMovementReaction::setIdentifier(short id)
{
    this->id = id;
}

//get stuff
void EntityMovementReaction::setUserData(void* data)
{
    Ogre::Entity* mEntity = (Ogre::Entity*) data;
    node = mEntity->getParentSceneNode();
}

bool EntityMovementReaction::start()
{
    if(running)
        return 0;

    running=true;

    switch(id)
    {
    case 0:
    default:
        dir = Ogre::Vector3(0,0,-10);
        break;
    };

    return 1;
}

bool EntityMovementReaction::update(Ogre::Real tslf)
{
    tslf *= Global::timestep;
    node->translate(dir*tslf);

    return 1;
}

