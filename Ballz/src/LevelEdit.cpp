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
	applyPostProcessing();
	applyFog();

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
	originName = Global::gameMgr->getCurrentLvlInfo()->name;

	level.fogColor = Ogre::ColourValue(0.5f, 0.55f, 0.65f, 1.0f);
	level.fogStartDistance = 80;
	level.fogEndDistance = 150;
	level.ColorShift = Ogre::Vector4(1.08f, 1.12f, 1.16f, 1);
	level.ContSatuSharpNoise = 0;
	level.ambientColor = Ogre::ColourValue(0.35f, 0.35f, 0.35f);
	level.sunColor = Ogre::ColourValue(1, 1, 1);
	level.bloomStr = 1;
	level.bloomDepth = 0.35f;

	rows = { { originName,EditRow::Caption } ,{ "Save",EditRow::Action },{ "Temp",EditRow::Params },{ "PP",EditRow::Params } , { "Scene",EditRow::Params } };
}

static float yaw;
static float pitch;

static void mergeDefaults(std::vector<EditVariable>& from, std::vector<EditVariable>& target)
{
	for (auto& var : from)
	{
		bool found = false;

		for (auto& mvar : target)
		{
			if (mvar.name == var.name)
			{
				found = true;
			}
		}

		if(!found)
			target.push_back(var);
	}

	from.clear();
}

void LevelEdit::generateParams()
{
	std::vector<EditVariable> temp;
	temp.push_back({ "BloomAdd" , level.bloomAdd });
	temp.push_back({ "BloomSize" , level.bloomSize });
	temp.push_back({ "BloomDepth" , level.bloomDepth });
	temp.push_back({ "BloomStr" , level.bloomStr });
	temp.push_back({ "ColorShift" , level.ColorShift, 0.01f });
	temp.push_back({ "ContSatSharpNoise", level.ContSatuSharpNoise, 0.01f });
	mergeDefaults(temp, ppVariables);

	temp.push_back({ "Ambient", level.ambientColor, 0.025f });
	temp.push_back({ "FogColor", level.fogColor, 0.025f });
	temp.push_back({ "FogStartEnd", level.fogStartDistance, level.fogEndDistance});
	temp.push_back({ "SunColor", level.sunColor, 0.025f });
	mergeDefaults(temp, envVariables);

	auto& sunOr = Global::sceneMgr->getLight("Sun")->getParentSceneNode()->getOrientation();
	EditVariable var( "SunDir", sunOr.getPitch().valueDegrees(), sunOr.getYaw().valueDegrees());
	var.step = 5.0f;
	temp.push_back(var);
	yaw = var.buffer[0];
	pitch = var.buffer[1];
	mergeDefaults(temp, tempVariables);
}

void LevelEdit::applyPPEdit(EditVariable& var)
{
	if (var.name == "BloomAdd")
		level.bloomAdd = var.buffer[0];
	if (var.name == "BloomSize")
		level.bloomSize = var.buffer[0];
	if (var.name == "BloomDepth")
		level.bloomDepth = var.buffer[0];
	if (var.name == "BloomStr")
		level.bloomStr = var.buffer[0];
	if (var.name == "ColorShift")
		level.ColorShift = Ogre::Vector4(var.buffer);
	if (var.name == "ContSatSharpNoise")
		level.ContSatuSharpNoise = Ogre::Vector4(var.buffer);

	applyPostProcessing();
}

void LevelEdit::applySceneEdit(EditVariable& var)
{
	if (var.name == "Ambient")
	{
		level.ambientColor = Ogre::ColourValue(var.buffer[0], var.buffer[1], var.buffer[2]);
		Global::sceneMgr->setAmbientLight(level.ambientColor);
	}
	if (var.name == "FogColor")
	{
		level.fogColor = Ogre::ColourValue(var.buffer[0], var.buffer[1], var.buffer[2]);

		applyFog();
	}
	if (var.name == "FogStartEnd")
	{
		level.fogStartDistance = var.buffer[0];
		level.fogEndDistance = var.buffer[1];

		applyFog();
	}
	if (var.name == "SunColor")
	{
		auto sun = Global::sceneMgr->getLight("Sun");

		level.sunColor = Ogre::ColourValue(var.buffer[0], var.buffer[1], var.buffer[2]);
		sun->setDiffuseColour(level.sunColor);
	}
	if (var.name == "SunDir")
	{
		auto sunNode = Global::sceneMgr->getLight("Sun")->getParentSceneNode();
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

void LevelEdit::applyPostProcessing()
{
	PostProcessMgr* postProcMgr = Global::ppMgr;

	postProcMgr->vars.ColouringShift = level.ColorShift;
	postProcMgr->vars.ContSatuSharpNoise = level.ContSatuSharpNoise;
	postProcMgr->vars.bloomStrDepAddSize.z = level.bloomAdd;
	postProcMgr->vars.bloomStrDepAddSize.x = level.bloomStr;
	postProcMgr->vars.bloomStrDepAddSize.y = level.bloomDepth;
	postProcMgr->vars.bloomStrDepAddSize.w = level.bloomSize;
}

void LevelEdit::applyFog()
{
	Global::sceneMgr->setFog(Ogre::FOG_LINEAR, level.fogColor, 1, level.fogStartDistance, level.fogEndDistance);
}