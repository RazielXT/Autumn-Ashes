#pragma once

#include "stdafx.h"
#include <vector>
#include <map>
#include "DetailGeometry.h"

using VCEditFunc = std::function<void(Ogre::Entity*, float*, float*, Ogre::RGBA*, void* userData)>;

struct OptimizedGroup
{
	Ogre::AxisAlignedBox bbox;
	Ogre::MaterialPtr mat;
	std::string name;
};

struct LightBakeInfo
{
	Forests::GrassLayer* layer;
	Ogre::Vector3 pos;
	Ogre::Vector2 size;
	float distance;

	std::string groundName;
};

class GeometryManager
{
public:

	void forgetPagedGeometry(Forests::PagedGeometry *g);
	void addPagedGeometry(Forests::PagedGeometry *g);
	void addPagedGeometry(Forests::PagedGeometry *g, std::string name);
	void addPagedGeometry(Forests::PagedGeometry *g, std::string name, LightBakeInfo& info);

	void clear();
	void update();

	void postLoad();
	void addDetailGeometryEntity(int id, Ogre::SceneNode* node, std::string type, bool keepMesh, Ogre::Vector3 color);
	void addDetailGeometryMask(Ogre::Entity* maskEnt, std::vector<DetailGeometryParams>& geometries, OgreNewt::Body* target, float rayDistance);

	void resetMaskedDetailGeometries();

	//editFunc = (entity*, pos*, tc*, vc*)
	bool modifyVertexBuffer(Ogre::Entity* ent, VCEditFunc editFunc, void* data = nullptr);

	void addOptimizedGroup(OptimizedGroup group);
	std::vector<OptimizedGroup> getClosestOptGroup(float radius);
	std::vector<OptimizedGroup>& getOptGroups();

private:

	std::vector<LightBakeInfo> lightBakingTodo;
	void bakeLight(LightBakeInfo& info, Ogre::Camera* cam, Ogre::TexturePtr target);
	void bakeLights();

	std::vector<OptimizedGroup> optimizedGroups;

	std::vector<DetailGeometry *> detailGeometries;
	DetailGeometry* getInstance(std::string name);

	void generateGeometryMask(Ogre::Entity* maskEnt, MaskGrid& grid, Ogre::Vector2& size);

	std::map<std::string, Forests::PagedGeometry *> namedPagedGeometries;
	std::vector<Forests::PagedGeometry *> pagedGeometries;
};

