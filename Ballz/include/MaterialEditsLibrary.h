#pragma once

#include "Gorilla.h"

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

#include <boost/serialization/nvp.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>

struct MaterialEdit
{
    struct MaterialVariable
    {
        std::string name;
        int size = 0;
        float buffer[4];

        bool edited = false;

        template<class Archive>
        void serialize(Archive & ar, const unsigned int version)
        {
            ar & name;
            ar & size;
            ar & boost::serialization::make_array(buffer, 4);
        }
    };

    std::string originMatName;
    std::vector<MaterialVariable> psVariables;
    std::vector<MaterialVariable> vsVariables;

    std::vector<MaterialVariable> moreParams;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & originMatName;
        ar & psVariables;
        ar & vsVariables;
        ar & moreParams;
    }

    void mergeChanges(MaterialEdit& r, bool ignoreNotExisting);
    void setMaterialParam(Ogre::MaterialPtr ptr, MaterialEdit::MaterialVariable& var);
    void setParticleParam(Ogre::ParticleSystem* ps, MaterialEdit::MaterialVariable& var);
    void generateParticleParams(Ogre::ParticleSystem* ps);
};

class MaterialEditsLibrary
{
public:

    MaterialEditsLibrary();
    ~MaterialEditsLibrary()
    {
    }


    bool loadSavedChanges(MaterialEdit& edit, std::string entName);
    void addEdit(MaterialEdit& edit, std::string entName);
    void removeEdit(std::string entName);

    bool loadSavedParticleChanges(MaterialEdit& edit, std::string particleName);
    void addParticleEdit(MaterialEdit& edit, std::string particleName);
    void removeParticleEdit(std::string particleName);

    void saveFile(std::string path);
    void loadFile(std::string path);

    void applyChanges();

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & editHistory;
        ar & editParticleHistory;
    }

private:

    using EditedEntities = std::map < std::string, MaterialEdit >;
    EditedEntities editHistory;

    using EditedParticles = std::map < std::string, MaterialEdit >;
    EditedParticles editParticleHistory;

    int idCounter = 500;
};
