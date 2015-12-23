#pragma once

#include "Gorilla.h"
#include "MaterialEditsLibrary.h"

struct LoadedMaterialEdit
{
    MaterialEdit edit;

    bool changed = false;
    bool matInstance = false;
    Ogre::MaterialPtr ptr;
    Ogre::Entity* entity;
    int idCounter = 5000;

    void materialChanged();
    void resetMaterial();
    void reset();
    bool queryWorld();
    void setMaterialParam(MaterialEdit::MaterialVariable& var);
};

class GuiMaterialEdit
{
public:

    GuiMaterialEdit();
    ~GuiMaterialEdit()
    {
    }

    void initUi(Gorilla::Layer* layer);

    bool pressedKey(const OIS::KeyEvent &arg);

    void setVisible(int lvl = 1);

    void queryMaterial();

    void updateState();

    void updateText();

private:

    int selectedOffset = 3;

    int activeLvl = 0;

    int activeBaseId = 3;
    int activeVarId = 0;
    int activeParamId = 0;

    LoadedMaterialEdit curMatEdit;

    Gorilla::Caption*       debugMaterialCaption[4];
    Gorilla::Caption*       debugVariablesCaption[DEBUG_VARIABLES_COUNT];
    Gorilla::Caption*       debugVariableParamsCaption[DEBUG_VARIABLES_COUNT];

};
