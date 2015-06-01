#include "stdafx.h"
#include "Crow.h"

using namespace Ogre;

Crow::Crow()
{
    //init entity+node
    mEntity = Global::mSceneMgr->createEntity("testObject04.mesh");
    mNode = Global::mSceneMgr->getRootSceneNode()->createChildSceneNode();
    mNode->attachObject(mEntity);
    mNode->setScale(0.05f, 0.05f, 0.05f);

    stateChangeTimer = 0;
}

Crow::~Crow()
{
}

void Crow::update(Ogre::Real tslf)
{
    //update model anim

    //update node pos
    if (path.update(tslf, mNode))
        stateChangeTimer = 5;
    else
        //update state timer
        stateChangeTimer -= tslf;
}

bool Crow::readyToFly() const
{
    return (path.state == OnGround && stateChangeTimer <= 0);
}

bool Crow::readyToLand() const
{
    return (path.state == Flying && stateChangeTimer <= 0);
}

void Crow::flyTo(Ogre::Animation* flightAnim)
{
    if (path.state == OnGround)
    {
        //start lifting
        path.setLiftingAnim(flightAnim, 0);
    }
    else
    {
        //force state
        path.setLiftingAnim(flightAnim, 0);
    }
}

void Crow::landTo(Ogre::Vector3 pos)
{
    if (path.state == OnGround)
    {
        //start landing
        path.setLandingAnim(pos);
    }
    else
    {
        //force state
        path.setLandingAnim(pos);
    }
}
