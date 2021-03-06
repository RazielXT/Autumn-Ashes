#include "stdafx.h"
#include "DetailGeometryEdit.h"
#include "ManualDetailGeometry.h"
#include "BasicDetailGeometry.h"

DetailGeometryEdit::DetailGeometryEdit(LoadedDG* manualDG)
{
	sg = manualDG->sg;
	entity = nullptr;

	for (size_t i = 0; i < manualDG->usedMats.size(); i++)
	{
		auto matPtr = Ogre::MaterialManager::getSingleton().getByName(manualDG->usedMats[i]->getName());
		auto vsVars = generateVsParams(matPtr);
		auto psVars = generatePsParams(matPtr);

		matsArray.push_back(matPtr);
		psVariablesMap[matPtr->getName()] = psVars;
		vsVariablesMap[matPtr->getName()] = vsVars;
	}

	materialPtr = matsArray[currentMatId];
	loadMaterial();
	mainMaterial = originName;
	dgName = manualDG->name;

	changedMaterial = Global::gameMgr->sceneEdits.loadSavedDetailGeometryChanges(*this, dgName);

	rows.push_back({ dgName,EditRow::Caption });
	for (auto& mat : matsArray)
	{
		rows.push_back({ mat->getName(),rows.size() == 1 ? EditRow::Caption : EditRow::Action });
	}
	rows.push_back({ "Save",EditRow::Action });

	rows.push_back({ "VS",EditRow::Params });
	rows.push_back({ "PS",EditRow::Params });
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
		if (changedMaterial)
			Global::gameMgr->sceneEdits.addDetailGeometryEdit(*this, dgName);
		else
			Global::gameMgr->sceneEdits.removeDetailGeometryEdit(dgName);
	}
	else
	{
		//materials switching
		for (size_t i = 0; i < matsArray.size(); i++)
		{
			auto& mat = matsArray[i];
			if (mat->getName() == name)
			{
				rows[currentMatId + 1].type = EditRow::Action;
				rows[i + 1].type = EditRow::Caption;

				currentMatId = i;
				materialPtr = mat;
				loadMaterial();
			}
		}
	}
}

void DetailGeometryEdit::merge(DetailGeometryEdit& r, bool addNotExisting)
{
	dgName = r.dgName;

	for (auto& vsVarsIt : r.vsVariablesMap)
	{
		auto& target = vsVariablesMap[vsVarsIt.first];

		mergeParams(vsVarsIt.second, target, addNotExisting);
	}

	for (auto& psVarsIt : r.psVariablesMap)
	{
		auto& target = psVariablesMap[psVarsIt.first];

		mergeParams(psVarsIt.second, target, addNotExisting);
	}
}

DetailGeometryEdit* DetailGeometryEdit::query()
{
	auto dg = ManualDetailGeometry::getClosest();

	if (!dg || dg->usedMats.empty())
	{
		dg = &BasicDetailGeometry::loadedDG[0];

		if (!dg || dg->usedMats.empty())
			return nullptr;
	}

	auto edit = new DetailGeometryEdit(dg);

	return edit;
}

void DetailGeometryEdit::applyAllChanges(std::map < std::string, DetailGeometryEdit >& changes)
{
	for (auto& savedDg : changes)
	{
		auto& loadedDg = ManualDetailGeometry::loadedMDG;

		LoadedDG* dgInfo = nullptr;
		for (auto& ldg : loadedDg)
		{
			if (ldg.name == savedDg.first)
			{
				dgInfo = &ldg;
			}
		}

		if (!dgInfo)
		{
			auto& loadedDg = BasicDetailGeometry::loadedDG;

			for (auto& ldg : loadedDg)
			{
				if (ldg.name == savedDg.first)
				{
					dgInfo = &ldg;
				}
			}
		}

		if (dgInfo)
		{
			for (auto mat : dgInfo->usedMats)
			{
				for (auto savedMat : savedDg.second.psVariablesMap)
				{
					if (savedMat.first == mat->getName())
					{
						for (auto& var : savedMat.second)
						{
							int pass = mat->getTechnique(0)->getNumPasses() - 1;
							mat->getTechnique(0)->getPass(pass)->getFragmentProgramParameters()->setNamedConstant(var.name, var.buffer, 1, var.size);
						}

						break;
					}
				}

				for (auto savedMat : savedDg.second.vsVariablesMap)
				{
					if (savedMat.first == mat->getName())
					{
						for (auto& var : savedMat.second)
						{
							int pass = mat->getTechnique(0)->getNumPasses() - 1;
							mat->getTechnique(0)->getPass(pass)->getVertexProgramParameters()->setNamedConstant(var.name, var.buffer, 1, var.size);
						}

						break;
					}
				}
			}
		}
	}
}

void DetailGeometryEdit::resetMaterial()
{
	//psVariables = psVariablesArray[currentMatId];
}

void DetailGeometryEdit::materialChanged()
{
	vsVariablesMap[matsArray[currentMatId]->getName()] = vsVariables;
	psVariablesMap[matsArray[currentMatId]->getName()] = psVariables;
	changedMaterial = true;

	//MaterialEdit::materialChanged();
}

