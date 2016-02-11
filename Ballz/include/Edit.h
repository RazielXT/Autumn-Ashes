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

struct EditRow
{
	std::string name;
	enum {Params, Static, Save} type;
};
using EditBaseRows = std::vector<EditRow>;

struct Edit
{
	static int idCounter;

	std::string originName;

	virtual bool query() = 0;
	virtual EditBaseRows getBaseRows() = 0;
	virtual EditVariables* getParams(int id) = 0;
	virtual void editChanged(EditVariable& var, int id) = 0;
	virtual void save() = 0;

protected:

	void mergeParams(std::vector<EditVariable>& from, std::vector<EditVariable>& target, bool addNotExisting);
};