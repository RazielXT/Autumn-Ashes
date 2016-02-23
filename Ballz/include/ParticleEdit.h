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
    ParticleEdit() {}
    ParticleEdit(Ogre::ParticleSystem* ps);
    virtual ~ParticleEdit() {}

    std::vector<EditVariable> particleParams;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & originName;
        ar & psVariables;
        ar & particleParams;
    }

    virtual EditVariables* getParams(const std::string& row) override;
    virtual void editChanged(EditVariable& var, const std::string& row) override;
    virtual void customAction(std::string name) override;

    void merge(ParticleEdit& r, bool addNotExisting);

    static ParticleEdit* query();
    static void applyChanges(std::map < std::string, ParticleEdit >& changes);
    static ParticleChildren particleChildren;

protected:

    bool changedParticle = false;
    virtual void resetMaterial() override;
    virtual void materialChanged() override;

    void setParticleParam(Ogre::ParticleSystem* ps, EditVariable& var);
    void generateParticleParams();

    Ogre::ParticleSystem* ps;
};