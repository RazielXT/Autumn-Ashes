#include "stdafx.h"
#include "MaterialEdit.h"
#include "GameStateManager.h"
#include "GUtils.h"
#include "Player.h"
#include "SUtils.h"

MaterialEdit* MaterialEdit::query()
{
	GUtils::RayInfo out;
	if (GUtils::getRayInfo(Global::camera->position, Global::camera->direction, 1000, out))
	{
		auto node = static_cast<Ogre::SceneNode*>(out.body->getOgreNode());

		if (node && node->numAttachedObjects() > 0)
		{
			auto ent = static_cast<Ogre::Entity*>(node->getAttachedObject(0));

			if (editableEntity(ent))
			{
				return new MaterialEdit(ent);
			}
		}
	}

	return nullptr;
}

bool MaterialEdit::editableEntity(Ogre::Entity* ent)
{
	auto mat = ent->getSubEntity(0)->getMaterial();
	return mat->getTechnique(0)->getNumPasses() > 0;
}

MaterialEdit::MaterialEdit(Ogre::Entity* ent)
{
	entity = ent;
	materialPtr = ent->getSubEntity(0)->getMaterial();

	loadMaterial();
	changedMaterial = Global::gameMgr->sceneEdits.loadMaterialChanges(*this, entity->getName());

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
	var.edited = true;

	if (materialPtr.isNull())
		return;

	materialChanged();

	if (row == "PS")
	{
		int pass = materialPtr->getTechnique(0)->getNumPasses() - 1;
		materialPtr->getTechnique(0)->getPass(pass)->getFragmentProgramParameters()->setNamedConstant(var.name, var.buffer, 1, var.size);
	}
	if (row == "VS")
	{
		int pass = materialPtr->getTechnique(0)->getNumPasses() - 1;
		materialPtr->getTechnique(0)->getPass(pass)->getVertexProgramParameters()->setNamedConstant(var.name, var.buffer, 1, var.size);
	}
}

void MaterialEdit::customAction(std::string name)
{
	if (name == "Save")
	{
		if (changedMaterial)
			Global::gameMgr->sceneEdits.saveMaterialEdit(entity->getName());
	}
}

void MaterialEdit::merge(MaterialEdit& r, bool addNotExisting)
{
	originName = r.originName;

	mergeParams(r.psVariables, psVariables, addNotExisting);
	mergeParams(r.vsVariables, vsVariables, addNotExisting);
}

void MaterialEdit::applyAllChanges(const std::map < std::string, MaterialEdit >& changes)
{
	for (auto& ent : changes)
	{
		if (Global::sceneMgr->hasEntity(ent.first))
		{
			auto e = Global::sceneMgr->getEntity(ent.first);
			auto curMat = e->getSubEntity(0)->getMaterial();
			auto baseMatName = ent.second.originName.substr(0, ent.second.originName.find_first_of('_'));

			auto curName = curMat->getName();
			if (curName.find("_CM"))
				curName = curName.substr(0, curName.find("_CM"));

			if (SUtils::startsWith(curName, baseMatName) || SUtils::startsWith(baseMatName, curName))
				//if (ent.second.originName == curMat->getName())
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
	int pass = mat->getTechnique(0)->getNumPasses() - 1;
	auto& vsParams = mat->getTechnique(0)->getPass(pass)->getVertexProgramParameters()->getConstantDefinitions().map;

	for (auto& var : changes.vsVariables)
	{
		if(vsParams.find(var.name) != vsParams.end())
			mat->getTechnique(0)->getPass(pass)->getVertexProgramParameters()->setNamedConstant(var.name, var.buffer, 1, var.size);
	}

	auto& psParams = mat->getTechnique(0)->getPass(pass)->getFragmentProgramParameters()->getConstantDefinitions().map;

	for (auto& var : changes.psVariables)
	{
		if (psParams.find(var.name) != psParams.end())
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
			entity->getSubEntity(0)->setMaterial(materialPtr);
	}

	Global::gameMgr->sceneEdits.addMaterialEdit(*this, entity->getName());
}

void MaterialEdit::resetMaterial()
{
	changedMaterial = false;
	matInstance = false;
	materialPtr = Ogre::MaterialManager::getSingleton().getByName(originName);

	if (entity)
		entity->getSubEntity(0)->setMaterial(materialPtr);
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
	auto& al = gpuParams->getAutoConstantList();

	bool skip = true;

	for (auto c : l.map)
	{
		skip = !skip;
		if (skip) continue;

		if (c.second.constType <= 4)
		{
			bool autoConstant = false;

			for (auto& ac : al)
			{
				if(ac.physicalIndex == c.second.physicalIndex)
					autoConstant = true;
			}

			if (!autoConstant)
			{
				EditVariable var;
				var.name = c.first;
				var.size = c.second.constType;
				memcpy(var.buffer, gpuParams->getFloatPointer(c.second.physicalIndex), 4 * var.size);

				vars.push_back(var);
			}
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