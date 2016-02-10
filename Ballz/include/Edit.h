#pragma once

#include "Gorilla.h"

struct EditVariable
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

using EditVariables = std::vector<EditVariable>;

struct EditCategory
{
	std::string name;
	bool hasParams;
};
using EditCategories = std::vector<EditCategory>;

struct Edit
{
	virtual bool query() = 0;
	virtual EditCategories getCategories() = 0;
	virtual EditVariables* getParams(int id) = 0;
	virtual void editChanged(EditVariable& var, int id) = 0;
	virtual void save() = 0;
};