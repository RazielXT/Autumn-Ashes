#include "stdafx.h"
#include "GuiSceneEdit.h"
#include "SceneEditsLibrary.h"
#include "MUtils.h"
#include "GUtils.h"
#include "Player.h"
#include "GameStateManager.h"

void GuiSceneEdit::initUi(Gorilla::Layer* layer)
{
    float uiBaseX = 50;
    float uiBaseY = 700;

    float uiWidth = 500;
    float uiHeight = 50;
    float uiSizeScale = 0.45f;

    float uiStepHeight = uiHeight*uiSizeScale;
    float uiRowOffset = uiWidth*uiSizeScale + 50;

    for (size_t i = 0; i < EDIT_BASE_ROWS_MAX; i++)
    {
        baseRowsCaption[i] = layer->createCaption(48, uiBaseX, uiBaseY + uiStepHeight * (float)i, "");
        baseRowsCaption[i]->size(uiWidth, uiHeight);
        baseRowsCaption[i]->setScale(uiSizeScale);
        baseRowsCaption[i]->align(Gorilla::TextAlign_Left);
    }

    for (size_t i = 0; i < EDIT_PARAMS_ROWS_MAX; i++)
    {
        variablesCaption[i] = layer->createCaption(48, uiBaseX + uiRowOffset, uiBaseY + uiStepHeight * (float)i, "");
        variablesCaption[i]->size(uiWidth, uiHeight);
        variablesCaption[i]->setScale(uiSizeScale);
        variablesCaption[i]->align(Gorilla::TextAlign_Left);
    }

    for (size_t i = 0; i < EDIT_PARAM_SIZE_MAX; i++)
    {
        variableParamsCaption[i] = layer->createCaption(48, uiBaseX + uiRowOffset * 2, uiBaseY + uiStepHeight * (float)i, "");
        variableParamsCaption[i]->size(uiWidth, uiHeight);
        variableParamsCaption[i]->setScale(uiSizeScale);
        variableParamsCaption[i]->align(Gorilla::TextAlign_Left);
    }

    setVisible(0);
}

bool GuiSceneEdit::pressedKey(const OIS::KeyEvent &arg)
{
    if (activeLvl == 0 || !currentEdit)
        return false;

    switch (arg.key)
    {
    case OIS::KC_LEFT:
    {
        activeLvl = Ogre::Math::Clamp(activeLvl - 1, 1, 3);
        if (activeLvl == 1)
        {
            activeVarId = 0;
            activeParamId = 0;
        }
    }
    break;
    case OIS::KC_RIGHT:
        if (activeLvl == 1 && base[activeBaseId].type == EditRow::Action)
        {
            currentEdit->customAction(base[activeBaseId].name);
        }
        else
        {
            if (activeLvl == 1 && base[activeBaseId].type == EditRow::Params)
            {
                activeVarId = 0;
                activeParamId = 0;
                currentVars = currentEdit->getParams(base[activeBaseId].name);
            }

            if(currentVars)
                activeLvl = Ogre::Math::Clamp(activeLvl + 1, 1, 3);
        }
        break;
    case OIS::KC_DOWN:
        if (activeLvl == 1)
        {
            activeBaseId = (activeBaseId + 1) % base.size();
        }
        if (activeLvl == 2)
        {
            activeVarId = (activeVarId + 1) % currentVars->size();

            activeParamId = 0;
        }
        if (activeLvl == 3)
        {
            activeParamId = (activeParamId + 1) % currentVars->at(activeVarId).size;
        }
        break;
    case OIS::KC_UP:
        if (activeLvl == 1)
        {
            activeBaseId = (activeBaseId - 1);

            if (activeBaseId < 0)
                activeBaseId += base.size();

            activeBaseId = activeBaseId % base.size();
        }
        if (activeLvl == 2)
        {
            activeVarId = (activeVarId - 1);

            if (activeVarId < 0)
                activeVarId += currentVars->size();

            activeVarId = activeVarId % currentVars->size();

            activeParamId = 0;
        }
        if (activeLvl == 3)
        {
            activeParamId = (activeParamId - 1);

            if (activeParamId < 0)
                activeParamId += currentVars->at(activeVarId).size;

            activeParamId = activeParamId % currentVars->at(activeVarId).size;
        }
        break;
    case OIS::KC_DELETE:
        if (activeLvl == 3)
        {
            auto& var = currentVars->at(activeVarId);
            for (int i = 0; i < var.size; i++)
                var.buffer[i] = 1.0f;
            currentEdit->editChanged(var, base[activeBaseId].name);
        }
        break;
    case OIS::KC_END:
        if (activeLvl == 3)
        {
            auto& var = currentVars->at(activeVarId);
            for (int i = 0; i < var.size; i++)
                var.buffer[i] = var.buffer[activeParamId];
            currentEdit->editChanged(var, base[activeBaseId].name);
        }
        break;
    case OIS::KC_SUBTRACT:
        if (activeLvl == 3)
        {
            auto& var = currentVars->at(activeVarId);
            var.buffer[activeParamId] -= var.step;
            currentEdit->editChanged(var, base[activeBaseId].name);
        }
        break;
    case OIS::KC_ADD:
        if (activeLvl == 3)
        {
            auto& var = currentVars->at(activeVarId);
            var.buffer[activeParamId] += var.step;
            currentEdit->editChanged(var, base[activeBaseId].name);
        }
        break;
    case OIS::KC_MULTIPLY:
        if (activeLvl == 3)
        {
            auto& var = currentVars->at(activeVarId);
            var.buffer[activeParamId] *= 1.5f;
            currentEdit->editChanged(var, base[activeBaseId].name);
        }
        break;
    case OIS::KC_DIVIDE:
        if (activeLvl == 3)
        {
            auto& var = currentVars->at(activeVarId);
            var.buffer[activeParamId] /= 1.5f;
            currentEdit->editChanged(var, base[activeBaseId].name);
        }
        break;
    default:
        return false;
    }

    updateState();

    return true;
}

void GuiSceneEdit::setVisible(int lvl)
{
    activeLvl = lvl;
    bool show = lvl > 0;

    if (!show)
    {
        delete currentEdit;
        currentEdit = nullptr;
    }

    float alpha = show ? 1.0f : 0.0f;
    Ogre::ColourValue bckColor(0, 0, 0, show ? 0.8f : 0);

    updateBase();

    baseRowsCaption[activeBaseId]->colour(Ogre::ColourValue(1, activeLvl == 1 ? 0.0f : 1.0f, 0, alpha));

    if (lvl < 1) alpha = bckColor.a = 0;

    for (size_t i = 0; i < EDIT_PARAMS_ROWS_MAX; i++)
    {
        variablesCaption[i]->colour(Ogre::ColourValue(1, 1, 1, alpha));
        variablesCaption[i]->background(bckColor);
        variablesCaption[i]->text("");
    }
    variablesCaption[selectedOffset]->colour(Ogre::ColourValue(1, activeLvl == 2 ? 0.0f : 1.0f, 0, alpha));

    //if (lvl < 2) alpha = bckColor.a = 0;

    for (size_t i = 0; i < EDIT_PARAM_SIZE_MAX; i++)
    {
        variableParamsCaption[i]->colour(Ogre::ColourValue(1, 1, 1, alpha));
        variableParamsCaption[i]->background(bckColor);
        variableParamsCaption[i]->text("");
    }

    variableParamsCaption[activeParamId]->colour(Ogre::ColourValue(1, activeLvl == 3 ? 0.0f : 1.0f, 0, alpha));
}

void GuiSceneEdit::queryLevel()
{
    if (currentEdit && !currentEdit->persistent)
        delete currentEdit;

    currentEdit = LevelEdit::query();

    setVisible(1);
    updateState();
}

void GuiSceneEdit::queryDetailGeometry()
{
    if (currentEdit && !currentEdit->persistent)
        delete currentEdit;

    currentEdit = DetailGeometryEdit::query();

    if (currentEdit)
    {
        setVisible(1);
        updateState();
    }
    else
        setVisible(0);
}

void GuiSceneEdit::queryMaterial()
{
    if (currentEdit && !currentEdit->persistent)
        delete currentEdit;

    currentEdit = MaterialEdit::query();

    if (currentEdit)
    {
        setVisible(1);
        updateState();
    }
    else
        setVisible(0);
}

void GuiSceneEdit::queryOptGroup()
{
    if (currentEdit && !currentEdit->persistent)
        delete currentEdit;

    currentEdit = OptimizedGroupEdit::query();

    if (currentEdit)
    {
        setVisible(1);
        updateState();
    }
    else
        setVisible(0);
}

void GuiSceneEdit::queryParticle()
{
    if (currentEdit && !currentEdit->persistent)
        delete currentEdit;

    currentEdit = ParticleEdit::query();

    if (currentEdit)
    {
        setVisible(1);
        updateState();
    }
    else
        setVisible(0);
}

void GuiSceneEdit::updateBase()
{
    for (size_t i = 0; i < EDIT_BASE_ROWS_MAX; i++)
    {
        baseRowsCaption[i]->text("");
    }

    if (currentEdit)
    {
        base = currentEdit->getBaseRows();

        for (size_t i = 0; i < base.size(); i++)
        {
            baseRowsCaption[i]->text(base[i].name);

            if (base[i].type == EditRow::Caption)
                baseRowsCaption[i]->colour(Ogre::ColourValue(0.3f, 0.85f, 1.0, 1));
            else if (base[i].type == EditRow::Params)
                baseRowsCaption[i]->colour(Ogre::ColourValue(1, 1, 1, 1));
            else if (base[i].type == EditRow::Action)
                baseRowsCaption[i]->colour(Ogre::ColourValue(0.3f, 1, 0.2f, 1));
            else
                baseRowsCaption[i]->colour(Ogre::ColourValue(0.2f, 0.6f, 0.6f, 1));
        }
    }
}

void GuiSceneEdit::updateState()
{
    setVisible(activeLvl);
    updateText();
}

void GuiSceneEdit::updateText()
{
    if (activeLvl > 1)
    {
        for (int i = 0; i < EDIT_PARAMS_ROWS_MAX; i++)
        {
            int id = (activeVarId - selectedOffset + i);

            id = (id < 0) ? std::max<int>(0,id + currentVars->size()) : id % currentVars->size();

            variablesCaption[i]->text(currentVars->at(id).name);
        }

        auto& var = currentVars->at(activeVarId);

        if (var.size > 0)
            for (int i = 0; i < EDIT_PARAM_SIZE_MAX; i++)
            {
                if (i == var.size)
                    break;

                variableParamsCaption[i]->text(std::to_string(var.buffer[i]));
            }
    }
}
