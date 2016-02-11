#pragma once

#include "MaterialEdit.h"


struct ParticleEdit : public MaterialEdit
{
	std::vector<EditVariable> particleParams;

	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & originName;
		ar & psVariables;
		ar & particleParams;
	}

	virtual bool query();
	virtual EditBaseRows getBaseRows();
	virtual EditVariables* getParams(int row);
	virtual void editChanged(EditVariable& var, int id);
	virtual void save();
	void merge(ParticleEdit& r, bool addNotExisting);

	void setParticleParam(Ogre::ParticleSystem* ps, EditVariable& var);

protected:

	void materialChanged();	
	void generateParticleParams();

	Ogre::ParticleSystem* ps;
};