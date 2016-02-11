#include "stdafx.h"
#include "GuiMaterialEdit.h"
#include "MUtils.h"
#include "GUtils.h"
#include "Player.h"
#include "GameStateManager.h"

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
            if (particleMode)
            {
                if (curMatEdit.changed)
                    Global::gameMgr->materialEdits.addParticleEdit(curMatEdit.edit, curMatEdit.ps->getName());
                else
                    Global::gameMgr->materialEdits.removeParticleEdit(curMatEdit.ps->getName());
            }
            else
            {
                if (curMatEdit.changed)
                    Global::gameMgr->materialEdits.addEdit(curMatEdit.edit, curMatEdit.entity->getName());
                else
                    Global::gameMgr->materialEdits.removeEdit_(curMatEdit.entity->getName());
            }

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
            if (particleMode && activeBaseId == 2)
                activeVarId = (activeVarId + 1) % curMatEdit.edit.moreParams.size();
            else
                activeVarId = (activeVarId + 1) % curMatEdit.edit.psVariables.size();

            activeParamId = 0;
        }
        if (activeLvl == 3)
        {
            if (particleMode && activeBaseId == 2)
                activeParamId = (activeParamId + 1) % curMatEdit.edit.moreParams[activeVarId].size;
            else
                activeParamId = (activeParamId + 1) % curMatEdit.edit.psVariables[activeVarId].size;
        }

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

            if (particleMode && activeBaseId == 2)
            {
                if (activeVarId < 0)
                    activeVarId += curMatEdit.edit.moreParams.size();

                activeVarId = activeVarId % curMatEdit.edit.moreParams.size();
            }
            else
            {
                if (activeVarId < 0)
                    activeVarId += curMatEdit.edit.psVariables.size();

                activeVarId = activeVarId % curMatEdit.edit.psVariables.size();
            }

            activeParamId = 0;
        }
        if (activeLvl == 3)
        {
            activeParamId = (activeParamId - 1);

            if (particleMode && activeBaseId == 2)
            {
                if (activeParamId < 0)
                    activeParamId += curMatEdit.edit.moreParams[activeVarId].size;

                activeParamId = activeParamId % curMatEdit.edit.moreParams[activeVarId].size;
            }
            else
            {
                if (activeParamId < 0)
                    activeParamId += curMatEdit.edit.psVariables[activeVarId].size;

                activeParamId = activeParamId % curMatEdit.edit.psVariables[activeVarId].size;
            }
        }
        break;
    case OIS::KC_SUBTRACT:
        if (activeLvl == 3)
        {
            if (particleMode && activeBaseId == 2)
            {
                curMatEdit.edit.moreParams[activeVarId].buffer[activeParamId] -= 0.1f;
                curMatEdit.edit.moreParams[activeVarId].edited = true;
                curMatEdit.setParticleParam(curMatEdit.edit.psVariables[activeVarId]);
            }
            else
            {
                curMatEdit.materialChanged();
                curMatEdit.edit.psVariables[activeVarId].buffer[activeParamId] -= 0.1f;
                curMatEdit.edit.psVariables[activeVarId].edited = true;
                curMatEdit.setMaterialParam(curMatEdit.edit.psVariables[activeVarId]);
            }
        }
        break;
    case OIS::KC_ADD:
        if (activeLvl == 3)
        {
            if (particleMode && activeBaseId == 2)
            {
                curMatEdit.edit.moreParams[activeVarId].buffer[activeParamId] += 0.1f;
                curMatEdit.edit.moreParams[activeVarId].edited = true;
                curMatEdit.setParticleParam(curMatEdit.edit.psVariables[activeVarId]);
            }
            else
            {
                curMatEdit.materialChanged();
                curMatEdit.edit.psVariables[activeVarId].buffer[activeParamId] += 0.1f;
                curMatEdit.edit.psVariables[activeVarId].edited = true;
                curMatEdit.setMaterialParam(curMatEdit.edit.psVariables[activeVarId]);
            }
        }
        break;
    case OIS::KC_MULTIPLY:
        if (activeLvl == 3)
        {
            if (particleMode && activeBaseId == 2)
            {
                curMatEdit.edit.moreParams[activeVarId].buffer[activeParamId] *= 1.5f;
                curMatEdit.edit.moreParams[activeVarId].edited = true;
                curMatEdit.setParticleParam(curMatEdit.edit.moreParams[activeVarId]);
            }
            else
            {
                curMatEdit.materialChanged();
                curMatEdit.edit.psVariables[activeVarId].buffer[activeParamId] *= 1.5f;
                curMatEdit.edit.psVariables[activeVarId].edited = true;
                curMatEdit.setMaterialParam(curMatEdit.edit.psVariables[activeVarId]);
            }
        }
        break;
    case OIS::KC_DIVIDE:
        if (activeLvl == 3)
        {
            if (particleMode && activeBaseId == 2)
            {
                curMatEdit.edit.moreParams[activeVarId].buffer[activeParamId] /= 1.5f;
                curMatEdit.edit.moreParams[activeVarId].edited = true;
                curMatEdit.setParticleParam(curMatEdit.edit.moreParams[activeVarId]);
            }
            else
            {
                curMatEdit.materialChanged();
                curMatEdit.edit.psVariables[activeVarId].buffer[activeParamId] /= 1.5f;
                curMatEdit.edit.psVariables[activeVarId].edited = true;
                curMatEdit.setMaterialParam(curMatEdit.edit.psVariables[activeVarId]);
            }
        }
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
    if (curMatEdit.queryWorld())
    {
        if (Global::gameMgr->materialEdits.loadSavedChanges(curMatEdit.edit, curMatEdit.entity->getName()))
            curMatEdit.changed = true;

        particleMode = false;
        setVisible(1);

        debugMaterialCaption[0]->text(curMatEdit.edit.originMatName);

        updateState();
    }
    else
        setVisible(0);
}

void GuiMaterialEdit::queryParticle()
{
    if (curMatEdit.queryParticle())
    {
        if (Global::gameMgr->materialEdits.loadSavedParticleChanges(curMatEdit.edit, curMatEdit.ps->getName()))
            curMatEdit.changed = true;

        particleMode = true;
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
    if (particleMode)
        debugMaterialCaption[2]->text("Particle");
    else
        debugMaterialCaption[2]->text("VS");

    if (activeLvl > 1 && activeBaseId == 3 && curMatEdit.edit.psVariables.size() > 0)
    {
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
    else if (activeLvl > 1 && activeBaseId == 2 && particleMode && curMatEdit.edit.moreParams.size() > 0)
    {
        for (int i = 0; i < DEBUG_VARIABLES_COUNT; i++)
        {
            int id = (activeVarId - selectedOffset + i);

            id = (id < 0) ? id + curMatEdit.edit.moreParams.size() : id % curMatEdit.edit.moreParams.size();

            debugVariablesCaption[i]->text(curMatEdit.edit.moreParams[id].name);
        }

        auto& var = curMatEdit.edit.moreParams[activeVarId];

        if (activeLvl > 1 && var.size > 0)
            for (int i = 0; i < DEBUG_VARIABLES_COUNT; i++)
            {
                if (i == var.size)
                    break;

                debugVariableParamsCaption[i]->text(std::to_string(var.buffer[i]));
            }
    }
}

GuiMaterialEdit::GuiMaterialEdit()
{
}

void LoadedMaterialEdit::reset()
{
    ptr.setNull();
    ps = nullptr;
    entity = nullptr;
    changed = false;
    matInstance = false;

    edit.psVariables.clear();
    edit.vsVariables.clear();
}

bool LoadedMaterialEdit::queryWorld()
{
    reset();

    GUtils::RayInfo out;
    if (GUtils::getRayInfo(Global::player->getCameraPosition(), Global::player->getFacingDirection(), 1000, out))
    {
        auto node = static_cast<Ogre::SceneNode*>(out.body->getOgreNode());

        if (node && node->numAttachedObjects() > 0)
        {
            auto ent = static_cast<Ogre::Entity*>(node->getAttachedObject(0));
            auto mat = ent->getSubEntity(0)->getMaterial();

            if (mat->getTechnique(0)->getNumPasses() > 0)
            {
                entity = ent;
                ptr = mat;

                loadMaterial();

                return edit.psVariables.size() > 0;
            }
        }
    }

    return false;
}

void LoadedMaterialEdit::resetMaterial()
{
    entity->setMaterialName(edit.originMatName);
    changed = false;
    matInstance = false;
    ptr = entity->getSubEntity(0)->getMaterial();
}

void LoadedMaterialEdit::materialChanged()
{
    if (!changed)
    {
        changed = true;

        auto newMat = ptr->clone(ptr->getName() + std::to_string(idCounter++));

        if (entity)
            entity->setMaterial(newMat);

        if (ps)
            ps->setMaterialName(newMat->getName());

        ptr = newMat;
    }
}

bool LoadedMaterialEdit::queryParticle()
{
    ps = Global::gameMgr->particleMgr.getClosestParticle();

    if (!ps || ps->getNumEmitters() == 0)
        return false;

    ptr = Ogre::MaterialManager::getSingleton().getByName(ps->getMaterialName());

    loadMaterial();
    edit.generateParticleParams(ps);

    return true;
}

void LoadedMaterialEdit::loadMaterial()
{
    edit.psVariables.clear();
    edit.originMatName = ptr->getName();
    int pass = ptr->getTechnique(0)->getNumPasses() - 1;
    auto params = ptr->getTechnique(0)->getPass(pass)->getFragmentProgramParameters();
    auto& l = params->getConstantDefinitions();
    bool skip = true;

    for (auto c : l.map)
    {
        skip = !skip;
        if (skip) continue;

        if (c.second.constType <= 4)
        {
            MaterialEdit_::MaterialVariable var;
            var.name = c.first;
            var.size = c.second.constType;
            memcpy(var.buffer, params->getFloatPointer(c.second.physicalIndex), 4 * var.size);

            edit.psVariables.push_back(var);
        }
    }
}

void LoadedMaterialEdit::setMaterialParam(MaterialEdit_::MaterialVariable& var)
{
    edit.setMaterialParam(ptr, var);
}

void LoadedMaterialEdit::setParticleParam(MaterialEdit_::MaterialVariable& var)
{
    edit.setParticleParam(ps, var);

    auto children = Global::gameMgr->materialEdits.particleSiblings.getChildren(ps->getName());
    for (auto child : children)
    {
        edit.setParticleParam(child, var);
    }
}
