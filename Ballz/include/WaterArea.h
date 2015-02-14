#pragma once

#include "stdafx.h"
#include "EventTask.h"

class WaterArea : public EventTask
{
public:

    WaterArea();
    ~WaterArea();

    bool update(Ogre::Real tslf);

    void init(Ogre::Entity* water);

private:
    Ogre::AxisAlignedBox b;
    bool inside = false;
    Player* p;
};