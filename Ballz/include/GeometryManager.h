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

    void addDetailGeometry(Ogre::Entity* maskEnt, std::vector<DetailGeometryInfo>& geometries, OgreNewt::Body* targetarget, float rayDistance);

    void resetDetailGeometries();

    //editFunc = (entity*, pos*, tc*, vc*)
    bool modifyVertexBuffer(Ogre::Entity* ent, std::function<void(Ogre::Entity*, float*, float*, Ogre::RGBA*)> editFunc);

private:

    std::vector<DetailGeometry *> detailGeometries;
    DetailGeometry* getInstance(std::string name);

    void generateGeometryMask(Ogre::Entity* maskEnt, MaskGrid& grid, Ogre::Vector2& size);

    std::vector<Forests::PagedGeometry *> pagedGeometries;
};

