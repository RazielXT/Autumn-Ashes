#pragma once

#include "Edit.h"

struct LevelInfo;

struct LevelEdit : public Edit
{
	LevelEdit();
	virtual ~LevelEdit() {}

	std::vector<EditVariable> ppVariables;
	std::vector<EditVariable> envVariables;
	std::vector<EditVariable> reserved;

	std::vector<EditVariable> tempVariables;

	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & ppVariables;
		ar & envVariables;
		ar & reserved;
	}

	virtual EditVariables* getParams(const std::string& row) override;
	virtual void editChanged(EditVariable& var, const std::string& row) override;
	virtual void customAction(std::string name) override;

	static LevelEdit* query();
	void applyChanges();
	void clear();
	void init();

protected:

	void generateParams();

	void applyPPEdit(EditVariable& var);
	void applySceneEdit(EditVariable& var);

	LevelInfo* level;
};

//BOOST_CLASS_VERSION(LevelEdit, 1)