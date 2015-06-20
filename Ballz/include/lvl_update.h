#pragma once

#include "PagedGeometry.h"
#include "BatchPage.h"
#include "GrassLoader.h"
#include "EventTask.h"
#include "stdafx.h"


class LvlMenuUpdate : public EventTask
{
public:

    LvlMenuUpdate(Ogre::SceneNode* cRot,Forests::PagedGeometry *gr);
    //~Lvl2Update ();
    bool update(Ogre::Real tslf);

private:
    Forests::PagedGeometry *grass;
    Ogre::SceneNode* cRotator;
};
