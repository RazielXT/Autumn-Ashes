#pragma once

#include "Gorilla.h"

struct MaterialEdit
{
    struct MaterialVariable
    {
        std::string name;
        int size = 0;
        float buffer[4];
        bool changed = false;
    };

    std::string name;
    std::vector<MaterialVariable> psVariables;
    std::vector<MaterialVariable> vsVariables;
    bool instanced = false;

    Ogre::MaterialPtr ptr;

	void reset();
    bool queryMaterial();
	void setMaterialParam(MaterialVariable& var);
};

class GuiMaterialEdit
{
public:

    GuiMaterialEdit();
    ~GuiMaterialEdit()
    {
    }

    void initUi(Gorilla::Layer* layer);

    void pressedKey(const OIS::KeyEvent &arg);

    void setVisible(int lvl = 0);

    void queryMaterial();

	void updateState();

	void updateText();

private:

	int selectedOffset = 3;

	int activeLvl = 0;

	int activeBaseId = 0;
	int activeVarId = 0;
	int activeParamId = 0;

    MaterialEdit matEdit;

    Gorilla::Caption*       debugMaterialCaption[4];
    Gorilla::Caption*       debugVariablesCaption[DEBUG_VARIABLES_COUNT];
    Gorilla::Caption*       debugVariableParamsCaption[DEBUG_VARIABLES_COUNT];

};
