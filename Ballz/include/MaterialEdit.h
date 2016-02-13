#pragma once

#include "Edit.h"


struct MaterialEdit : public Edit
{
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

    virtual bool query();
    virtual EditVariables* getParams(int row);
    virtual void editChanged(EditVariable& var, int row);
    virtual void customAction(std::string name);
    void merge(MaterialEdit& r, bool addNotExisting);

    static void applyChanges(const std::map < std::string, MaterialEdit >& changes);

protected:

    void resetMaterial();
    void loadMaterial();
    void materialChanged();
    void reset();

    bool changed = false;
    bool matInstance = false;
    Ogre::MaterialPtr ptr;
    Ogre::Entity* entity;
};