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

		generateParams();
	}

	virtual EditVariables* getParams(const std::string& row) override;
	virtual void editChanged(EditVariable& var, const std::string& row) override;
	virtual void customAction(std::string name) override;

	static LevelEdit* query();
	void applyChanges();
	void clear();
	void init();

	void applyFog();
	void applyPostProcessing();

protected:

	void generateParams();

	void applyPPEdit(EditVariable& var);
	void applySceneEdit(EditVariable& var);

	struct
	{
		Ogre::ColourValue fogColor;
		float fogStartDistance;
		float fogEndDistance;
		Ogre::ColourValue ambientColor;
		Ogre::ColourValue sunColor;

		Ogre::Vector4 ContSatuSharpNoise;
		Ogre::Vector4 ColorShift;
		float bloomAdd;
		float bloomSize;
		float bloomStr;
		float bloomDepth;
	}
	level;
};

//BOOST_CLASS_VERSION(LevelEdit, 1)