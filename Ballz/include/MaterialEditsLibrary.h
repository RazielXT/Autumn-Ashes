#pragma once

#include "Gorilla.h"

struct MaterialEdit
{
    struct MaterialVariable
    {
        std::string name;
        int size = 0;
        float buffer[4];

        bool edited = false;
    };

    std::string originMatName;
    std::vector<MaterialVariable> psVariables;
    std::vector<MaterialVariable> vsVariables;

    void mergeChanges(MaterialEdit& r, bool ignoreNotExisting);
};

class MaterialEditsLibrary
{
public:

    MaterialEditsLibrary();
    ~MaterialEditsLibrary()
    {
    }

    bool loadEdit(MaterialEdit& edit, std::string entName);
    void saveEdit(MaterialEdit& edit, std::string entName);
    void removeEdit(std::string entName);

    void saveFile();
    void loadFile();
	void applyChanges();

private:

    using EditedEntities = std::map < std::string, MaterialEdit > ;
    using EditedLevels = std::map < std::string, EditedEntities > ;

    EditedLevels editHistory;
	int idCounter = 500;
};
