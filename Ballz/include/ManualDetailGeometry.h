#pragma once

#include "stdafx.h"
#include "DetailGeometry.h"
#include "DetailGeometryMaterial.h"
#include "Player.h"

struct LoadedManualDG
{
    Ogre::AxisAlignedBox bbox;
    Ogre::StaticGeometry* sg = nullptr;
    int id;
    std::string name;
    std::vector<Ogre::Material*> usedMats;
};

class ManualDetailGeometry
{
public:

    ManualDetailGeometry(int id);

    void addObject(Ogre::SceneNode* node, std::string type, bool keepMesh, Ogre::Vector3 color);


    static std::vector<LoadedManualDG> loadedMDG;
    static LoadedManualDG* getClosest();
    static void buildAll();
    static ManualDetailGeometry* get(int id);

protected:

    LoadedManualDG info;
    std::vector<Ogre::Entity*> usedEntities;
    DetailGeometryMaterial materialHelper;

    void build();

    static std::map<int, ManualDetailGeometry*> mdg;

};