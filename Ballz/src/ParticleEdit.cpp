#include "stdafx.h"
#include "ParticleEdit.h"
#include "GameStateManager.h"

ParticleChildren ParticleEdit::particleChildren;

ParticleEdit* ParticleEdit::query()
{
    auto ps = Global::gameMgr->particleMgr.getClosestParticle();

    if (!ps || ps->getNumEmitters() == 0)
        return nullptr;

    ParticleEdit* edit = new ParticleEdit(ps);

    return edit;
}

ParticleEdit::ParticleEdit(Ogre::ParticleSystem* particle)
{
    ps = particle;
    entity = nullptr;

    materialPtr = Ogre::MaterialManager::getSingleton().getByName(ps->getMaterialName());

    loadMaterial();

    changed = Global::gameMgr->sceneEdits.loadSavedParticleChanges(*this, ps->getName());

    generateParticleParams();

    rows = { { ps->getName(),EditRow::Caption },{ originName,EditRow::Caption },{ "Save",EditRow::Action },{ "Restart",EditRow::Action },{ "Particle",EditRow::Params },{ "PS",EditRow::Params } };
}

EditVariables* ParticleEdit::getParams(const std::string& row)
{
    if (row == "Particle")
    {
        return &particleParams;
    }
    else
        return MaterialEdit::getParams(row);
}

void ParticleEdit::editChanged(EditVariable& var, const std::string& row)
{
    if (row == "Particle")
    {
        var.edited = true;

        setParticleParam(ps, var);

        auto children = particleChildren.getChildren(ps->getName());
        for (auto child : children)
        {
            setParticleParam(child, var);
        }

        changed = true;
    }
    else
        MaterialEdit::editChanged(var, row);
}

void ParticleEdit::customAction(std::string name)
{
    if (name == "Save")
    {
        if (changed)
            Global::gameMgr->sceneEdits.addParticleEdit(*this, ps->getName());
        else
            Global::gameMgr->sceneEdits.removeParticleEdit(ps->getName());
    }
    if (name == "Restart")
    {
        ps->clear();

        auto children = particleChildren.getChildren(ps->getName());
        for (auto child : children)
        {
            child->clear();
        }
    }
}

void ParticleEdit::merge(ParticleEdit& r, bool addNotExisting)
{
    mergeParams(r.particleParams, particleParams, addNotExisting);

    MaterialEdit::merge(r, addNotExisting);
}

void ParticleEdit::applyChanges(std::map < std::string, ParticleEdit >& changes)
{
    for (auto& ps : changes)
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

void ParticleEdit::setParticleParam(Ogre::ParticleSystem* ps, EditVariable& var)
{
    if (var.name == "Angle")
        ps->getEmitter(0)->setAngle(Ogre::Degree(var.buffer[0]));
    if (var.name == "MinMaxVelocity")
        ps->getEmitter(0)->setParticleVelocity(var.buffer[0], var.buffer[1]);
    if (var.name == "Emission")
        ps->getEmitter(0)->setEmissionRate(var.buffer[0]);
    if (var.name == "MinMaxTime")
        ps->getEmitter(0)->setTimeToLive(var.buffer[0], var.buffer[1]);
    if (var.name == "XYSize")
        ps->setDefaultDimensions(var.buffer[0], var.buffer[1]);
    if (var.name == "Speed")
        ps->setSpeedFactor(var.buffer[0]);
}

void ParticleEdit::generateParticleParams()
{
    particleParams.clear();
    EditVariable var;
    auto emitter = ps->getEmitter(0);

    var.name = "Angle";
    var.size = 1;
    var.buffer[0] = emitter->getAngle().valueDegrees();
    particleParams.push_back(var);

    var.name = "MinMaxVelocity";
    var.size = 2;
    var.buffer[0] = emitter->getMinParticleVelocity();
    var.buffer[1] = emitter->getMaxParticleVelocity();
    particleParams.push_back(var);

    var.name = "Emission";
    var.size = 1;
    var.buffer[0] = emitter->getEmissionRate();
    particleParams.push_back(var);

    var.name = "MinMaxTime";
    var.size = 2;
    var.buffer[0] = emitter->getMinTimeToLive();
    var.buffer[1] = emitter->getMaxTimeToLive();
    particleParams.push_back(var);

    var.name = "XYSize";
    var.size = 2;
    var.buffer[0] = ps->getDefaultWidth();
    var.buffer[1] = ps->getDefaultHeight();
    particleParams.push_back(var);

    var.name = "Speed";
    var.size = 1;
    var.buffer[0] = ps->getSpeedFactor();
    particleParams.push_back(var);
}

void ParticleEdit::resetMaterial()
{
    MaterialEdit::resetMaterial();

    ps->setMaterialName(materialPtr->getName());

    auto children = particleChildren.getChildren(ps->getName());
    for (auto child : children)
    {
        child->setMaterialName(materialPtr->getName());
    }
}

void ParticleEdit::materialChanged()
{
    if (!changed)
    {
        MaterialEdit::materialChanged();

        ps->setMaterialName(materialPtr->getName());

        auto children = particleChildren.getChildren(ps->getName());
        for (auto child : children)
        {
            child->setMaterialName(materialPtr->getName());
        }
    }
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
