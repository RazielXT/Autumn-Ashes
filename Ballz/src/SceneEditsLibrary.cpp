#include "stdafx.h"
#include "SceneEditsLibrary.h"
#include "GameStateManager.h"

#include <iostream>
#include "SUtils.h"


void SceneEditsLibrary::addMaterialEdit(MaterialEdit& edit, std::string entName)
{
	auto& ent = materialEditHistory.data[entName];

	if (ent.originName.empty())
		ent.originName = edit.originName;

	ent.merge(edit, true);

	saveMaterialHistory(Global::gameMgr->getCurrentLvlInfo()->path);
}

void SceneEditsLibrary::removeMaterialEdit(std::string entName)
{
	auto& ent = materialEditHistory.data.begin();

	while (ent != materialEditHistory.data.end())
	{
		if (ent->first == entName)
		{
			materialEditHistory.data.erase(ent);
			break;
		}
	}

	saveMaterialHistory(Global::gameMgr->getCurrentLvlInfo()->path);
}

bool SceneEditsLibrary::loadSavedMaterialChanges(MaterialEdit& edit, std::string entName)
{
	auto& ent = materialEditHistory.data.begin();

	while (ent != materialEditHistory.data.end())
	{
		if (ent->first == entName)
		{
			//must have at least copy name of saved origin
			if (!SUtils::startsWith(edit.originName, ent->second.originName))
			{
				//old save, erase it
				materialEditHistory.data.erase(ent);
				return false;
			}
			else
				edit.merge(ent->second, false);

			return true;
		}

		ent++;
	}

	return false;
}

void SceneEditsLibrary::loadChanges()
{
	auto path = Global::gameMgr->getCurrentLvlInfo()->path;

	loadMaterialHistory(path);
	MaterialEdit::applyChanges(materialEditHistory.data);

	loadParticleHistory(path);
	ParticleEdit::applyChanges(particleEditHistory.data);

	loadLevelEdit(path);
	levelEdits.applyChanges();

	loadOgHistory(path);
	OptimizedGroupEdit::applyChanges(ogEditHistory.data);

	loadDgHistory(path);
	DetailGeometryEdit::applyChanges(dgEditHistory.data);
}

void SceneEditsLibrary::clear()
{
	materialEditHistory.data.clear();
	particleEditHistory.data.clear();
	ParticleEdit::particleChildren.children.clear();
}

void SceneEditsLibrary::saveLevelEdit(std::string path)
{
	std::ofstream ofs(path + "Level.edit", std::ios::binary);
	boost::archive::binary_oarchive oa(ofs);
	oa << levelEdits;
}

void SceneEditsLibrary::saveLevelEdit()
{
	saveLevelEdit(Global::gameMgr->getCurrentLvlInfo()->path);
}

void SceneEditsLibrary::loadLevelEdit(std::string path)
{
	levelEdits.init();

	std::ifstream ifs(path + "Level.edit", std::ios::binary);

	if (ifs.good())
	{
		boost::archive::binary_iarchive ia(ifs);

		ia >> levelEdits;
	}
}

void SceneEditsLibrary::saveMaterialHistory(std::string path)
{
	std::ofstream ofs(path + "Materials.edit", std::ios::binary);
	boost::archive::binary_oarchive oa(ofs);
	oa << materialEditHistory;
}

void SceneEditsLibrary::loadMaterialHistory(std::string path)
{
	std::ifstream ifs(path + "Materials.edit", std::ios::binary);

	if (ifs.good())
	{
		boost::archive::binary_iarchive ia(ifs);

		ia >> materialEditHistory;
	}
}

void SceneEditsLibrary::saveParticleHistory(std::string path)
{
	std::ofstream ofs(path + "Particles.edit", std::ios::binary);
	boost::archive::binary_oarchive oa(ofs);
	oa << particleEditHistory;
}

void SceneEditsLibrary::loadParticleHistory(std::string path)
{
	std::ifstream ifs(path + "Particles.edit", std::ios::binary);

	if (ifs.good())
	{
		boost::archive::binary_iarchive ia(ifs);

		ia >> particleEditHistory;
	}
}

void SceneEditsLibrary::saveDgHistory(std::string path)
{
	std::ofstream ofs(path + "DetailGeometries.edit", std::ios::binary);
	boost::archive::binary_oarchive oa(ofs);
	oa << dgEditHistory;
}

void SceneEditsLibrary::loadDgHistory(std::string path)
{
	std::ifstream ifs(path + "DetailGeometries.edit", std::ios::binary);

	if (ifs.good())
	{
		boost::archive::binary_iarchive ia(ifs);

		ia >> dgEditHistory;
	}
}

void SceneEditsLibrary::saveOgHistory(std::string path)
{
	std::ofstream ofs(path + "OptimizedGroups.edit", std::ios::binary);
	boost::archive::binary_oarchive oa(ofs);
	oa << ogEditHistory;
}

void SceneEditsLibrary::loadOgHistory(std::string path)
{
	std::ifstream ifs(path + "OptimizedGroups.edit", std::ios::binary);

	if (ifs.good())
	{
		boost::archive::binary_iarchive ia(ifs);

		ia >> ogEditHistory;
	}
}

bool SceneEditsLibrary::loadSavedParticleChanges(ParticleEdit& edit, std::string particleName)
{
	auto& particle = particleEditHistory.data.begin();

	while (particle != particleEditHistory.data.end())
	{
		if (particle->first == particleName)
		{
			edit.merge(particle->second, false);
			return true;
		}

		particle++;
	}

	return false;
}

void SceneEditsLibrary::addParticleEdit(ParticleEdit& edit, std::string particleName)
{
	auto& ent = particleEditHistory.data[particleName];

	if (ent.originName.empty())
		ent.originName = edit.originName;

	ent.merge(edit, true);

	saveParticleHistory(Global::gameMgr->getCurrentLvlInfo()->path);
}

void SceneEditsLibrary::removeParticleEdit(std::string particleName)
{
	auto& ent = particleEditHistory.data.begin();

	while (ent != particleEditHistory.data.end())
	{
		if (ent->first == particleName)
		{
			particleEditHistory.data.erase(ent);
			break;
		}

		ent++;
	}

	saveParticleHistory(Global::gameMgr->getCurrentLvlInfo()->path);
}

bool SceneEditsLibrary::loadSavedDetailGeometryChanges(DetailGeometryEdit& edit, std::string name)
{
	auto& dg = dgEditHistory.data.begin();

	while (dg != dgEditHistory.data.end())
	{
		if (dg->first == name)
		{
			//dgs have unique mats
			if (edit.originName != dg->second.originName)
			{
				//old save, erase it
				dgEditHistory.data.erase(dg);
				return false;
			}
			else
				edit.merge(dg->second, false);

			return true;
		}

		dg++;
	}

	return false;
}

void SceneEditsLibrary::addDetailGeometryEdit(DetailGeometryEdit& edit, std::string name)
{
	auto& dg = dgEditHistory.data[name];

	if (dg.dgName.empty())
		dg.dgName = edit.dgName;

	dg.merge(edit, true);

	saveDgHistory(Global::gameMgr->getCurrentLvlInfo()->path);
}

void SceneEditsLibrary::removeDetailGeometryEdit(std::string name)
{
	auto& dg = dgEditHistory.data.begin();

	while (dg != dgEditHistory.data.end())
	{
		if (dg->first == name)
		{
			dgEditHistory.data.erase(dg);
			break;
		}

		dg++;
	}

	saveDgHistory(Global::gameMgr->getCurrentLvlInfo()->path);
}

bool SceneEditsLibrary::loadSavedOptimizedGroupChanges(OptimizedGroupEdit& edit, std::string name)
{
	auto& ent = ogEditHistory.data.begin();

	while (ent != ogEditHistory.data.end())
	{
		if (ent->first == name)
		{
			//must have at least copy name of saved origin
			if (!SUtils::startsWith(edit.originName, ent->second.originName))
			{
				//old save, erase it
				ogEditHistory.data.erase(ent);
				return false;
			}
			else
				edit.merge(ent->second, false);

			return true;
		}

		ent++;
	}

	return false;
}

void SceneEditsLibrary::addOptimizedGroupEdit(OptimizedGroupEdit& edit, std::string name)
{
	auto& og = ogEditHistory.data[name];

	if (og.originName.empty())
		og.originName = edit.originName;

	og.merge(edit, true);

	saveOgHistory(Global::gameMgr->getCurrentLvlInfo()->path);
}

void SceneEditsLibrary::removeOptimizedGroupEdit(std::string name)
{
	auto& og = ogEditHistory.data.begin();

	while (og != ogEditHistory.data.end())
	{
		if (og->first == name)
		{
			ogEditHistory.data.erase(og);
			break;
		}

		og++;
	}

	saveOgHistory(Global::gameMgr->getCurrentLvlInfo()->path);
}

LevelEdit* SceneEditsLibrary::getLevelEdit()
{
	return &levelEdits;
}
