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

	ogChanged = Global::gameMgr->sceneEdits.loadMaterialChanges(*this, groupName);

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
	static std::string lastSelected;

	auto groups = Global::gameMgr->geometryMgr->getClosestOptGroup(5);

	if (groups.empty())
		return nullptr;

	auto group = *groups.begin();
	for (auto g = groups.begin(); g != groups.end(); g++)
	{
		if (g->name == lastSelected)
		{
			g++;

			if (g != groups.end())
			{
				group = *g;
			}

			break;
		}
	}

	lastSelected = group.name;

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

void OptimizedGroupEdit::applyAllChanges(const std::map < std::string, OptimizedGroupEdit >& changes)
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

