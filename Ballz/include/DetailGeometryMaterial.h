#pragma once

#include "stdafx.h"
#include "DetailGeometryInfo.h"
#include "GameStateManager.h"

class DetailGeometryMaterial
{
	std::map<std::string, Ogre::Material*> materials;

	static std::map<std::string, VCEditFunc> editVCMeshes;
	static std::vector<std::string> editVCMeshesDone;

public:

	static void reset();

	DetailGeometryMaterial();
	void updateMaterial(Ogre::Entity* ent, Ogre::Vector3&, DetailGeometryInfo& info);
	std::vector<Ogre::Material*> getGeneratedMaterials();
};