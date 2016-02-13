#pragma once

#include "Gorilla.h"
#include "SceneEditsLibrary.h"

#define EDIT_BASE_ROWS_MAX 5
#define EDIT_PARAMS_ROWS_MAX 7
#define EDIT_PARAM_SIZE_MAX 4

class GuiSceneEdit
{
public:

    void initUi(Gorilla::Layer* layer);
    bool pressedKey(const OIS::KeyEvent &arg);

    void queryMaterial();
    void queryParticle();

    void setVisible(int lvl = 1);
    void updateState();
    void updateText();

private:

    void updateBase();

    EditBaseRows base;
    EditVariables* currentVars;
    Edit* currentEdit;

    int selectedOffset = 3;

    int activeLvl = 0;

    int activeBaseId = 3;
    int activeVarId = 0;
    int activeParamId = 0;

    Gorilla::Caption*       baseRowsCaption[EDIT_BASE_ROWS_MAX];
    Gorilla::Caption*       variablesCaption[EDIT_PARAMS_ROWS_MAX];
    Gorilla::Caption*       variableParamsCaption[EDIT_PARAM_SIZE_MAX];

};
