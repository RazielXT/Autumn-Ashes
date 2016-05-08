#pragma once

#include "stdafx.h"
#include "DetailGeometry.h"
#include "DetailGeometryMaterial.h"
#include "Player.h"

class ManualDetailGeometry
{
public:

	ManualDetailGeometry(int id);

	void addObject(Ogre::SceneNode* node, std::string type, bool keepMesh, Ogre::Vector3 color);

	static std::vector<LoadedDG> loadedMDG;
	static LoadedDG* getClosest();
	static void buildAll();
	static ManualDetailGeometry* get(int id);

protected:

	LoadedDG info;
	std::vector<Ogre::Entity*> usedEntities;
	DetailGeometryMaterial materialHelper;

	void build();

	static std::map<int, ManualDetailGeometry*> mdg;

};