#pragma once

#include "stdafx.h"
#include "DetailGeometry.h"
#include "DetailGeometryInfo.h"
#include "DetailGeometryMaterial.h"

class PgDetailGeometry : public DetailGeometry
{
public:

    virtual ~PgDetailGeometry() {}

    virtual void addGeometry(MaskGrid& grid, GeometryMaskInfo& gridInfo, DetailGeometryParams& info);
    virtual void clear();

protected:

    virtual void init(DetailGeometryParams& info);
    DetailGeometryInfo info;
    DetailGeometryMaterial mats;

    virtual bool acceptsWeight(float w) const;
    void placeObject(Ogre::Vector3 pos, Ogre::Quaternion or, float scale, Ogre::Vector3 color, Forests::TreeLoader3D *treeLoader);


    GeometryManager* parent;
    Forests::PagedGeometry *trees = nullptr;
    std::vector<Ogre::Entity*> temps;

};