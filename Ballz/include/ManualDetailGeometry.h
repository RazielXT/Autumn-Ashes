#pragma once

#include "stdafx.h"
#include "DetailGeometry.h"
#include "DetailGeometryMaterial.h"
#include "Player.h"

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

public:

	ManualDetailGeometry(int id);
    int id;

	static std::vector<LoadedManualDG> loadedMDG;
	static LoadedManualDG* getClosest();
	static void buildAll();
	static ManualDetailGeometry* get(int id);

    void addObject(Ogre::SceneNode* node, std::string type, bool keepMesh, Ogre::Vector3 color);

};