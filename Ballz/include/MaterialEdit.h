#pragma once

#include "Edit.h"


struct MaterialEdit : public Edit
{
    MaterialEdit() {}
    MaterialEdit(Ogre::Entity* ent);
    virtual ~MaterialEdit() {}

    std::vector<EditVariable> psVariables;
    std::vector<EditVariable> vsVariables;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & originName;
        ar & psVariables;
        ar & vsVariables;
    }

    virtual EditVariables* getParams(int row);
    virtual void editChanged(EditVariable& var, int row);
    virtual void customAction(std::string name);
    void merge(MaterialEdit& r, bool addNotExisting);

    static MaterialEdit* query();
    static void applyChanges(const std::map < std::string, MaterialEdit >& changes);

protected:

    void loadMaterial();

    virtual void resetMaterial();
    virtual void materialChanged();

    bool changed = false;
    bool matInstance = false;
    Ogre::MaterialPtr materialPtr;
    Ogre::Entity* entity = nullptr;
};