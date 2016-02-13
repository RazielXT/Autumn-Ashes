#pragma once

#include "MaterialEdit.h"


struct ParticleChildren
{
    std::map<std::string, std::vector<std::string>> children;

    std::vector<Ogre::ParticleSystem*> getChildren(std::string parent);
    bool isParent(std::string particle);
    std::string getParent(std::string child);
    void connectChild(std::string parent, std::string child);
};

struct ParticleEdit : public MaterialEdit
{
    virtual ~ParticleEdit() {}

    std::vector<EditVariable> particleParams;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & originName;
        ar & psVariables;
        ar & particleParams;
    }

    virtual bool query();
    virtual EditVariables* getParams(int row);
    virtual void editChanged(EditVariable& var, int row);
    virtual void customAction(std::string name);
    void merge(ParticleEdit& r, bool addNotExisting);

    void setParticleParam(Ogre::ParticleSystem* ps, EditVariable& var);

    static void applyChanges(std::map < std::string, ParticleEdit >& changes);
    static ParticleChildren particleChildren;

protected:

    void materialChanged();
    void generateParticleParams();

    Ogre::ParticleSystem* ps;
};