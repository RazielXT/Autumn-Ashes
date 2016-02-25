#pragma once

#include "MaterialEdit.h"
#include "OptimizedGroupEdit.h"
#include "ParticleEdit.h"
#include "LevelEdit.h"
#include "DetailGeometryEdit.h"

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

#include <boost/serialization/nvp.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/version.hpp>


class SceneEditsLibrary
{
public:

	void loadChanges();
	void clear();

	bool loadSavedMaterialChanges(MaterialEdit& edit, std::string entName);
	void addMaterialEdit(MaterialEdit& edit, std::string entName);
	void removeMaterialEdit(std::string entName);

	bool loadSavedParticleChanges(ParticleEdit& edit, std::string particleName);
	void addParticleEdit(ParticleEdit& edit, std::string particleName);
	void removeParticleEdit(std::string particleName);

	bool loadSavedDetailGeometryChanges(DetailGeometryEdit& edit, std::string name);
	void addDetailGeometryEdit(DetailGeometryEdit& edit, std::string name);
	void removeDetailGeometryEdit(std::string name);

	bool loadSavedOptimizedGroupChanges(OptimizedGroupEdit& edit, std::string name);
	void addOptimizedGroupEdit(OptimizedGroupEdit& edit, std::string name);
	void removeOptimizedGroupEdit(std::string name);

	LevelEdit* getLevelEdit();
	void saveLevelEdit();

private:

	void saveLevelEdit(std::string path);
	void loadLevelEdit(std::string path);
	LevelEdit levelEdits;


	template <class T>
	struct EditedData
	{
		std::map < std::string, T > data;

		template<class Archive>
		void serialize(Archive & ar, const unsigned int version)
		{
			ar & data;
		}
	};

	void saveMaterialHistory(std::string path);
	void loadMaterialHistory(std::string path);
	EditedData<MaterialEdit> materialEditHistory;

	void saveParticleHistory(std::string path);
	void loadParticleHistory(std::string path);
	EditedData<ParticleEdit> particleEditHistory;

	void saveDgHistory(std::string path);
	void loadDgHistory(std::string path);
	EditedData<DetailGeometryEdit> dgEditHistory;

	void saveOgHistory(std::string path);
	void loadOgHistory(std::string path);
	EditedData<OptimizedGroupEdit> ogEditHistory;

	int idCounter = 500;
};
