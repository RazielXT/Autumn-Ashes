#pragma once

#include "stdafx.h"
#include "ImpostorTexture.h"


class ImpostorRenderer
{
public:

    static ImpostorRenderer instance;

    void addGeometry(Ogre::Entity* ent);
    void clear();

protected:

    ImpostorRenderer() {}
    ~ImpostorRenderer() {}

    std::map<Ogre::Entity*, ImpostorTexture*> textures;

};