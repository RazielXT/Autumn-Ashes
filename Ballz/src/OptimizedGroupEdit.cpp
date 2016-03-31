#include "stdafx.h"
#include "OptimizedGroupEdit.h"
#include "GameStateManager.h"
#include "GUtils.h"

OptimizedGroupEdit::OptimizedGroupEdit(OptimizedGroup* group)
{
	materialPtr = group->mat;
	groupName = group->name;
	changedMaterial = true;

	loadMaterial();

	ogChanged = Global::gameMgr->sceneEdits.loadSavedMaterialChanges(*this, groupName);

	rows = { { "Opt_" + groupName,EditRow::Caption } ,{ originName,EditRow::Caption },{ "Save",EditRow::Action },{ "VS",EditRow::Static },{ "PS",EditRow::Params } };
}

void OptimizedGroupEdit::editChanged(EditVariable& var, const std::string& row)
{
	if (row == "PS")
	{
		ogChanged = true;
	}

	MaterialEdit::editChanged(var, row);
}

void OptimizedGroupEdit::customAction(std::string name)
{
	if (name == "Save")
	{
		if (ogChanged)
			Global::gameMgr->sceneEdits.addOptimizedGroupEdit(*this, groupName);
		else
			Global::gameMgr->sceneEdits.removeOptimizedGroupEdit(groupName);
	}
}

OptimizedGroupEdit* OptimizedGroupEdit::query()
{
	auto group = Global::gameMgr->geometryMgr->getClosestOptGroup();

	if(!group.mat.isNull())
	{
		if (group.mat->getTechnique(0)->getNumPasses() > 0)
		{
			auto edit = new OptimizedGroupEdit(&group);

			return edit;
		}
	}

	return nullptr;
}

void OptimizedGroupEdit::applyChanges(const std::map < std::string, OptimizedGroupEdit >& changes)
{
	auto groups = Global::gameMgr->geometryMgr->getOptGroups();

	for (auto& g : groups)
	{
		auto it = changes.find(g.name);

		if (it != changes.end())
		{
			applyMaterialChanges(g.mat, it->second);
		}
	}
}
