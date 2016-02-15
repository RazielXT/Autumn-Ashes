#include "stdafx.h"
#include "LevelEdit.h"
#include "GameStateManager.h"

LevelEdit::LevelEdit()
{
    persistent = true;
}

EditVariables* LevelEdit::getParams(const std::string& row)
{
    if (row == "PP")
        return &ppVariables;
    if (row == "Scene")
        return &envVariables;

    return nullptr;
}

void LevelEdit::editChanged(EditVariable& var, const std::string& row)
{
    if (row == "PP")
    {
        mergeParams({ var }, ppVariables,  true);
        applyPPEdit(var);
    }
    if (row == "Scene")
    {
        mergeParams({ var }, envVariables, true);
        applySceneEdit(var);
    }
}

void LevelEdit::customAction(std::string name)
{
    if (name == "Save")
    {
        Global::gameMgr->sceneEdits.saveLevelEdit();
    }
}

LevelEdit* LevelEdit::query()
{
    auto lvl = Global::gameMgr->sceneEdits.getLevelEdit();

    return lvl;
}

void LevelEdit::applyChanges()
{
    for (auto& var : ppVariables)
    {
        applyPPEdit(var);
    }

    for (auto& var : envVariables)
    {
        applySceneEdit(var);
    }
}

void LevelEdit::clear()
{
    envVariables.clear();
    ppVariables.clear();
}

void LevelEdit::init()
{
    level = Global::gameMgr->getCurrentLvlInfo();
    originName = level->name;

    rows = { { originName,EditRow::Caption } ,{ "Save",EditRow::Action },{ "PP",EditRow::Params },{ "Scene",EditRow::Params } };

    generateParams();
}

void LevelEdit::generateParams()
{
    clear();

    ppVariables.push_back({ "BloomAdd" , level->bloomAdd });
    ppVariables.push_back({ "BloomSize" , level->bloomSize });
    ppVariables.push_back({ "BloomDepth" , level->bloomDepth });
    ppVariables.push_back({ "BloomStr" , level->bloomStr });
    ppVariables.push_back({ "ColorShift" , level->ColorShift });
    ppVariables.push_back({ "ContSatSharpNoise", level->ContSatuSharpNoise });

    envVariables.push_back({ "Ambient", level->ambientColor });
    envVariables.push_back({ "FogColor", level->fogColor });
    envVariables.push_back({ "FogStartEnd", level->fogStartDistance, level->fogEndDistance});
}

void LevelEdit::applyPPEdit(EditVariable& var)
{
    if (var.name == "BloomAdd")
        level->bloomAdd = var.buffer[0];
    if (var.name == "BloomSize")
        level->bloomSize = var.buffer[0];
    if (var.name == "BloomDepth")
        level->bloomDepth = var.buffer[0];
    if (var.name == "BloomStr")
        level->bloomStr = var.buffer[0];
    if (var.name == "ColorShift")
        level->ColorShift = Ogre::Vector4(var.buffer);
    if (var.name == "ContSatSharpNoise")
        level->ContSatuSharpNoise = Ogre::Vector4(var.buffer);

    level->applyPostProcessing();
}

void LevelEdit::applySceneEdit(EditVariable& var)
{
    if (var.name == "Ambient")
    {
        level->ambientColor = Ogre::ColourValue(var.buffer[0], var.buffer[1], var.buffer[2]);
        Global::mSceneMgr->setAmbientLight(level->ambientColor);
    }
    if (var.name == "FogColor")
    {
        level->ambientColor = Ogre::ColourValue(var.buffer[0], var.buffer[1], var.buffer[2]);

        level->applyFog();
    }
    if (var.name == "FogStartEnd")
    {
        level->fogStartDistance = var.buffer[0];
        level->fogEndDistance = var.buffer[1];

        level->applyFog();
    }
}

