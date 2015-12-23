#pragma once

#include "Gorilla.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

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

    void mergeChanges(MaterialEdit& r, bool ignoreNotExisting);

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & originMatName;
        ar & psVariables;
        ar & vsVariables;
    }
};

class MaterialEditsLibrary
{
public:

    MaterialEditsLibrary();
    ~MaterialEditsLibrary()
    {
    }


    bool loadSavedChanges(MaterialEdit& edit, std::string entName);
    void saveEdit(MaterialEdit& edit, std::string entName);
    void removeEdit(std::string entName);

    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & editHistory;
    }

    void saveFile();
    void loadFile();
    void applyChanges();

private:

    using EditedEntities = std::map < std::string, MaterialEdit >;
    using EditedLevels = std::map < std::string, EditedEntities > ;

    EditedLevels editHistory;

    int idCounter = 500;
};
