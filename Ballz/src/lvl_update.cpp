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

Lvl2Update::Lvl2Update()
{
    stateExecution = UNDEFINED;
}

bool Lvl2Update::update(Ogre::Real evt)
{
    if (Global::fallSoundOffsetH > 0)
    {
        Global::fallSoundOffsetH -= evt;
    }
    if (Global::fallSoundOffsetL > 0)
    {
        Global::fallSoundOffsetL -= evt;
    }

    return true;
}
