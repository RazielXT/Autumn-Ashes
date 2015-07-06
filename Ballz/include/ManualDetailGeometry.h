#pragma once

#include "stdafx.h"
#include "DetailGeometry.h"
#include "DetailGeometryMaterial.h"

class ManualDetailGeometry
{

protected:

    DetailGeometryMaterial mats;

    std::vector<Ogre::Entity*> usedEntities;
    Ogre::StaticGeometry* sg = nullptr;
    void build();

    static std::map<int, ManualDetailGeometry*> mdg;

public:

    static void finish()
    {
        for (auto dgi : mdg)
        {
            auto dg = dgi.second;
            dg->build();
            delete dg;
        }

        mdg.clear();
    }

    static ManualDetailGeometry* get(int id)
    {
        if (mdg.find(id) != mdg.end())
        {
            return mdg[id];
        }
        else
        {
            ManualDetailGeometry* dg = new ManualDetailGeometry();
            mdg[id] = dg;
            return dg;
        }
    }

    void addObject(Ogre::SceneNode* node, std::string type, bool keepMesh, Ogre::Vector3 color);

};