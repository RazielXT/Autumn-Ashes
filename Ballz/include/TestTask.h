#ifndef TEVENTR_H
#define TEVENTR_H

#include "stdafx.h"
#include "EventTask.h"

class TestReaction : public EventTask
{
public:

    TestReaction();
    ~TestReaction();

    void setUserData(void* data);
    bool start();
    bool update(Ogre::Real tslf);
    void reaction();

private:

    OgreNewt::Body* mBody;
    bool running;
    float timer;

};

#endif //TEVENTR_H