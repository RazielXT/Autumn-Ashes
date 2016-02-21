#include "stdafx.h"
#include "ManualDetailGeometry.h"
#include "MUtils.h"
#include "SUtils.h"
#include "DetailGeometryInfo.h"

using namespace Ogre;

std::map<int, ManualDetailGeometry*> ManualDetailGeometry::mdg;
std::vector<LoadedManualDG> ManualDetailGeometry::loadedMDG;

void ManualDetailGeometry::build()
{
    if (sg == nullptr)
        return;

    sg->build();
    loadedMDG.push_back({ bbox , sg , id, name, mats.getGeneratedMaterials() });

    for (auto e : usedEntities)
    {
        Global::mSceneMgr->destroyEntity(e);
    }

    usedEntities.clear();
}

void ManualDetailGeometry::addObject(Ogre::SceneNode* node, std::string type, bool keepMesh, Vector3 color)
{
    bbox.merge(node->getPosition());

    auto info = DetailGeometryInfo::get(type);

    if (sg == nullptr)
    {
        static int msgCount = 0;
        sg = Global::mSceneMgr->createStaticGeometry("manDG" + std::to_string(msgCount++));
        sg->setCastShadows(true);
        sg->setVisibilityFlags(VisibilityFlag_Normal);

        float staticEntitiesGridSize = info.gridSize;
        Ogre::Vector3 gridRegion(staticEntitiesGridSize, staticEntitiesGridSize, staticEntitiesGridSize);
        sg->setRegionDimensions(gridRegion);
        //sg->setOrigin(gridRegion / 2.0f + gridInfo.node->getPosition());
        sg->setRenderingDistance(info.maxDistance);
    }

    Quaternion qCorrect(Degree(180), Vector3(1, 0, 0));

    if (!keepMesh)
    {
        String meshName = info.possibleEntities[(int)Math::RangeRandom(0, info.possibleEntities.size() - 0.01f)];

        while (!meshName.empty())
        {
            auto name = SUtils::strtok_str(meshName, ';');
            auto ent = Global::mSceneMgr->createEntity(name);
            //node->attachObject(ent);

            mats.updateMaterial(ent, color, info);
            sg->addEntity(ent, node->getPosition(), node->getOrientation()*qCorrect, info.generalScale*node->getScale());
            usedEntities.push_back(ent);
        }

        if (name.empty())
            name = type;
    }
    else
    {
        auto ent = (Entity*)node->getAttachedObject(0);
        node->detachAllObjects();

        mats.updateMaterial(ent, color,info);
        sg->addEntity(ent, node->getPosition(), node->getOrientation()*qCorrect, node->getScale());
        usedEntities.push_back(ent);

        if (name.empty())
            name = ent->getName();
    }
}