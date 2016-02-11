#include "stdafx.h"
#include "SceneEditsLibrary.h"
#include "GameStateManager.h"

#include <iostream>
#include "SUtils.h"


void SceneEditsLibrary::addEdit(MaterialEdit& edit, std::string entName)
{
	auto& ent = editMaterialHistory[entName];

	if (ent.originName.empty())
		ent.originName = edit.originName;

	ent.merge(edit, true);

	saveMaterialHistory(Global::gameMgr->getCurrentLvlInfo()->path);
}

void SceneEditsLibrary::removeMaterialEdit(std::string entName)
{
	auto& ent = editMaterialHistory.begin();

	while (ent != editMaterialHistory.end())
	{
		if (ent->first == entName)
		{
			editMaterialHistory.erase(ent);
			break;
		}
	}

	saveMaterialHistory(Global::gameMgr->getCurrentLvlInfo()->path);
}

bool SceneEditsLibrary::loadSavedChanges(MaterialEdit& edit, std::string entName)
{
	auto& ent = editMaterialHistory.begin();

	while (ent != editMaterialHistory.end())
	{
		if (ent->first == entName)
		{
			//must have at least copy name of saved origin
			if (!SUtils::startsWith(edit.originName, ent->second.originName))
			{
				//old save, erase it
				editMaterialHistory.erase(ent);
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

void SceneEditsLibrary::applyChanges()
{
	loadMaterialHistory(Global::gameMgr->getCurrentLvlInfo()->path);

	for (auto& ent : editMaterialHistory)
	{
		if (Global::mSceneMgr->hasEntity(ent.first))
		{
			auto e = Global::mSceneMgr->getEntity(ent.first);
			auto curMat = e->getSubEntity(0)->getMaterial();

			if (ent.second.originName == curMat->getName())
			{
				auto newMat = curMat->clone(curMat->getName() + std::to_string(idCounter++));
				e->setMaterial(newMat);

				for (auto& var : ent.second.psVariables)
				{
					int pass = newMat->getTechnique(0)->getNumPasses() - 1;
					newMat->getTechnique(0)->getPass(pass)->getFragmentProgramParameters()->setNamedConstant(var.name, var.buffer, 1, var.size);
				}
			}
		}
	}

	loadParticleHistory(Global::gameMgr->getCurrentLvlInfo()->path);

	for (auto& ps : editParticleHistory)
	{
		if (Global::mSceneMgr->hasParticleSystem(ps.first))
		{
			if (!particleChildren.getParent(ps.first).empty())
				continue;

			auto p = Global::mSceneMgr->getParticleSystem(ps.first);
			auto curMatName = p->getMaterialName();

			std::vector<std::string> particles = { ps.first };
			if (particleChildren.isParent(ps.first))
			{
				auto& ch = particleChildren.children[ps.first];
				particles.insert(particles.end(), ch.begin(), ch.end());
			}

			if (ps.second.originName == curMatName)
			{
				Ogre::MaterialPtr newMat = Ogre::MaterialManager::getSingleton().getByName(curMatName);
				newMat = newMat->clone(curMatName + std::to_string(idCounter++));

				for (auto& var : ps.second.psVariables)
				{
					int pass = newMat->getTechnique(0)->getNumPasses() - 1;
					newMat->getTechnique(0)->getPass(pass)->getFragmentProgramParameters()->setNamedConstant(var.name, var.buffer, 1, var.size);
				}

				for (auto& particle : particles)
				{
					if (!Global::mSceneMgr->hasParticleSystem(particle))
						continue;

					p = Global::mSceneMgr->getParticleSystem(particle);
					p->setMaterialName(newMat->getName());

					for (auto& var : ps.second.particleParams)
					{
						ps.second.setParticleParam(p, var);
					}
				}
			}
		}
	}
}

void SceneEditsLibrary::saveMaterialHistory(std::string path)
{
	std::ofstream ofs(path + "Materials.edit", std::ios::binary);
	boost::archive::binary_oarchive oa(ofs);
	oa << editMaterialHistory;
}

void SceneEditsLibrary::loadMaterialHistory(std::string path)
{
	std::ifstream ifs(path + "Materials.edit", std::ios::binary);

	if (ifs.good())
	{
		boost::archive::binary_iarchive ia(ifs);

		EditedEntities* temp;
		ia >> temp;

		editMaterialHistory = std::move(*temp);

		delete temp;
	}
}

void SceneEditsLibrary::saveParticleHistory(std::string path)
{
	std::ofstream ofs(path + "Particles.edit", std::ios::binary);
	boost::archive::binary_oarchive oa(ofs);
	oa << editMaterialHistory;
}

void SceneEditsLibrary::loadParticleHistory(std::string path)
{
	std::ifstream ifs(path + "Particles.edit", std::ios::binary);

	if (ifs.good())
	{
		boost::archive::binary_iarchive ia(ifs);

		EditedParticles* temp;
		ia >> temp;

		editParticleHistory = std::move(*temp);

		delete temp;
	}
}

bool SceneEditsLibrary::loadSavedParticleChanges(ParticleEdit& edit, std::string particleName)
{
	auto& particle = editParticleHistory.begin();

	while (particle != editParticleHistory.end())
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
	auto& ent = editParticleHistory[particleName];

	if (ent.originName.empty())
		ent.originName = edit.originName;

	ent.merge(edit, true);

	saveParticleHistory(Global::gameMgr->getCurrentLvlInfo()->path);
}

void SceneEditsLibrary::removeParticleEdit(std::string particleName)
{
	auto& ent = editParticleHistory.begin();

	while (ent != editParticleHistory.end())
	{
		if (ent->first == particleName)
		{
			editParticleHistory.erase(ent);
			break;
		}

		ent++;
	}

	saveParticleHistory(Global::gameMgr->getCurrentLvlInfo()->path);
}

std::string ParticleChildren::getParent(std::string name)
{
	for (auto ch : children)
	{
		if (std::find(ch.second.begin(), ch.second.end(), name) != ch.second.end())
			return ch.first;
	}

	return "";
}

void ParticleChildren::connectChild(std::string parent, std::string child)
{
	children[parent].push_back(child);
}

bool ParticleChildren::isParent(std::string name)
{
	return children.find(name) != children.end();
}

std::vector<Ogre::ParticleSystem*> ParticleChildren::getChildren(std::string parent)
{
	std::vector<Ogre::ParticleSystem*> out;

	if (isParent(parent))
		for (auto ch : children[parent])
		{
			if (Global::mSceneMgr->hasParticleSystem(ch))
			{
				out.push_back(Global::mSceneMgr->getParticleSystem(ch));
			}
		}

	return out;
}
