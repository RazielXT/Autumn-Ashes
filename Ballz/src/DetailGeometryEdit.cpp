#include "stdafx.h"
#include "DetailGeometryEdit.h"
#include "ManualDetailGeometry.h"

DetailGeometryEdit::DetailGeometryEdit(LoadedManualDG* manualDG, int matID)
{
    sg = manualDG->sg;
    entity = nullptr;

    materialPtr = Ogre::MaterialManager::getSingleton().getByName(manualDG->usedMats[matID]->getName());

    loadMaterial();
    dgName = manualDG->name + "_" + std::to_string(manualDG->id);

    changed = Global::gameMgr->sceneEdits.loadSavedDetailGeometryChanges(*this, dgName + originName);

    rows = { { dgName,EditRow::Caption },{ originName,EditRow::Caption },{ "Save",EditRow::Action },{ "PS",EditRow::Params } };
}

EditVariables* DetailGeometryEdit::getParams(const std::string& row)
{
    return MaterialEdit::getParams(row);
}

void DetailGeometryEdit::editChanged(EditVariable& var, const std::string& row)
{
    MaterialEdit::editChanged(var, row);
}

void DetailGeometryEdit::customAction(std::string name)
{
    if (name == "Save")
    {
        if (changed)
            Global::gameMgr->sceneEdits.addDetailGeometryEdit(*this, dgName + originName);
        else
            Global::gameMgr->sceneEdits.removeDetailGeometryEdit(dgName + originName);
    }
}

DetailGeometryEdit* DetailGeometryEdit::query()
{
    auto dg = ManualDetailGeometry::getClosest();

    if (!dg)
        return nullptr;

    auto edit = new DetailGeometryEdit(dg, 0);

    return edit;
}

void DetailGeometryEdit::applyChanges(std::map < std::string, DetailGeometryEdit >& changes)
{
    /*for (auto& dg : changes)
    {
    	if (Global::mSceneMgr->hasEntity(ent.first))
    	{
    		auto e = Global::mSceneMgr->getEntity(ent.first);
    		auto curMat = e->getSubEntity(0)->getMaterial();

    		if (ent.second.originName == curMat->getName())
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
    }*/
}

void DetailGeometryEdit::resetMaterial()
{
    MaterialEdit::resetMaterial();
}

void DetailGeometryEdit::materialChanged()
{
    MaterialEdit::materialChanged();
}

