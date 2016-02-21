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
    return true;
}

void SceneEditsLibrary::addDetailGeometryEdit(DetailGeometryEdit& edit, std::string name)
{

}

void SceneEditsLibrary::removeDetailGeometryEdit(std::string name)
{

}

LevelEdit* SceneEditsLibrary::getLevelEdit()
{
    return &levelEdits;
}
