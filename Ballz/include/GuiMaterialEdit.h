#pragma once

#include "Gorilla.h"

struct MaterialEdit
{
	struct MaterialVariable
	{
		std::string name;
		int size = 0;
		float buffer[4] = { 0 };
		bool changed = false;
	};

	std::string name;
	std::vector<MaterialVariable> psVariables;
	std::vector<MaterialVariable> vsVariables;
	bool instanced = false;

	Ogre::MaterialPtr ptr;

	bool queryMaterial();
};

class GuiMaterialEdit
{
public:

	GuiMaterialEdit();
	~GuiMaterialEdit()
	{
	}

	void initUi(Gorilla::Layer* layer);

	void update(float tslf);

	void pressedKey(const OIS::KeyEvent &arg);

	void setVisible(bool show, int lvl = 0);

	void queryMaterial();

private:

	bool active = false;
	MaterialEdit matEdit;

	Gorilla::Caption*       debugMaterialCaption[4];
	Gorilla::Caption*       debugVariablesCaption[DEBUG_VARIABLES_COUNT];
	Gorilla::Caption*       debugVariableParamsCaption[DEBUG_VARIABLES_COUNT];

};
