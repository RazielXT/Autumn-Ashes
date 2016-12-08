#include "stdafx.h"
#include "GeometryManager.h"
#include "BasicDetailGeometry.h"
#include "PgDetailGeometry.h"
#include "GrassDetailGeometry.h"
#include "ManualDetailGeometry.h"
#include "BillboardDetailGeometry.h"
#include "Player.h"
#include "PSSMCamera.h"

using namespace Ogre;

void GeometryManager::forgetPagedGeometry(Forests::PagedGeometry *g)
{
	auto it = std::find(pagedGeometries.begin(), pagedGeometries.end(), g);

	if (it != pagedGeometries.end())
		pagedGeometries.erase(it);

	for (auto it2 = namedPagedGeometries.begin(); it2 != namedPagedGeometries.end(); it2++)
	{
		if (it2->second == g)
		{
			namedPagedGeometries.erase(it2);
			break;
		}
	}
}

void GeometryManager::addPagedGeometry(Forests::PagedGeometry *g, std::string name)
{
	auto it = namedPagedGeometries.find(name);

	if (it != namedPagedGeometries.end())
	{
		auto f = it->second;
		forgetPagedGeometry(f);
		delete f->getPageLoader();
		delete f;
	}

	namedPagedGeometries[name] = g;

	addPagedGeometry(g);
}

void GeometryManager::addPagedGeometry(Forests::PagedGeometry *g)
{
	pagedGeometries.push_back(g);
}

void GeometryManager::addPagedGeometry(Forests::PagedGeometry *g, std::string name, LightBakeInfo& info)
{
	addPagedGeometry(g, name);
	lightBakingTodo.push_back(info);
}

void GeometryManager::clear()
{
	for(auto g : pagedGeometries)
	{
		delete g->getPageLoader();
		delete g;
	}

	pagedGeometries.clear();
	namedPagedGeometries.clear();

	for (auto g : detailGeometries)
	{
		delete g;
	}

	detailGeometries.clear();

	DetailGeometryMaterial::reset();
	BasicDetailGeometry::loadedDG.clear();
	optimizedGroups.clear();
}

void GeometryManager::postLoad()
{
	ManualDetailGeometry::buildAll();
	bakeLights();
	update();
}

void GeometryManager::update()
{
	for(auto g : pagedGeometries)
	{
		g->update();
	}
}

void GeometryManager::bakeLight(LightBakeInfo& info, Ogre::Camera* cam, Ogre::TexturePtr texture)
{
	if (!info.layer)
		return;

	cam->setFarClipDistance(info.distance);
	cam->setOrthoWindow(info.size.x, info.size.y);
	cam->setPosition(info.pos);
	cam->setOrientation(Ogre::Quaternion(Ogre::Radian(Ogre::Degree(-90)), Ogre::Vector3(1, 0, 0)));

	auto shSetup = Global::sceneMgr->getShadowCameraSetup();
	PSSMShadowCameraSetup2* pssmSetup = (PSSMShadowCameraSetup2*)shSetup.get();

	Ogre::PSSMShadowCameraSetup2::SplitPointList splitPointList = pssmSetup->getSplitPoints();
	auto origSplit = splitPointList[pssmSetup->getSplitCount()];
	auto bestSplit = std::max(origSplit, std::max(info.size.x, info.size.y));
	splitPointList[pssmSetup->getSplitCount()] = bestSplit;
	pssmSetup->setSplitPoints(splitPointList);

	auto v = cam->getViewport();
	uint32_t flag = VisibilityFlag_Normal;
	Ogre::Entity* ent = nullptr;

	if (info.groundName.empty())
	{
		v->setVisibilityMask(VisibilityFlag_Normal);
		cam->setVisibilityFlags(VisibilityFlag_Normal);
	}
	else
	{
		v->setVisibilityMask(VisibilityFlag_Temp);
		cam->setVisibilityFlags(VisibilityFlag_Temp);

		ent = Global::sceneMgr->getEntity(info.groundName);
		flag = ent->getVisibilityFlags();
		ent->setVisibilityFlags(VisibilityFlag_Temp);
	}

	auto target = texture->getBuffer()->getRenderTarget();
	target->update();

	target->writeContentsToFile("baking.jpg");

	info.layer->setColorMap(texture);

	splitPointList[pssmSetup->getSplitCount()] = origSplit;
	pssmSetup->setSplitPoints(splitPointList);

	if (ent)
		ent->setVisibilityFlags(flag);
}

void GeometryManager::bakeLights()
{
	Ogre::SceneManager::MovableObjectIterator iterator = Global::sceneMgr->getMovableObjectIterator("Entity");
	while (iterator.hasMoreElements())
	{
		Ogre::Entity* e = static_cast<Ogre::Entity*>(iterator.getNext());

		Ogre::LogManager::getSingleton().getLog("RuntimeEvents.log")->logMessage(e->getName() + "\t" + std::to_string(e->getVisibilityFlags()) + "\t" + e->getSubEntity(0)->getMaterialName());
		//e->setVisibilityFlags(e->getVisibilityFlags() & ~VisibilityFlag_Temp);
	}

	Global::sceneMgr->setFog(FOG_LINEAR, ColourValue::White, 0, 10000,15000);

	auto lightBakingCam = Global::sceneMgr->createCamera("lightBaking");
	lightBakingCam->setNearClipDistance(1);
	lightBakingCam->setFarClipDistance(100);
	//mReflectCam->setAspectRatio(1);
	lightBakingCam->setProjectionType(PT_ORTHOGRAPHIC);
	lightBakingCam->setDirection(0, -1, 0);

	auto texture = TextureManager::getSingleton().createManual("bakeLight",
	               ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, TEX_TYPE_2D,
	               512, 512, 0, PF_R8G8B8A8, TU_RENDERTARGET);

	auto rttTex = texture->getBuffer()->getRenderTarget();

	Viewport *v = rttTex->addViewport(lightBakingCam);
	v->setClearEveryFrame(true);
	v->setSkiesEnabled(false);
	v->setShadowsEnabled(true);
	v->setBackgroundColour(ColourValue(1,0,0,0));
	v->setVisibilityMask(VisibilityFlag_Normal);

	for (auto& info : lightBakingTodo)
		bakeLight(info, lightBakingCam, texture);

	texture->unload();
	lightBakingTodo.clear();
	Global::sceneMgr->destroyCamera(lightBakingCam);

	Global::gameMgr->sceneEdits.getLevelEdit()->applyFog();
}

DetailGeometry* GeometryManager::getInstance(std::string name)
{
	if (name == "RocksPG")
		//return new BillboardDetailGeometry();
		return new PgDetailGeometry();
	if (name == "Rocks" || name == "TreesAspen" || name == "Bush")
		return new BasicDetailGeometry();
	if (name == "Grass")
		return new GrassDetailGeometry();

	return nullptr;
}


void GeometryManager::resetMaskedDetailGeometries()
{
	for (auto g : detailGeometries)
	{
		g->clear();
		delete g;
	}

	detailGeometries.clear();
}

void GeometryManager::addDetailGeometryEntity(int id, Ogre::SceneNode* node, std::string type, bool keepMesh, Vector3 color)
{
	auto pg = ManualDetailGeometry::get(id);
	pg->addObject(node, type, keepMesh, color);
}

void GeometryManager::addDetailGeometryMask(Ogre::Entity* maskEnt, std::vector<DetailGeometryParams>& geometries, OgreNewt::Body* target, float rayDistance)
{
	MaskGrid posGrid;
	GeometryMaskInfo info;
	info.rayDistance = rayDistance;
	info.target = target;
	info.node = maskEnt->getParentSceneNode();
	info.parent = this;

	generateGeometryMask(maskEnt, posGrid, info.size);

	for (auto g : geometries)
	{
		auto p = getInstance(g.name);

		if (p)
		{
			detailGeometries.push_back(p);

			p->addGeometry(posGrid, info, g);
		}
	}
}

void loadPoint(float* pReal, Ogre::RGBA pCReal, GeometryMaskPoint& out, SceneNode* node)
{
	out.pos.x = (*pReal++);
	out.pos.y = (*pReal++);
	out.pos.z = (*pReal++);

	out.pos *= node->getScale();
	// out[i] = node->getOrientation()*out[i];
	// out.pos += node->getPosition();

	out.color.setAsARGB(pCReal);
}

bool GeometryManager::modifyVertexBuffer(Entity* ent, VCEditFunc editFunc, void* data)
{
	auto node = ent->getParentSceneNode();
	auto m = ent->getMesh().get()->getSubMesh(0);

	const Ogre::VertexElement* posElem = m->vertexData->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);
	const Ogre::VertexElement* posElemCol = m->vertexData->vertexDeclaration->findElementBySemantic(Ogre::VES_DIFFUSE);
	const Ogre::VertexElement* posElemTC = m->vertexData->vertexDeclaration->findElementBySemantic(Ogre::VES_TEXTURE_COORDINATES);

	if (posElem == nullptr)
		return false;

	Ogre::HardwareVertexBufferSharedPtr vbuf = m->vertexData->vertexBufferBinding->getBuffer(posElem->getSource());
	unsigned char* vertex = static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_NORMAL));
	Ogre::Real* pReal;
	Ogre::Real* pTCReal = nullptr;
	Ogre::RGBA* pCReal = nullptr;

	for (size_t j = 0; j < m->vertexData->vertexCount; j ++)
	{
		posElem->baseVertexPointerToElement(vertex, &pReal);

		if (posElemCol)
			posElemCol->baseVertexPointerToElement(vertex, &pCReal);

		if (posElemTC)
			posElemTC->baseVertexPointerToElement(vertex, &pTCReal);

		editFunc(ent, pReal, pTCReal, pCReal, data);

		vertex += vbuf->getVertexSize();
	}

	vbuf->unlock();

	return true;
}

void GeometryManager::generateGeometryMask(Ogre::Entity* maskEnt, MaskGrid& posGrid, Vector2& size)
{
	auto node = maskEnt->getParentSceneNode();
	auto m = maskEnt->getMesh().get()->getSubMesh(0);

	const Ogre::VertexElement* posElem = m->vertexData->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);
	const Ogre::VertexElement* posElemCol = m->vertexData->vertexDeclaration->findElementBySemantic(Ogre::VES_DIFFUSE);
	Ogre::HardwareVertexBufferSharedPtr vbuf = m->vertexData->vertexBufferBinding->getBuffer(posElem->getSource());
	unsigned char* vertex = static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
	Ogre::Real* pReal;
	Ogre::RGBA* pCReal;

	std::vector<GeometryMaskPoint> loadedRow;
	GeometryMaskPoint temp[4];

	bool isFirstRow = true;
	bool firstCol = true;
	std::vector<GeometryMaskPoint> firstRow;
	float lastRowStart = 0;

	if ((m->vertexData->vertexCount % 6) == 0 && posElemCol != NULL)
	{
		for (size_t j = 0; j < m->vertexData->vertexCount; j += 6)
		{
			posElem->baseVertexPointerToElement(vertex, &pReal);
			posElemCol->baseVertexPointerToElement(vertex, &pCReal);
			loadPoint(pReal, *pCReal, temp[0], node);
			vertex += vbuf->getVertexSize();

			posElem->baseVertexPointerToElement(vertex, &pReal);
			posElemCol->baseVertexPointerToElement(vertex, &pCReal);
			loadPoint(pReal, *pCReal, temp[1], node);
			vertex += vbuf->getVertexSize();

			posElem->baseVertexPointerToElement(vertex, &pReal);
			posElemCol->baseVertexPointerToElement(vertex, &pCReal);
			loadPoint(pReal, *pCReal, temp[2], node);
			vertex += vbuf->getVertexSize() * 2;

			posElem->baseVertexPointerToElement(vertex, &pReal);
			posElemCol->baseVertexPointerToElement(vertex, &pCReal);
			loadPoint(pReal, *pCReal, temp[3], node);
			vertex += vbuf->getVertexSize() * 2;

			//next row, unless starting
			if (!(isFirstRow && firstCol) && lastRowStart >= temp[0].pos.x)
			{
				if (isFirstRow)
					posGrid.push_back(firstRow);

				posGrid.push_back(loadedRow);

				firstCol = true;
				isFirstRow = false;

				loadedRow.clear();
			}

			if (isFirstRow)
			{
				if (firstCol)
					firstRow.push_back(temp[0]);

				firstRow.push_back(temp[1]);
			}

			if (firstCol)
				loadedRow.push_back(temp[3]);
			loadedRow.push_back(temp[2]);

			lastRowStart = temp[0].pos.x;
			firstCol = false;
		}

		if (m->vertexData->vertexCount == 6)
		{
			posGrid.push_back(firstRow);
		}

		posGrid.push_back(loadedRow);


		size.x = temp[2].pos.x - posGrid[0][0].pos.x;
		size.y = posGrid[0][0].pos.z - temp[2].pos.z;
	}

	vbuf->unlock();
}

void GeometryManager::addOptimizedGroup(OptimizedGroup group)
{
	optimizedGroups.push_back(group);
}

std::vector<OptimizedGroup>& GeometryManager::getOptGroups()
{
	return optimizedGroups;
}

std::vector<OptimizedGroup> GeometryManager::getClosestOptGroup(float radius)
{
	auto pos = Global::sceneMgr->getCamera("Camera")->getDerivedPosition();
	std::vector<std::pair<float, OptimizedGroup>> inReach;

	for (auto& g : optimizedGroups)
	{
		float dist = g.bbox.distance(pos);
		if (dist < radius)
		{
			inReach.push_back({ dist, g });
		}
	}

	std::sort(inReach.begin(), inReach.end(), [](std::pair<float, OptimizedGroup>& left, std::pair<float, OptimizedGroup>& right)
	{
		return left.first < right.first;
	});

	std::vector<OptimizedGroup> out;
	for (auto& o : inReach)
		out.push_back(o.second);

	return out;
}