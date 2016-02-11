#include "ParticleEdit.h"
#include "GameStateManager.h"

bool ParticleEdit::query()
{
	ps = Global::gameMgr->particleMgr.getClosestParticle();

	if (!ps || ps->getNumEmitters() == 0)
		return false;

	ptr = Ogre::MaterialManager::getSingleton().getByName(ps->getMaterialName());

	loadMaterial();

	changed = Global::gameMgr->sceneEdits.loadSavedParticleChanges(*this, ps->getName());

	generateParticleParams();

	return true;
}

EditBaseRows ParticleEdit::getBaseRows()
{
	return{ { originName,EditRow::Static },{ "Save",EditRow::Save },{ "Particle",EditRow::Params },{ "PS",EditRow::Params } };
}

EditVariables* ParticleEdit::getParams(int row)
{
	if (row == 2)
	{
		return &particleParams;
	}
	else
		return MaterialEdit::getParams(row);
}

void ParticleEdit::editChanged(EditVariable& var, int id)
{
	if (id == 2)
	{
		setParticleParam(ps, var);

		auto children = Global::gameMgr->sceneEdits.particleChildren.getChildren(ps->getName());
		for (auto child : children)
		{
			setParticleParam(child, var);
		}
	}
	else
		return MaterialEdit::editChanged(var, id);
}

void ParticleEdit::save()
{
	if (changed)
		Global::gameMgr->sceneEdits.addParticleEdit(*this, ps->getName());
	else
		Global::gameMgr->sceneEdits.removeParticleEdit(ps->getName());
}

void ParticleEdit::merge(ParticleEdit& r, bool addNotExisting)
{
	mergeParams(r.particleParams, particleParams, addNotExisting);

	MaterialEdit::merge(r, addNotExisting);
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
}

void ParticleEdit::materialChanged()
{
	if (!changed)
	{
		MaterialEdit::materialChanged();

		ps->setMaterialName(ptr->getName());

		auto children = Global::gameMgr->sceneEdits.particleChildren.getChildren(ps->getName());
		for (auto child : children)
		{
			child->setMaterialName(ptr->getName());
		}
	}
}
