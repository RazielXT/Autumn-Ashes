#pragma once

#include "stdafx.h"
#include "EventTask.h"
#include "OgreTexture.h"

class WaterReflection : public EventTask
{
public:

    WaterReflection();
    ~WaterReflection();

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