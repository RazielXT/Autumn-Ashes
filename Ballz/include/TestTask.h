#ifndef TEVENTR_H
#define TEVENTR_H

#include "stdafx.h"
#include "EventTask.h"
#include "VolumeDetection.h"

class TestReaction : public EventTask
{
public:

    TestReaction();
    ~TestReaction();

    void setUserData(void* data);
    bool start();
    bool update(Ogre::Real tslf);

private:

    VolumeDetection volume;
};

#endif //TEVENTR_H