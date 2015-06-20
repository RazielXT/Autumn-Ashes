#include "stdafx.h"
#include "lvl_update.h"
#include "PagedGeometry.h"
#include "BatchPage.h"
#include "GrassLoader.h"


LvlMenuUpdate::LvlMenuUpdate(Ogre::SceneNode* cRot, Forests::PagedGeometry *gr)
{
    cRotator = cRot;
    cRotator->yaw(Ogre::Degree(-30));

    grass = gr;
    stateExecution = UNDEFINED;
};

bool LvlMenuUpdate::update(Ogre::Real evt)
{
    cRotator->yaw(Ogre::Degree(evt*-3));
    if (grass)
        grass->update();

    return true;
}

