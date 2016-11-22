#include "stdafx.h"
#include "BasicDetailGeometry.h"
#include "MUtils.h"
#include "SUtils.h"
#include "GUtils.h"
#include "GameStateManager.h"
#include "DetailGeometryMaterial.h"
#include "OgreProgressiveMesh.h"

using namespace Ogre;

int DetailGeometry::matID = 0;

std::vector<LoadedDG> BasicDetailGeometry::loadedDG;

Ogre::InstanceManager* mBillboardsManager;
Ogre::InstanceManager* mTreesManager;
String bbName = "billboard.mesh";
String treeName = "aspenLeafs.mesh";

void BasicDetailGeometry::addGeometry(MaskGrid& grid, GeometryMaskInfo& gridInfo, DetailGeometryParams& params)
{
	init(params);

	float xStart = 0;
	float xEnd = gridInfo.size.x;
	float xStep = info.stepSize.x / params.density;

	float yStart = 0;
	float yEnd = gridInfo.size.y;
	float yStep = info.stepSize.y / params.density;

	static int sgCount = 0;

	sg = Global::sceneMgr->createStaticGeometry("basicDG" + std::to_string(sgCount++));
	sg->setCastShadows(true);
	sg->setVisibilityFlags(VisibilityFlag_Instanced);

	float staticEntitiesGridSize = info.gridSize;
	Ogre::Vector3 gridRegion(staticEntitiesGridSize, staticEntitiesGridSize, staticEntitiesGridSize);
	sg->setRegionDimensions(gridRegion);
	sg->setOrigin(gridRegion / 2.0f + gridInfo.node->getPosition());
	sg->setRenderingDistance(info.maxDistance);
	int bgc = 0;

	Ogre::AxisAlignedBox bbox;

	mBillboardsManager = Global::sceneMgr->createInstanceManager(
	                         "InstanceMgr", bbName,
	                         ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME, InstanceManager::HWInstancingBasic,
	                         300, 0);

	mTreesManager = Global::sceneMgr->createInstanceManager(
	                    "InstanceMgr2", treeName,
	                    ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME, InstanceManager::HWInstancingBasic,
	                    300, 0);

	for (float x = xStart; x <= xEnd; x+=xStep)
		for (float y = yStart; y <= yEnd; y += yStep)
		{
			float xOffset = Ogre::Math::RangeRandom(-xStep, xStep) / 2.0f;
			float yOffset = Ogre::Math::RangeRandom(-yStep, yStep) / 2.0f;
			auto pos = getMaskAt(grid, gridInfo, x + xOffset, y + yOffset);

			auto wmasked = (pos.color*params.weightMask);
			float w = wmasked.r + wmasked.g + wmasked.b + wmasked.a;

			float scaleMask = 1.0f;
			if (params.customEdit.customScaleEnabled)
			{
				auto smasked = (pos.color*params.customEdit.customScaleMask);
				float scaleW = wmasked.r + wmasked.g + wmasked.b + wmasked.a;
				scaleMask = MUtils::lerp(params.customEdit.customMinmaxScale.x, params.customEdit.customMinmaxScale.y, scaleW);
			}

			GUtils::RayInfo ray;
			bool foundRay = false;

			if (gridInfo.target)
				foundRay = GUtils::getRayFilteredInfo(pos.pos, gridInfo.node->getOrientation()*Vector3(0, -1, 0), gridInfo.rayDistance, ray, gridInfo.target);
			else
				foundRay = GUtils::getRayInfo(pos.pos, gridInfo.node->getOrientation()*Vector3(0, -1, 0), gridInfo.rayDistance, ray);

			if (foundRay && ray.normal.y >= info.maxSteepY && acceptsWeight(w))
			{
				float scale = info.generalScale*scaleMask*Ogre::Math::RangeRandom(params.minmaxScale.x, params.minmaxScale.y);
				placeObject(ray.pos, MUtils::quaternionFromNormal(ray.normal), scale, params.color);

				bbox.merge(ray.pos);

				bgc++;
			}
		}

	//sg->build();

	//mCurrentManager->setBatchesAsStaticAndUpdate(true);

	LoadedDG dgInfo;
	dgInfo.sg = sg;
	dgInfo.name = params.name + "_" + gridInfo.node->getName();
	dgInfo.id = 0;
	dgInfo.bbox = bbox;
	dgInfo.usedMats = mats.getGeneratedMaterials();

	loadedDG.push_back(dgInfo);

	for (auto e : temps)
		Global::sceneMgr->destroyEntity(e);

	temps.clear();
}

void BasicDetailGeometry::clear()
{
	/*for (auto e : entities)
	{
	    auto node = e->getParentSceneNode();
	    node->detachAllObjects();

	    Global::mSceneMgr->destroySceneNode(node);
	    Global::mSceneMgr->destroyEntity(e);
	}*/

	mats.reset();
	Global::sceneMgr->destroyStaticGeometry(sg);
}

bool BasicDetailGeometry::acceptsWeight(float w) const
{
	return w >= Ogre::Math::RangeRandom(0, 1);
}

void BasicDetailGeometry::init(DetailGeometryParams& param)
{
	info = DetailGeometryInfo::get(param.name);
}

int renderFar = 0;

void BasicDetailGeometry::placeObject(Vector3 pos, Quaternion or, float scale, Vector3 color)
{
	Quaternion randomYaw(Degree(Math::RangeRandom(0, 360)), Vector3(0, 1, 0));
	//String meshName = info.possibleEntities[(int)Math::RangeRandom(0, info.possibleEntities.size() - 0.01f)];
	//auto node = Global::mSceneMgr->getRootSceneNode()->createChildSceneNode(pos, randomYaw*or);
	//node->setScale(Vector3(scale, scale, scale));

	/*while (!meshName.empty())
	{
		auto name = SUtils::strtok_str(meshName, ';');
		auto ent = Global::mSceneMgr->createEntity(name);
		//node->attachObject(ent);

		mats.updateMaterial(ent, color,info);
		//sg->addEntity(ent, pos, randomYaw, Vector3(scale));
		pos.y -= 10;
		Global::mSceneMgr->getRootSceneNode()->createChildSceneNode(pos, randomYaw)->attachObject(ent);
		//temps.push_back(ent);
	}*/

	//pos.y -= 10;
	//Ogre::ProgressiveMesh prog;
	//prog.
	//auto ent = Global::mSceneMgr->createEntity("")->getMesh()->lod;
	InstancedEntity *ent = mBillboardsManager->createInstancedEntity("billboardTest"); //aspenTreeInstanced
	ent->setOrientation(randomYaw);
	ent->setPosition(pos);

	ent->setRenderingDistance(renderFar ? 1000 : 2000);

	renderFar = (renderFar + 1) % 3;

	mTreesManager->setSetting(InstanceManager::CAST_SHADOWS, false);

	ent = mTreesManager->createInstancedEntity("aspenTreeInstanced"); //aspenTreeInstanced
	ent->setOrientation(randomYaw);
	ent->setPosition(pos);
	ent->setRenderingDistance(300);

	/*OgreNewt::ConvexCollisionPtr col = OgreNewt::ConvexCollisionPtr(new OgreNewt::CollisionPrimitives::ConvexHull(Global::mWorld, ent, 0));
	auto body = new OgreNewt::Body(Global::mWorld, col);
	Ogre::Vector3 inertia, offset;
	col->calculateInertialMatrix(inertia, offset);
	auto userD = new bodyUserData();
	body->setUserData(Ogre::Any(userD));
	body->setMassMatrix(0, inertia);
	body->setCenterOfMass(offset);
	body->setPositionOrientation(node->_getDerivedPosition(), node->_getDerivedOrientation());*/
}

