#include "stdafx.h"
#include "ManualDetailGeometry.h"
#include "MUtils.h"
#include "SUtils.h"
#include "DetailGeometryInfo.h"

using namespace Ogre;

std::map<int, ManualDetailGeometry*> ManualDetailGeometry::mdg;
std::vector<LoadedDG> ManualDetailGeometry::loadedMDG;

ManualDetailGeometry::ManualDetailGeometry(int id)
{
	info.id = id;
}

void ManualDetailGeometry::build()
{
	if (info.sg == nullptr)
		return;

	info.sg->build();
	info.name = info.name + "_" + std::to_string(info.id);
	info.usedMats = materialHelper.getGeneratedMaterials();

	loadedMDG.push_back(info);

	for (auto e : usedEntities)
	{
		Global::mSceneMgr->destroyEntity(e);
	}

	usedEntities.clear();
}

void ManualDetailGeometry::addObject(Ogre::SceneNode* node, std::string type, bool keepMesh, Vector3 color)
{
	info.bbox.merge(node->getPosition());

	auto dgTypeInfo = DetailGeometryInfo::get(type);

	if (info.sg == nullptr)
	{
		static int msgCount = 0;
		info.sg = Global::mSceneMgr->createStaticGeometry("manDG" + std::to_string(msgCount++));
		info.sg->setCastShadows(true);
		info.sg->setVisibilityFlags(VisibilityFlag_Normal);

		float staticEntitiesGridSize = dgTypeInfo.gridSize;
		Ogre::Vector3 gridRegion(staticEntitiesGridSize, staticEntitiesGridSize, staticEntitiesGridSize);
		info.sg->setRegionDimensions(gridRegion);
		//sg->setOrigin(gridRegion / 2.0f + gridInfo.node->getPosition());
		info.sg->setRenderingDistance(dgTypeInfo.maxDistance);
	}

	Quaternion qCorrect(Degree(180), Vector3(1, 0, 0));

	if (!keepMesh)
	{
		String meshName = dgTypeInfo.possibleEntities[(int)Math::RangeRandom(0, dgTypeInfo.possibleEntities.size() - 0.01f)];

		while (!meshName.empty())
		{
			auto mname = SUtils::strtok_str(meshName, ';');
			auto ent = Global::mSceneMgr->createEntity(mname);
			//node->attachObject(ent);

			materialHelper.updateMaterial(ent, color, dgTypeInfo);
			info.sg->addEntity(ent, node->getPosition(), node->getOrientation()*qCorrect, dgTypeInfo.generalScale*node->getScale());
			usedEntities.push_back(ent);
		}

		if (info.name.empty())
			info.name = type;
	}
	else
	{
		auto ent = (Entity*)node->getAttachedObject(0);
		node->detachAllObjects();

		materialHelper.updateMaterial(ent, color, dgTypeInfo);
		info.sg->addEntity(ent, node->getPosition(), node->getOrientation()*qCorrect, node->getScale());
		usedEntities.push_back(ent);

		if (info.name.empty())
			info.name = ent->getName();
	}
}

LoadedDG* ManualDetailGeometry::getClosest()
{
	if (!Global::player)
		return nullptr;

	LoadedDG* dgOut = nullptr;
	float closest = 999999;
	auto pos = Global::camera->getPosition();

	for (auto& dg : loadedMDG)
	{
		float dist = pos.squaredDistance(dg.bbox.getCenter());

		if (dist < closest)
		{
			dgOut = &dg;
			closest = dist;
		}
	}

	return dgOut;
}

void ManualDetailGeometry::buildAll()
{
	loadedMDG.clear();

	for (auto dgi : mdg)
	{
		auto dg = dgi.second;
		dg->build();
		delete dg;
	}

	mdg.clear();
}

ManualDetailGeometry* ManualDetailGeometry::get(int id)
{
	if (mdg.find(id) != mdg.end())
	{
		return mdg[id];
	}
	else
	{
		ManualDetailGeometry* dg = new ManualDetailGeometry(id);
		mdg[id] = dg;
		return dg;
	}
}