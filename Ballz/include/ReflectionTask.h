#pragma once

#include "stdafx.h"
#include "EventTask.h"
#include "OgreTexture.h"

class ReflectionTask : public EventTask
{
public:

    ReflectionTask();
    ~ReflectionTask();

    bool start();
    bool update(Ogre::Real tslf);

    void init(Ogre::Entity* water, int visibilityFlag);

private:

    Ogre::TexturePtr texture;
    Ogre::Camera* mReflectCam;
    Ogre::Entity* mEntity;
    int visibilityFlag;

    static int id;
};