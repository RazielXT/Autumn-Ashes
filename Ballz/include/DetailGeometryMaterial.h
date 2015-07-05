#pragma once

#include "stdafx.h"
#include "DetailGeometryInfo.h"
#include "GameStateManager.h"

class DetailGeometryMaterial
{
    std::map<std::string, Ogre::Material*> materials;

    static std::vector<std::string> darkenVCMeshes;
    static std::vector<std::string> darkenVCMeshesDone;

public:

    static void reset()
    {
        darkenVCMeshesDone.clear();
    }

    DetailGeometryMaterial()
    {
        if (darkenVCMeshes.empty())
        {
            darkenVCMeshes.push_back("aspenLeafs.mesh");
            darkenVCMeshes.push_back("aspen2Leafs.mesh");
            darkenVCMeshes.push_back("treeBunchLeafs.mesh");
            darkenVCMeshes.push_back("treeBunchBigLeafs.mesh");
            darkenVCMeshes.push_back("bush1.mesh");
        }
    }

    void updateMaterial(Ogre::Entity* ent, Ogre::Vector3& color, DetailGeometryInfo& info)
    {
        if (std::find(darkenVCMeshes.begin(), darkenVCMeshes.end(), ent->getMesh()->getName()) != darkenVCMeshes.end())
        {
            if (std::find(darkenVCMeshesDone.begin(), darkenVCMeshesDone.end(), ent->getMesh()->getName()) == darkenVCMeshesDone.end())
            {
                auto darken = [](Ogre::Entity* e, float* pos, float*, Ogre::RGBA* color)
                {
                    if (!color)
                        return;

                    float sideSize = std::max(e->getBoundingBox().getHalfSize().z, e->getBoundingBox().getHalfSize().x);
                    float ySize = e->getBoundingBox().getHalfSize().y;
                    float yCenter = e->getBoundingBox().getCenter().y;

                    Ogre::Vector3 vpos(pos[0], 0, pos[2]);
                    float topLight = pos[1] < yCenter ? 0.0f : std::max<float>(0.0f, (pos[1] - yCenter) / ySize);
                    float centerDark = vpos.length() / sideSize;

                    float dark = std::max(topLight, centerDark);
                    dark = Ogre::Math::Clamp<float>(dark, 0.0f, 1.0f);
                    *color = Ogre::ColourValue(0, 0, 0, dark).getAsARGB();
                };

                Global::gameMgr->geometryMgr->modifyVertexBuffer(ent, darken);

                darkenVCMeshesDone.push_back(ent->getMesh()->getName());
            }
        }


        if (color.x != 1 || color.y != 1 || color.z != 1)
        {
            auto name = ent->getSubEntity(0)->getMaterialName();
            static int matID = 0;

            if (materials.find(name) == materials.end())
            {
                auto mat = (Ogre::Material*)Ogre::MaterialManager::getSingleton().getByName(name).get();
                auto newMat = mat->clone(mat->getName() + "MGen" + std::to_string(++matID)).get();
                newMat->setDiffuse(Ogre::ColourValue(color.x, color.y, color.z, 1));

                materials[name] = newMat;
            }

            auto mat = materials[name];
            ent->getSubEntity(0)->setMaterialName(mat->getName());
        }
    }
};