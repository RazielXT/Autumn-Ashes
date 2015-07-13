#pragma once

#include "stdafx.h"
#include "EventTask.h"
#include "VolumeDetection.h"

class WaterArea : public EventTask
{
public:

    WaterArea();
    ~WaterArea();

    void setUserData(void* data);
    bool start();
    bool update(Ogre::Real tslf);

private:

    VolumeDetection volume;
};