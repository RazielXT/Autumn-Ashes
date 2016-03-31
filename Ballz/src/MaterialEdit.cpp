#include "stdafx.h"
#include "MaterialEdit.h"
#include "GameStateManager.h"
#include "GUtils.h"
#include "Player.h"
#include "SUtils.h"

MaterialEdit* MaterialEdit::query()
{
	GUtils::RayInfo out;
	if (GUtils::getRayInfo(Global::camera->getPosition(), Global::camera->getFacingDirection(), 1000, out))
	{
		auto node = static_cast<Ogre::SceneNode*>(out.body->getOgreNode());

		if (node && node->numAttachedObjects() > 0)
		{
			auto ent = static_cast<Ogre::Entity*>(node->getAttachedObject(0));
			auto mat = ent->getSubEntity(0)->getMaterial();

			if (mat->getTechnique(0)->getNumPasses() > 0)
			{
				auto edit = new MaterialEdit(ent);

				return edit;
			}
		}
	}

	return nullptr;
}

MaterialEdit::MaterialEdit(Ogre::Entity* ent)
{
	entity = ent;
	materialPtr = ent->getSubEntity(0)->getMaterial();

	loadMaterial();
	changedMaterial = Global::gameMgr->sceneEdits.loadSavedMaterialChanges(*this, entity->getName());

	rows = { { ent->getName(),EditRow::Caption } , { originName,EditRow::Caption },{ "Save",EditRow::Action },{ "VS",EditRow::Params },{ "PS",EditRow::Params } };
}

EditVariables* MaterialEdit::getParams(const std::string& row)
{
	if (row == "PS")
	{
		return &psVariables;
	}
	if (row == "VS")
	{
		return &vsVariables;
	}

	return nullptr;
}

void MaterialEdit::editChanged(EditVariable& var, const std::string& row)
{
	if (row == "PS")
	{
		var.edited = true;

		if (materialPtr.isNull())
			return;

		materialChanged();

		int pass = materialPtr->getTechnique(0)->getNumPasses() - 1;
		materialPtr->getTechnique(0)->getPass(pass)->getFragmentProgramParameters()->setNamedConstant(var.name, var.buffer, 1, var.size);
	}
	if (row == "VS")
	{
		var.edited = true;

		if (materialPtr.isNull())
			return;

		materialChanged();

		int pass = materialPtr->getTechnique(0)->getNumPasses() - 1;
		materialPtr->getTechnique(0)->getPass(pass)->getVertexProgramParameters()->setNamedConstant(var.name, var.buffer, 1, var.size);
	}
}

void MaterialEdit::customAction(std::string name)
{
	if (name == "Save")
	{
		if (changedMaterial)
			Global::gameMgr->sceneEdits.addMaterialEdit(*this, entity->getName());
		else
			Global::gameMgr->sceneEdits.removeMaterialEdit(entity->getName());
	}
}

void MaterialEdit::merge(MaterialEdit& r, bool addNotExisting)
{
	originName = r.originName;

	mergeParams(r.psVariables, psVariables, addNotExisting);
	mergeParams(r.vsVariables, vsVariables, addNotExisting);
}

void MaterialEdit::applyChanges(const std::map < std::string, MaterialEdit >& changes)
{
	for (auto& ent : changes)
	{
		if (Global::mSceneMgr->hasEntity(ent.first))
		{
			auto e = Global::mSceneMgr->getEntity(ent.first);
			auto curMat = e->getSubEntity(0)->getMaterial();

			//if (SUtils::startsWith(curMat->getName(), ent.second.originName))
			if (ent.second.originName == curMat->getName())
			{
				auto newMat = curMat->clone(curMat->getName() + std::to_string(idCounter++));
				e->setMaterial(newMat);

				applyMaterialChanges(newMat, ent.second);
			}
		}
	}
}

void MaterialEdit::applyMaterialChanges(Ogre::MaterialPtr mat, const MaterialEdit& changes)
{
	for (auto& var : changes.vsVariables)
	{
		int pass = mat->getTechnique(0)->getNumPasses() - 1;
		mat->getTechnique(0)->getPass(pass)->getVertexProgramParameters()->setNamedConstant(var.name, var.buffer, 1, var.size);
	}
	for (auto& var : changes.psVariables)
	{
		int pass = mat->getTechnique(0)->getNumPasses() - 1;
		mat->getTechnique(0)->getPass(pass)->getFragmentProgramParameters()->setNamedConstant(var.name, var.buffer, 1, var.size);
	}
}

void MaterialEdit::materialChanged()
{
	if (!changedMaterial)
	{
		changedMaterial = true;

		materialPtr = materialPtr->clone(materialPtr->getName() + std::to_string(idCounter++));

		if (entity)
			entity->setMaterial(materialPtr);
	}
}

void MaterialEdit::resetMaterial()
{
	changedMaterial = false;
	matInstance = false;
	materialPtr = Ogre::MaterialManager::getSingleton().getByName(originName);

	if (entity)
		entity->setMaterialName(originName);
}

std::vector<EditVariable> MaterialEdit::generateVsParams(Ogre::MaterialPtr matPtr)
{
	int pass = matPtr->getTechnique(0)->getNumPasses() - 1;
	auto params = matPtr->getTechnique(0)->getPass(pass)->getVertexProgramParameters();

	return generateShaderEditParams(params);
}

std::vector<EditVariable> MaterialEdit::generatePsParams(Ogre::MaterialPtr matPtr)
{
	int pass = matPtr->getTechnique(0)->getNumPasses() - 1;
	auto params = matPtr->getTechnique(0)->getPass(pass)->getFragmentProgramParameters();

	return generateShaderEditParams(params);
}

std::vector<EditVariable> MaterialEdit::generateShaderEditParams(Ogre::GpuProgramParametersSharedPtr gpuParams)
{
	std::vector<EditVariable> vars;
	auto& l = gpuParams->getConstantDefinitions();
	bool skip = true;

	for (auto c : l.map)
	{
		skip = !skip;
		if (skip) continue;

		if (c.second.constType <= 4)
		{
			EditVariable var;
			var.name = c.first;
			var.size = c.second.constType;
			memcpy(var.buffer, gpuParams->getFloatPointer(c.second.physicalIndex), 4 * var.size);

			vars.push_back(var);
		}
	}

	return vars;
}

void MaterialEdit::loadMaterial()
{
	originName = materialPtr->getName();
	psVariables = generatePsParams(materialPtr);
	vsVariables = generateVsParams(materialPtr);
}