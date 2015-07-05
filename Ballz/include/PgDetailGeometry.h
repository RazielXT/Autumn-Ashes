#pragma once

#include "stdafx.h"
#include "DetailGeometry.h"

class PgDetailGeometry : public DetailGeometry
{
public:

    virtual ~PgDetailGeometry() {}

    virtual void addGeometry(MaskGrid& grid, GeometryMaskInfo& gridInfo, DetailGeometryParams& info);
    virtual void clear();

protected:

    virtual void init(DetailGeometryParams& info);
    std::vector<std::string> possibleEntities;

    virtual bool acceptsWeight(float w) const;
    void placeObject(Ogre::Vector3 pos, Ogre::Quaternion or, float scale, Ogre::Vector3 color, Forests::TreeLoader3D *treeLoader);

    void updateMaterial(Ogre::Entity* ent, Ogre::Vector3& color);
    std::map<std::string, Ogre::Material*> materials;

    GeometryManager* parent;
    Forests::PagedGeometry *trees = nullptr;

    Ogre::Vector2 stepSize;
    float maxSteepY;

};