#include "stdafx.h"
#include "GuiMaterialEdit.h"
#include "MUtils.h"
#include "Player.h"

void GuiMaterialEdit::initUi(Gorilla::Layer* layer)
{
    float uiBaseX = 50;
    float uiBaseY = 500;

    float uiWidth = 500;
    float uiHeight = 50;
    float uiSizeScale = 0.3f;

    float uiStepHeight = uiHeight*uiSizeScale;
    float uiRowOffset = uiWidth*uiSizeScale + 50;

    for (size_t i = 0; i < 4; i++)
    {
        debugMaterialCaption[i] = layer->createCaption(48, uiBaseX, uiBaseY + uiStepHeight * (float)i, "");
        debugMaterialCaption[i]->size(uiWidth, uiHeight);
        debugMaterialCaption[i]->setScale(uiSizeScale);
        debugMaterialCaption[i]->align(Gorilla::TextAlign_Left);
    }

    debugMaterialCaption[0]->text("");
    debugMaterialCaption[1]->text("Save");
    debugMaterialCaption[2]->text("VS");
    debugMaterialCaption[3]->text("PS");

    for (size_t i = 0; i < DEBUG_VARIABLES_COUNT; i++)
    {
        debugVariablesCaption[i] = layer->createCaption(48, uiBaseX + uiRowOffset, uiBaseY + uiStepHeight * (float)i, "");
        debugVariablesCaption[i]->size(uiWidth, uiHeight);
        debugVariablesCaption[i]->setScale(uiSizeScale);
        debugVariablesCaption[i]->align(Gorilla::TextAlign_Left);
    }

    for (size_t i = 0; i < DEBUG_VARIABLES_COUNT; i++)
    {
        debugVariableParamsCaption[i] = layer->createCaption(48, uiBaseX + uiRowOffset*2, uiBaseY + uiStepHeight * (float)i, "");
        debugVariableParamsCaption[i]->size(uiWidth, uiHeight);
        debugVariableParamsCaption[i]->setScale(uiSizeScale);
        debugVariableParamsCaption[i]->align(Gorilla::TextAlign_Left);
    }

    setVisible(0);
}

bool GuiMaterialEdit::pressedKey(const OIS::KeyEvent &arg)
{
    if (activeLvl == 0)
        return false;

    switch (arg.key)
    {
    case OIS::KC_LEFT:
        activeLvl = Ogre::Math::Clamp(activeLvl - 1, 1, 3);
        break;
    case OIS::KC_RIGHT:
        if (activeLvl == 1 && activeBaseId == 0)
        {
            curMatEdit.resetMaterial();
        }
        else if (activeLvl == 1 && activeBaseId == 1)
        {
            if (curMatEdit.changed)
                library.saveEdit(curMatEdit.edit, curMatEdit.entity->getName());
            else
                library.removeEdit(curMatEdit.entity->getName());
        }
        else
            activeLvl = Ogre::Math::Clamp(activeLvl + 1, 1, 3);
        break;
    case OIS::KC_DOWN:
        if (activeLvl == 1)
        {
            activeBaseId = (activeBaseId + 1) % 4;

        }
        if (activeLvl == 2)
        {
            activeVarId = (activeVarId + 1) % curMatEdit.edit.psVariables.size();
            activeParamId = 0;
        }
        if (activeLvl == 3)
            activeParamId = (activeParamId + 1) % curMatEdit.edit.psVariables[activeVarId].size;
        break;
    case OIS::KC_UP:
        if (activeLvl == 1)
        {
            activeBaseId = (activeBaseId - 1) % 4;

            if (activeBaseId < 0)
                activeBaseId += 4;

            activeParamId = 0;
        }
        if (activeLvl == 2)
        {
            activeVarId = (activeVarId - 1);

            if (activeVarId < 0)
                activeVarId += curMatEdit.edit.psVariables.size();

            activeVarId = activeVarId % curMatEdit.edit.psVariables.size();

            activeParamId = 0;
        }
        if (activeLvl == 3)
        {
            activeParamId = (activeParamId - 1);

            if (activeParamId < 0)
                activeParamId += curMatEdit.edit.psVariables[activeVarId].size;

            activeParamId = activeParamId % curMatEdit.edit.psVariables[activeVarId].size;
        }
        break;
    case OIS::KC_DIVIDE:
        curMatEdit.edit.psVariables[activeVarId].buffer[activeParamId] -= 0.1f;
        curMatEdit.edit.psVariables[activeVarId].edited = true;
        curMatEdit.changed = true;
        curMatEdit.setMaterialParam(curMatEdit.edit.psVariables[activeVarId]);
        break;
    case OIS::KC_MULTIPLY:
        curMatEdit.edit.psVariables[activeVarId].buffer[activeParamId] += 0.1f;
        curMatEdit.edit.psVariables[activeVarId].edited = true;
        curMatEdit.changed = true;
        curMatEdit.setMaterialParam(curMatEdit.edit.psVariables[activeVarId]);
        break;
    default:
        return false;
    }

    updateState();

    return true;
}

void GuiMaterialEdit::setVisible(int lvl)
{
    activeLvl = lvl;
    bool show = lvl > 0;

    if (!show)
        curMatEdit.reset();

    float alpha = show ? 1.0f : 0.0f;
    Ogre::ColourValue bckColor(0, 0, 0, show ? 0.8f : 0);

    debugMaterialCaption[0]->colour(Ogre::ColourValue(0.7f, 1, 0.3f, alpha));
    debugMaterialCaption[1]->colour(Ogre::ColourValue(1, 0, 1, alpha));

    for (size_t i = 2; i < 4; i++)
    {
        debugMaterialCaption[i]->colour(Ogre::ColourValue(1, 1, 1, alpha));
        debugVariablesCaption[i]->background(bckColor);
    }

    debugMaterialCaption[activeBaseId]->colour(Ogre::ColourValue(1, activeLvl == 1 ? 0.0f : 1.0f, 0, alpha));

    if (lvl < 1) alpha = bckColor.a = 0;

    for (size_t i = 0; i < DEBUG_VARIABLES_COUNT; i++)
    {
        debugVariablesCaption[i]->colour(Ogre::ColourValue(1, 1, 1, alpha));
        debugVariablesCaption[i]->background(bckColor);
        debugVariablesCaption[i]->text("");
    }
    debugVariablesCaption[selectedOffset]->colour(Ogre::ColourValue(1, activeLvl == 2 ? 0.0f : 1.0f, 0, alpha));

    //if (lvl < 2) alpha = bckColor.a = 0;

    for (size_t i = 0; i < DEBUG_VARIABLES_COUNT; i++)
    {
        debugVariableParamsCaption[i]->colour(Ogre::ColourValue(1, 1, 1, alpha));
        debugVariableParamsCaption[i]->background(bckColor);
        debugVariableParamsCaption[i]->text("");
    }

    debugVariableParamsCaption[activeParamId]->colour(Ogre::ColourValue(1, activeLvl == 3 ? 0.0f : 1.0f, 0, alpha));
}

void GuiMaterialEdit::queryMaterial()
{
    if (curMatEdit.queryMaterial())
    {
        library.loadEdit(curMatEdit.edit, curMatEdit.entity->getName());

        setVisible(1);

        debugMaterialCaption[0]->text(curMatEdit.edit.originMatName);

        updateState();
    }
    else
        setVisible(0);
}

void GuiMaterialEdit::updateState()
{
    setVisible(activeLvl);

    if (activeLvl == 0)
        return;

    updateText();
}

void GuiMaterialEdit::updateText()
{
    if (activeLvl > 1 && curMatEdit.edit.psVariables.size() > 0)
        for (int i = 0; i < DEBUG_VARIABLES_COUNT; i++)
        {
            int id = (activeVarId - selectedOffset + i);

            id = (id < 0) ? id + curMatEdit.edit.psVariables.size() : id % curMatEdit.edit.psVariables.size();

            debugVariablesCaption[i]->text(curMatEdit.edit.psVariables[id].name);
        }

    auto& var = curMatEdit.edit.psVariables[activeVarId];

    if (activeLvl > 1 && var.size > 0)
        for (int i = 0; i < DEBUG_VARIABLES_COUNT; i++)
        {
            if (i == var.size)
                break;

            debugVariableParamsCaption[i]->text(std::to_string(var.buffer[i]));
        }
}

GuiMaterialEdit::GuiMaterialEdit()
{

}

void LoadedMaterialEdit::reset()
{
    ptr.setNull();
    entity = nullptr;
    changed = false;
    matInstance = false;
}

bool LoadedMaterialEdit::queryMaterial()
{
    reset();

    MUtils::RayInfo out;
    if (MUtils::getRayInfo(Global::player->getCameraPosition(), Global::player->getFacingDirection(), 1000, out))
    {
        auto node = static_cast<Ogre::SceneNode*>(out.body->getOgreNode());

        if (node && node->numAttachedObjects() > 0)
        {
            auto ent = static_cast<Ogre::Entity*>(node->getAttachedObject(0));
            auto mat = ent->getSubEntity(0)->getMaterial();

            if (mat->getTechnique(0)->getNumPasses() > 1)
            {
                edit.originMatName = mat->getName();
                entity = ent;

                ptr = mat;

                auto params = mat->getTechnique(0)->getPass(1)->getFragmentProgramParameters();
                auto& l = params->getConstantDefinitions();
                bool skip = true;

                for (auto c : l.map)
                {
                    skip = !skip;
                    if (skip) continue;

                    Global::DebugPrint(c.first);

                    if (c.second.constType <= 4)
                    {
                        MaterialEdit::MaterialVariable var;
                        var.name = c.first;
                        var.size = c.second.constType;
                        memcpy(var.buffer, params->getFloatPointer(c.second.physicalIndex), 4 * var.size);

                        edit.psVariables.push_back(var);
                    }
                }

                return edit.psVariables.size() > 0;
            }
        }
    }

    return false;
}

void LoadedMaterialEdit::setMaterialParam(MaterialEdit::MaterialVariable& var)
{
    if (ptr.isNull())
        return;

    ptr->getTechnique(0)->getPass(1)->getFragmentProgramParameters()->setNamedConstant(var.name, var.buffer, 1, var.size);
}

void LoadedMaterialEdit::resetMaterial()
{
    entity->setMaterialName(edit.originMatName);
    changed = false;
    matInstance = false;
    ptr = entity->getSubEntity(0)->getMaterial();
}
