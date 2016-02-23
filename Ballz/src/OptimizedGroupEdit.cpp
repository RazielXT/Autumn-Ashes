#include "stdafx.h"
#include "OptimizedGroupEdit.h"
#include "GameStateManager.h"
#include "GUtils.h"

OptimizedGroupEdit::OptimizedGroupEdit(OptimizedGroup* group)
{
    materialPtr = group->mat;

    loadMaterial();
    changed = true;
    varsChanged = Global::gameMgr->sceneEdits.loadSavedMaterialChanges(*this, entity->getName());
	groupId = group->id;

    rows = { { "Opt_" + groupId,EditRow::Caption } ,{ originName,EditRow::Caption },{ "Save",EditRow::Action },{ "VS",EditRow::Static },{ "PS",EditRow::Params } };
}

void OptimizedGroupEdit::customAction(std::string name)
{
    if (name == "Save")
    {
    	if (varsChanged)
    		Global::gameMgr->sceneEdits.addOptimizedGroupEdit(*this, groupId);
    	else
    		Global::gameMgr->sceneEdits.removeOptimizedGroupEdit(groupId);
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
		auto it = changes.find(g.id);

		if (it != changes.end())
		{
			applyMaterialChanges(g.mat, it->second);
		}
	}
}

