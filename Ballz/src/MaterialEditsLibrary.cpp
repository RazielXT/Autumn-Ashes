#include "stdafx.h"
#include "MaterialEditsLibrary.h"
#include "GameStateManager.h"

#include <iostream>
#include "SUtils.h"

static void mergeParams(std::vector<MaterialEdit_::MaterialVariable>& from, std::vector<MaterialEdit_::MaterialVariable>& target, bool addNotExisting)
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
                MaterialEdit_::MaterialVariable mvar;
                mvar.edited = true;
                mvar.name = var.name;
                mvar.size = var.size;
                memcpy(mvar.buffer, var.buffer, 4 * var.size);

                target.push_back(mvar);
            }
        }

    }
}

void MaterialEdit_::mergeChanges(MaterialEdit_& r, bool addNotExisting)
{
    originMatName = r.originMatName;

    mergeParams(r.moreParams, moreParams, addNotExisting);

    mergeParams(r.psVariables, psVariables, addNotExisting);
}

void MaterialEdit_::setMaterialParam(Ogre::MaterialPtr ptr, MaterialEdit_::MaterialVariable& var)
{
    if (ptr.isNull())
        return;

    int pass = ptr->getTechnique(0)->getNumPasses() - 1;

    ptr->getTechnique(0)->getPass(pass)->getFragmentProgramParameters()->setNamedConstant(var.name, var.buffer, 1, var.size);
}

void MaterialEdit_::setParticleParam(Ogre::ParticleSystem* ps, MaterialEdit_::MaterialVariable& var)
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

void MaterialEdit_::generateParticleParams(Ogre::ParticleSystem* ps)
{
    moreParams.clear();
    MaterialEdit_::MaterialVariable var;

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

void MaterialEditsLibrary::addEdit(MaterialEdit_& edit, std::string entName)
{
    auto& ent = editHistory[entName];

    if (ent.originMatName.empty())
        ent.originMatName = edit.originMatName;

    ent.mergeChanges(edit, true);

    saveFile(Global::gameMgr->getCurrentLvlInfo()->path);
}

void MaterialEditsLibrary::removeEdit_(std::string entName)
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

void MaterialEditsLibrary::importToScene()
{
    auto& sceneEdit = Global::gameMgr->sceneEdits;

    for (auto& it : editParticleHistory)
    {
        /*
           ParticleEdit edit;

           auto& ent = it.first;
           auto& vars = it.second;

           edit.originName = vars.originMatName;

           for (auto& oVar : vars.psVariables)
           {
               EditVariable var;
               var.name = oVar.name;
               var.edited = oVar.edited;
               var.size = oVar.size;
               memcpy(var.buffer, oVar.buffer, 4 * oVar.size);

               edit.psVariables.push_back(var);
           }

           for (auto& oVar : vars.moreParams)
           {
               EditVariable var;
               var.name = oVar.name;
               var.edited = oVar.edited;
               var.size = oVar.size;
               memcpy(var.buffer, oVar.buffer, 4 * oVar.size);

               edit.particleParams.push_back(var);
           }
        */
        //sceneEdit.particleEditHistory.data[ent] = edit;
    }
}

bool MaterialEditsLibrary::loadSavedChanges(MaterialEdit_& edit, std::string entName)
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
            if (!particleSiblings.getParent(ps.first).empty())
                continue;

            auto p = Global::mSceneMgr->getParticleSystem(ps.first);
            auto curMatName = p->getMaterialName();

            std::vector<std::string> particles = { ps.first };
            if (particleSiblings.isParent(ps.first))
            {
                auto& ch = particleSiblings.children[ps.first];
                particles.insert(particles.end(), ch.begin(), ch.end());
            }

            if (ps.second.originMatName == curMatName)
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

                    for (auto& var : ps.second.moreParams)
                    {
                        ps.second.setParticleParam(p, var);
                    }
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

bool MaterialEditsLibrary::loadSavedParticleChanges(MaterialEdit_& edit, std::string particleName)
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

void MaterialEditsLibrary::addParticleEdit(MaterialEdit_& edit, std::string particleName)
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

        ent++;
    }

    saveFile(Global::gameMgr->getCurrentLvlInfo()->path);
}

std::string ParticleSiblings::getParent(std::string name)
{
    for (auto ch : children)
    {
        if (std::find(ch.second.begin(), ch.second.end(), name) != ch.second.end())
            return ch.first;
    }

    return "";
}

void ParticleSiblings::connectSiblings(std::string parent, std::string child)
{
    children[parent].push_back(child);
}

bool ParticleSiblings::isParent(std::string name)
{
    return children.find(name) != children.end();
}

std::vector<Ogre::ParticleSystem*> ParticleSiblings::getChildren(std::string parent)
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
