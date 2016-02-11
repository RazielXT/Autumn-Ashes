#include "MaterialEdit.h"
#include "GameStateManager.h"
#include "GUtils.h"
#include "Player.h"

int Edit::idCounter = 5000;

void Edit::mergeParams(std::vector<EditVariable>& from, std::vector<EditVariable>& target, bool addNotExisting)
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
				EditVariable mvar;
				mvar.edited = true;
				mvar.name = var.name;
				mvar.size = var.size;
				memcpy(mvar.buffer, var.buffer, 4 * var.size);

				target.push_back(mvar);
			}
		}
	}
}

bool MaterialEdit::query()
{
	reset();

	GUtils::RayInfo out;
	if (GUtils::getRayInfo(Global::player->getCameraPosition(), Global::player->getFacingDirection(), 1000, out))
	{
		auto node = static_cast<Ogre::SceneNode*>(out.body->getOgreNode());

		if (node && node->numAttachedObjects() > 0)
		{
			auto ent = static_cast<Ogre::Entity*>(node->getAttachedObject(0));
			auto mat = ent->getSubEntity(0)->getMaterial();

			if (mat->getTechnique(0)->getNumPasses() > 0)
			{
				entity = ent;
				ptr = mat;

				loadMaterial();
				changed = Global::gameMgr->sceneEdits.loadSavedChanges(*this, entity->getName());

				return psVariables.size() > 0;
			}
		}
	}

	return false;
}

EditBaseRows MaterialEdit::getBaseRows()
{
	return{ {originName,EditRow::Static},{ "Save",EditRow::Save },{ "VS",EditRow::Static },{ "PS",EditRow::Params } };
}

EditVariables* MaterialEdit::getParams(int row)
{
	if (row == 3)
	{
		return &psVariables;
	}

	return nullptr;
}

void MaterialEdit::editChanged(EditVariable& var, int id)
{
	if (id == 3)
	{
		if (ptr.isNull())
			return;

		materialChanged();

		int pass = ptr->getTechnique(0)->getNumPasses() - 1;

		ptr->getTechnique(0)->getPass(pass)->getFragmentProgramParameters()->setNamedConstant(var.name, var.buffer, 1, var.size);
	}
}

void MaterialEdit::save()
{
	if (changed)
		Global::gameMgr->sceneEdits.addEdit(*this, entity->getName());
	else
		Global::gameMgr->sceneEdits.removeMaterialEdit(entity->getName());
}

void MaterialEdit::merge(MaterialEdit& r, bool addNotExisting)
{
	originName = r.originName;

	mergeParams(r.psVariables, psVariables, addNotExisting);
}

void MaterialEdit::reset()
{
	ptr.setNull();
	entity = nullptr;
	changed = false;
	matInstance = false;

	psVariables.clear();
	vsVariables.clear();
}

void MaterialEdit::materialChanged()
{
	if (!changed)
	{
		changed = true;

		auto newMat = ptr->clone(ptr->getName() + std::to_string(idCounter++));

		if (entity)
			entity->setMaterial(newMat);

		ptr = newMat;
	}
}

void MaterialEdit::resetMaterial()
{
	entity->setMaterialName(originName);
	changed = false;
	matInstance = false;
	ptr = entity->getSubEntity(0)->getMaterial();
}

void MaterialEdit::loadMaterial()
{
	psVariables.clear();
	originName = ptr->getName();
	int pass = ptr->getTechnique(0)->getNumPasses() - 1;
	auto params = ptr->getTechnique(0)->getPass(pass)->getFragmentProgramParameters();
	auto& l = params->getConstantDefinitions();
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
			memcpy(var.buffer, params->getFloatPointer(c.second.physicalIndex), 4 * var.size);

			psVariables.push_back(var);
		}
	}
}