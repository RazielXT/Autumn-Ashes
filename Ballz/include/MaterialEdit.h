#pragma once

#include "Edit.h"


struct MaterialEdit : public Edit
{
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{

	}

	virtual bool query();
	virtual EditCategories getCategories();
	virtual EditVariables* getParams(int id);
	virtual void editChanged(EditVariable& var, int id);
	virtual void save();
};