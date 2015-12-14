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

    setVisible(false);
}

void GuiMaterialEdit::setVisible(bool show, int lvl)
{
    active = show;

    float alpha = show ? 1.0f : 0.0f;
    Ogre::ColourValue bckColor(0, 0, 0, show ? 0.8f : 0);

    for (size_t i = 0; i < 4; i++)
    {
        debugMaterialCaption[i]->colour(Ogre::ColourValue(1, 1, 1, alpha));
        debugVariablesCaption[i]->background(bckColor);
    }

    debugMaterialCaption[0]->colour(Ogre::ColourValue(1, 1, 1, alpha));

    if (lvl == 0) alpha = bckColor.a = 0;

    for (size_t i = 0; i < DEBUG_VARIABLES_COUNT; i++)
    {
        debugVariablesCaption[i]->colour(Ogre::ColourValue(1, 1, 1, alpha));
        debugVariablesCaption[i]->background(bckColor);
        debugVariablesCaption[i]->text("");
    }

    if (lvl == 1) alpha = bckColor.a = 0;

    for (size_t i = 0; i < DEBUG_VARIABLES_COUNT; i++)
    {
        debugVariableParamsCaption[i]->colour(Ogre::ColourValue(1, 1, 1, alpha));
        debugVariablesCaption[i]->background(bckColor);
        debugVariablesCaption[i]->text("");
    }
}

void GuiMaterialEdit::queryMaterial()
{
    if (matEdit.queryMaterial())
    {
        setVisible(true, 2);

        debugMaterialCaption[0]->text(matEdit.name);

        int i = 0;
        for (auto& v : matEdit.psVariables)
        {
            if (i < DEBUG_VARIABLES_COUNT)
                debugVariablesCaption[i++]->text(v.name);
        }

        for (i = 0; i < matEdit.psVariables[0].size; i++)
        {
            debugVariableParamsCaption[i]->text(std::to_string(matEdit.psVariables[0].buffer[i]));
        }
    }
    else
        setVisible(false);
}

GuiMaterialEdit::GuiMaterialEdit()
{

}

bool MaterialEdit::queryMaterial()
{
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
                name = mat->getName();
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
                        MaterialVariable var;
                        var.name = c.first;
                        var.size = c.second.constType;
                        memcpy(var.buffer, params->getFloatPointer(c.second.physicalIndex), 4 * var.size);

                        psVariables.push_back(var);
                    }
                }

                return psVariables.size() > 0;
            }
        }
    }

    return false;
}