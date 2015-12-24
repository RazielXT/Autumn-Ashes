#include "stdafx.h"
#include "MaterialEditsLibrary.h"
#include "GameStateManager.h"

#include <iostream>

void MaterialEdit::mergeChanges(MaterialEdit& r, bool addNotExisting)
{
    originMatName = r.originMatName;

    for (auto& var : r.psVariables)
    {
        if (var.edited)
        {
            bool found = false;

            for (auto& mvar : psVariables)
            {
                if (mvar.name == var.name && mvar.size == var.size)
                {
                    memcpy(mvar.buffer, var.buffer, 4 * var.size);
                    found = true;
                }
            }

            if (!found && addNotExisting)
            {
                MaterialVariable mvar;
                mvar.edited = true;
                mvar.name = var.name;
                mvar.size = var.size;
                memcpy(mvar.buffer, var.buffer, 4 * var.size);

                psVariables.push_back(mvar);
            }
        }

    }
}

void MaterialEditsLibrary::addEdit(MaterialEdit& edit, std::string entName)
{
    auto& ent = editHistory[entName];

    if (ent.originMatName.empty())
        ent.originMatName = edit.originMatName;

    ent.mergeChanges(edit, true);

    saveFile(Global::gameMgr->getCurrentLvlInfo()->path);
}

void MaterialEditsLibrary::removeEdit(std::string entName)
{
    auto& ent = editHistory.begin();

    while (ent != editHistory.end())
    {
        if (ent->first == entName)
        {
            editHistory.erase(ent);
            break;
        }
    }

    saveFile(Global::gameMgr->getCurrentLvlInfo()->path);
}

bool MaterialEditsLibrary::loadSavedChanges(MaterialEdit& edit, std::string entName)
{
    auto& ent = editHistory.begin();

    while (ent != editHistory.end())
    {
        if (ent->first == entName)
        {
            edit.mergeChanges(ent->second, false);
            return true;
        }

        ent++;
    }

    return false;
}

MaterialEditsLibrary::MaterialEditsLibrary()
{
}

void MaterialEditsLibrary::applyChanges()
{
    loadFile(Global::gameMgr->getCurrentLvlInfo()->path);

    for (auto& ent : editHistory)
    {
        if (Global::mSceneMgr->hasEntity(ent.first))
        {
            auto e = Global::mSceneMgr->getEntity(ent.first);
            auto curMat = e->getSubEntity(0)->getMaterial();

            if (ent.second.originMatName == curMat->getName())
            {
                auto newMat = curMat->clone(curMat->getName() + std::to_string(idCounter++));
                e->setMaterial(newMat);

                for (auto& var : ent.second.psVariables)
                {
                    newMat->getTechnique(0)->getPass(1)->getFragmentProgramParameters()->setNamedConstant(var.name, var.buffer, 1, var.size);
                }
            }
        }
    }
}

void MaterialEditsLibrary::saveFile(std::string path)
{
    std::ofstream ofs(path + "materialEdits");
    boost::archive::text_oarchive oa(ofs);
    oa << this;
}

void MaterialEditsLibrary::loadFile(std::string path)
{
    std::ifstream ifs(path + "materialEdits");

    if (ifs.good())
    {
        boost::archive::text_iarchive ia(ifs);

        MaterialEditsLibrary* arch;
        ia >> arch;

        editHistory = std::move(arch->editHistory);
        delete arch;
    }
}
