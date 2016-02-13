#pragma once

#include "MaterialEdit.h"
#include "ParticleEdit.h"

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

#include <boost/serialization/nvp.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>


class SceneEditsLibrary
{
public:

    bool loadSavedMaterialChanges(MaterialEdit& edit, std::string entName);
    void addMaterialEdit(MaterialEdit& edit, std::string entName);
    void removeMaterialEdit(std::string entName);

    bool loadSavedParticleChanges(ParticleEdit& edit, std::string particleName);
    void addParticleEdit(ParticleEdit& edit, std::string particleName);
    void removeParticleEdit(std::string particleName);

    void loadChanges();
    void clear();

private:

    void saveMaterialHistory(std::string path);
    void loadMaterialHistory(std::string path);

    void saveParticleHistory(std::string path);
    void loadParticleHistory(std::string path);

    struct EditedEntities
    {
        std::map < std::string, MaterialEdit > data;

        template<class Archive>
        void serialize(Archive & ar, const unsigned int version)
        {
            ar & data;
        }
    }
    materialEditHistory;

    struct EditedParticles
    {
        std::map < std::string, ParticleEdit > data;

        template<class Archive>
        void serialize(Archive & ar, const unsigned int version)
        {
            ar & data;
        }
    }
    particleEditHistory;

    int idCounter = 500;
};
