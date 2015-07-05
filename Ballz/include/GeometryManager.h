#pragma once

#include "stdafx.h"
#include <vector>
#include "DetailGeometry.h"


class GeometryManager
{
public:

    void forgetPagedGeometry(Forests::PagedGeometry *g);
    void addPagedGeometry(Forests::PagedGeometry *g);
    void clear();
    void update();

    void postLoad();
    void addDetailGeometryEntity(int id, Ogre::SceneNode* node, std::string type, bool keepMesh, Ogre::Vector3 color);
    void addDetailGeometryMask(Ogre::Entity* maskEnt, std::vector<DetailGeometryParams>& geometries, OgreNewt::Body* target, float rayDistance);

    void resetMaskedDetailGeometries();

    //editFunc = (entity*, pos*, tc*, vc*)
    bool modifyVertexBuffer(Ogre::Entity* ent, std::function<void(Ogre::Entity*, float*, float*, Ogre::RGBA*)> editFunc);

private:

    std::vector<DetailGeometry *> detailGeometries;
    DetailGeometry* getInstance(std::string name);

    void generateGeometryMask(Ogre::Entity* maskEnt, MaskGrid& grid, Ogre::Vector2& size);

    std::vector<Forests::PagedGeometry *> pagedGeometries;
};

