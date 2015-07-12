#include "stdafx.h"
#include "ImpostorRenderer.h"

using namespace Ogre;

void ImpostorRenderer::addGeometry(Ogre::Entity* ent)
{
    if (textures.find(ent) == textures.end())
    {
        ImpostorTexture* txt = new ImpostorTexture();
        txt->render(ent);

        textures[ent] = txt;
    }
}

void ImpostorRenderer::clear()
{
    for (auto p : textures)
    {
        delete p.second;
    }

    textures.clear();
}

