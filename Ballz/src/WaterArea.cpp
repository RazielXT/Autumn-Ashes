#include "stdafx.h"
#include "WaterArea.h"
#include "Player.h"

using namespace Ogre;

WaterArea::WaterArea()
{
}

WaterArea::~WaterArea()
{
}

void WaterArea::init(Ogre::Entity* water)
{
    inside = false;
    b = water->getBoundingBox();
    p = Global::player;
}

bool WaterArea::update(Ogre::Real tslf)
{
    bool now = b.contains(p->body->getPosition());

    if (!inside && now)
    {
        //splash
    }
    else
        inside = now;

    return 1;
}
