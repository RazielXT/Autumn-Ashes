#pragma once

#include "stdafx.h"
#include "DetailGeometry.h"
#include "DetailGeometryMaterial.h"

struct LoadedManualDG
{
    Ogre::AxisAlignedBox bbox;
    Ogre::StaticGeometry* sg;
    int id;
    std::string name;
    std::vector<Ogre::Material*> usedMats;
};

class ManualDetailGeometry
{

protected:

    Ogre::AxisAlignedBox bbox;
    std::string name;
    DetailGeometryMaterial mats;

    std::vector<Ogre::Entity*> usedEntities;
    Ogre::StaticGeometry* sg = nullptr;
    void build();

    static std::map<int, ManualDetailGeometry*> mdg;
    static std::vector<LoadedManualDG> loadedMDG;

public:

    ManualDetailGeometry(int id_)
    {
        id = id_;
    }
    int id;

    static LoadedManualDG* getClosest()
    {
        LoadedManualDG* dgOut = nullptr;

        for (auto& dg : loadedMDG)
        {
            dgOut = &dg;
        }

        return dgOut;
    }

    static void buildAll()
    {
        loadedMDG.clear();

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
            ManualDetailGeometry* dg = new ManualDetailGeometry(id);
            mdg[id] = dg;
            return dg;
        }
    }

    void addObject(Ogre::SceneNode* node, std::string type, bool keepMesh, Ogre::Vector3 color);

};