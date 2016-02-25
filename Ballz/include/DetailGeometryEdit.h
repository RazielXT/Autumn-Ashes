#pragma once

#include "MaterialEdit.h"

struct LoadedManualDG;

struct DetailGeometryEdit : public MaterialEdit
{
	DetailGeometryEdit() {}
	DetailGeometryEdit(LoadedManualDG* manualDG);
	virtual ~DetailGeometryEdit() {}

	std::vector<EditVariable> geometryParams;
	std::string dgName;

	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & dgName;
		ar & psVariablesMap;
		ar & geometryParams;
	}

	virtual EditVariables* getParams(const std::string& row) override;
	virtual void editChanged(EditVariable& var, const std::string& row) override;
	virtual void customAction(std::string name) override;

	void merge(DetailGeometryEdit& r, bool addNotExisting);

	static DetailGeometryEdit* query();
	static void applyChanges(std::map < std::string, DetailGeometryEdit >& changes);

protected:

	virtual void resetMaterial() override;
	virtual void materialChanged() override;

	Ogre::StaticGeometry* sg;

	std::map<std::string, std::vector<EditVariable>> psVariablesMap;

	std::vector<Ogre::MaterialPtr> matsArray;
	int currentMatId = 0;
};