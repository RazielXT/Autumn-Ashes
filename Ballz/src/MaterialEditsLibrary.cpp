#include "stdafx.h"
#include "MaterialEditsLibrary.h"
#include "GameStateManager.h"

#include <iostream>
#include "SUtils.h"

static void mergeParams(std::vector<MaterialEdit::MaterialVariable>& from, std::vector<MaterialEdit::MaterialVariable>& target, bool addNotExisting)
{
    for (auto& var : from)
    {
        if (var.edited)
        {
            bool found = false;

            for (auto& mvar : target)
            {
                if (mvar.name == var.name && mvar.size == var.size)
                {
                    memcpy(mvar.buffer, var.buffer, 4 * var.size);
                    found = true;
                }
            }

            if (!found && addNotExisting)
            {
                MaterialEdit::MaterialVariable mvar;
                mvar.edited = true;
                mvar.name = var.name;
                mvar.size = var.size;
                memcpy(mvar.buffer, var.buffer, 4 * var.size);

                target.push_back(mvar);
            }
        }

    }
}

void MaterialEdit::mergeChanges(MaterialEdit& r, bool addNotExisting)
{
    originMatName = r.originMatName;

    mergeParams(r.moreParams, moreParams, addNotExisting);

    mergeParams(r.psVariables, psVariables, addNotExisting);
}

void MaterialEdit::setMaterialParam(Ogre::MaterialPtr ptr, MaterialEdit::MaterialVariable& var)
{
    if (ptr.isNull())
        return;

    int pass = ptr->getTechnique(0)->getNumPasses() - 1;

    ptr->getTechnique(0)->getPass(pass)->getFragmentProgramParameters()->setNamedConstant(var.name, var.buffer, 1, var.size);
}

void MaterialEdit::setParticleParam(Ogre::ParticleSystem* ps, MaterialEdit::MaterialVariable& var)
{
    if (var.name == "Angle")
        ps->getEmitter(0)->setAngle(Ogre::Degree(var.buffer[0]));
    if (var.name == "MinMaxVelocity")
        ps->getEmitter(0)->setParticleVelocity(var.buffer[0], var.buffer[1]);
    if (var.name == "Emmision")
        ps->getEmitter(0)->setEmissionRate(var.buffer[0]);
    if (var.name == "MinMaxTime")
        ps->getEmitter(0)->setTimeToLive(var.buffer[0], var.buffer[1]);
    if (var.name == "XYSize")
        ps->setDefaultDimensions(var.buffer[0], var.buffer[1]);
}

void MaterialEdit::generateParticleParams(Ogre::ParticleSystem* ps)
{
    moreParams.clear();
    MaterialEdit::MaterialVariable var;

    var.name = "Angle";
    var.size = 1;
    var.buffer[0] = ps->getEmitter(0)->getAngle().valueDegrees();
    moreParams.push_back(var);

    var.name = "MinMaxVelocity";
    var.size = 2;
    var.buffer[0] = ps->getEmitter(0)->getMinParticleVelocity();
    var.buffer[1] = ps->getEmitter(0)->getMaxParticleVelocity();
    moreParams.push_back(var);

    var.name = "Emmision";
    var.size = 1;
    var.buffer[0] = ps->getEmitter(0)->getEmissionRate();
    moreParams.push_back(var);

    var.name = "MinMaxTime";
    var.size = 2;
    var.buffer[0] = ps->getEmitter(0)->getMinTimeToLive();
    var.buffer[1] = ps->getEmitter(0)->getMaxTimeToLive();
    moreParams.push_back(var);

    var.name = "XYSize";
    var.size = 2;
    var.buffer[0] = ps->getDefaultWidth();
    var.buffer[1] = ps->getDefaultHeight();
    moreParams.push_back(var);
}

void MaterialEditsLibrary::addEdit(MaterialEdit& edit, std::string entName)
{
    auto& ent = editHistory[entName];

    if (ent.originMatName.empty())
        ent.originMatName = edit.originMatName;

    ent.mergeChanges(edit, true);

    saveFile(Global::gameMgr->getCurrentLvlInfo()->path);
}

void MaterialEditsLibrary::removeEdit(std::string entName)
{
    auto& ent = editHistory.begin();

    while (ent != editHistory.end())
    {
        if (ent->first == entName)
        {
            editHistory.erase(ent);
            break;
        }
    }

    saveFile(Global::gameMgr->getCurrentLvlInfo()->path);
}

bool MaterialEditsLibrary::loadSavedChanges(MaterialEdit& edit, std::string entName)
{
    auto& ent = editHistory.begin();

    while (ent != editHistory.end())
    {
        if (ent->first == entName)
        {
            //must have at least copy name of saved origin
            if (!SUtils::startsWith(edit.originMatName, ent->second.originMatName))
            {
                //old save, erase it
                editHistory.erase(ent);
                return false;
            }
            else
                edit.mergeChanges(ent->second, false);

            return true;
        }

        ent++;
    }

    return false;
}

MaterialEditsLibrary::MaterialEditsLibrary()
{
}

void MaterialEditsLibrary::applyChanges()
{
    loadFile(Global::gameMgr->getCurrentLvlInfo()->path);

    for (auto& ent : editHistory)
    {
        if (Global::mSceneMgr->hasEntity(ent.first))
        {
            auto e = Global::mSceneMgr->getEntity(ent.first);
            auto curMat = e->getSubEntity(0)->getMaterial();

            if (ent.second.originMatName == curMat->getName())
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

    for (auto& ps : editParticleHistory)
    {
        if (Global::mSceneMgr->hasParticleSystem(ps.first))
        {
            auto p = Global::mSceneMgr->getParticleSystem(ps.first);
            auto curMatName = p->getMaterialName();

            if (ps.second.originMatName == curMatName)
            {
                Ogre::MaterialPtr newMat = Ogre::MaterialManager::getSingleton().getByName(curMatName);
                newMat = newMat->clone(curMatName + std::to_string(idCounter++));

                p->setMaterialName(newMat->getName());

                for (auto& var : ps.second.psVariables)
                {
                    int pass = newMat->getTechnique(0)->getNumPasses() - 1;
                    newMat->getTechnique(0)->getPass(pass)->getFragmentProgramParameters()->setNamedConstant(var.name, var.buffer, 1, var.size);
                }

                for (auto& var : ps.second.moreParams)
                {
                    ps.second.setParticleParam(p, var);
                }
            }
        }
    }
}

void MaterialEditsLibrary::saveFile(std::string path)
{
    std::ofstream ofs(path + "materialEdits", std::ios::binary);
    boost::archive::binary_oarchive oa(ofs);
    oa << this;
}

void MaterialEditsLibrary::loadFile(std::string path)
{
    std::ifstream ifs(path + "materialEdits", std::ios::binary);

    if (ifs.good())
    {
        boost::archive::binary_iarchive ia(ifs);

        MaterialEditsLibrary* arch;
        ia >> arch;

        editHistory = std::move(arch->editHistory);
        editParticleHistory = std::move(arch->editParticleHistory);
        delete arch;
    }
}

bool MaterialEditsLibrary::loadSavedParticleChanges(MaterialEdit& edit, std::string particleName)
{
    auto& particle = editParticleHistory.begin();

    while (particle != editParticleHistory.end())
    {
        if (particle->first == particleName)
        {
            edit.mergeChanges(particle->second, false);
            return true;
        }

        particle++;
    }

    return false;
}

void MaterialEditsLibrary::addParticleEdit(MaterialEdit& edit, std::string particleName)
{
    auto& ent = editParticleHistory[particleName];

    if (ent.originMatName.empty())
        ent.originMatName = edit.originMatName;

    ent.mergeChanges(edit, true);

    saveFile(Global::gameMgr->getCurrentLvlInfo()->path);
}

void MaterialEditsLibrary::removeParticleEdit(std::string particleName)
{
    auto& ent = editParticleHistory.begin();

    while (ent != editParticleHistory.end())
    {
        if (ent->first == particleName)
        {
            editParticleHistory.erase(ent);
            break;
        }
    }

    saveFile(Global::gameMgr->getCurrentLvlInfo()->path);
}
