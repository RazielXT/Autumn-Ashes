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
		ar & originMatName;
		ar & psVariables;
		ar & vsVariables;
	}

	virtual bool query();
	virtual EditBaseRows getBaseRows();
	virtual EditVariables* getParams(int row);
	virtual void editChanged(EditVariable& var, int id);
	virtual void save();
	void merge(MaterialEdit& r, bool addNotExisting);

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