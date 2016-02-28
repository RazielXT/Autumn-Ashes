#pragma once

#include "Edit.h"


struct MaterialEdit : public Edit
{
	MaterialEdit() {}
	MaterialEdit(Ogre::Entity* ent);
	virtual ~MaterialEdit() {}

	std::vector<EditVariable> psVariables;
	std::vector<EditVariable> vsVariables;

	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & originName;
		ar & psVariables;
		ar & vsVariables;
	}

	virtual EditVariables* getParams(const std::string& row) override;
	virtual void editChanged(EditVariable& var, const std::string& row) override;
	virtual void customAction(std::string name) override;

	void merge(MaterialEdit& r, bool addNotExisting);

	static MaterialEdit* query();
	static void applyChanges(const std::map < std::string, MaterialEdit >& changes);

	static void applyMaterialChanges(Ogre::MaterialPtr mat, const MaterialEdit& changes);

protected:

	std::vector<EditVariable> generateVsParams(Ogre::MaterialPtr matPtr);
	std::vector<EditVariable> generatePsParams(Ogre::MaterialPtr matPtr);

	std::vector<EditVariable> generateShaderEditParams(Ogre::GpuProgramParametersSharedPtr gpuParams);

	void loadMaterial();

	virtual void resetMaterial();
	virtual void materialChanged();

	bool changedMaterial = false;
	bool matInstance = false;
	Ogre::MaterialPtr materialPtr;
	Ogre::Entity* entity = nullptr;
};