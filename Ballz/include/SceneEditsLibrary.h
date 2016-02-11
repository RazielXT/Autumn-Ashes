#pragma once

#include "MaterialEdit.h"
#include "ParticleEdit.h"

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

#include <boost/serialization/nvp.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>



struct ParticleChildren
{
	std::map<std::string, std::vector<std::string>> children;

	std::vector<Ogre::ParticleSystem*> getChildren(std::string parent);
	bool isParent(std::string particle);
	std::string getParent(std::string child);
	void connectChild(std::string parent, std::string child);
};

class SceneEditsLibrary
{
public:

	bool loadSavedChanges(MaterialEdit& edit, std::string entName);
	void addEdit(MaterialEdit& edit, std::string entName);
	void removeMaterialEdit(std::string entName);

	bool loadSavedParticleChanges(ParticleEdit& edit, std::string particleName);
	void addParticleEdit(ParticleEdit& edit, std::string particleName);
	void removeParticleEdit(std::string particleName);

	void saveMaterialHistory(std::string path);
	void loadMaterialHistory(std::string path);

	void saveParticleHistory(std::string path);
	void loadParticleHistory(std::string path);

	void applyChanges();

	ParticleChildren particleChildren;

private:

	using EditedEntities = std::map < std::string, MaterialEdit >;
	EditedEntities editMaterialHistory;

	using EditedParticles = std::map < std::string, ParticleEdit >;
	EditedParticles editParticleHistory;

	int idCounter = 500;
};
