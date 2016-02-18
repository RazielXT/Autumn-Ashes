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
    if (row == "Temp")
        return &tempVariables;

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
    if (row == "Temp")
    {
        mergeParams({ var }, tempVariables, true);
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
    tempVariables.clear();
}

void LevelEdit::init()
{
    level = Global::gameMgr->getCurrentLvlInfo();
    originName = level->name;

    rows = { { originName,EditRow::Caption } ,{ "Save",EditRow::Action },{ "Temp",EditRow::Params },{ "PP",EditRow::Params } , { "Scene",EditRow::Params } };

    generateParams();
}

static float yaw;
static float pitch;

void LevelEdit::generateParams()
{
    clear();

    ppVariables.push_back({ "BloomAdd" , level->bloomAdd });
    ppVariables.push_back({ "BloomSize" , level->bloomSize });
    ppVariables.push_back({ "BloomDepth" , level->bloomDepth });
    ppVariables.push_back({ "BloomStr" , level->bloomStr });
    ppVariables.push_back({ "ColorShift" , level->ColorShift, 0.01f });
    ppVariables.push_back({ "ContSatSharpNoise", level->ContSatuSharpNoise, 0.01f });

    envVariables.push_back({ "Ambient", level->ambientColor, 0.025f });
    envVariables.push_back({ "FogColor", level->fogColor, 0.025f });
    envVariables.push_back({ "FogStartEnd", level->fogStartDistance, level->fogEndDistance});

    auto& sunOr = Global::mSceneMgr->getLight("Sun")->getParentSceneNode()->getOrientation();
    EditVariable var( "SunDir", sunOr.getPitch().valueDegrees(), sunOr.getYaw().valueDegrees());
    var.step = 5.0f;
    tempVariables.push_back(var);
    yaw = var.buffer[0];
    pitch = var.buffer[1];
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
    if (var.name == "SunDir")
    {
        auto sunNode = Global::mSceneMgr->getLight("Sun")->getParentSceneNode();
        auto sunOr = sunNode->getOrientation();

        if (var.buffer[0] != yaw)
        {
            sunNode->yaw(Ogre::Degree(yaw- var.buffer[0]));
            var.buffer[0] = yaw;
        }

        if (var.buffer[1] != pitch)
        {
            sunNode->pitch(Ogre::Degree(pitch - var.buffer[1]));
            var.buffer[1] = pitch;
        }
    }
}

