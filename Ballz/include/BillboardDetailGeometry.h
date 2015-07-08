#pragma once

#include "stdafx.h"
#include "DetailGeometry.h"
#include "DetailGeometryInfo.h"
#include "DetailGeometryMaterial.h"

class BillboardDetailGeometry : public DetailGeometry
{
public:

    virtual ~BillboardDetailGeometry() {}

    virtual void addGeometry(MaskGrid& grid, GeometryMaskInfo& gridInfo, DetailGeometryParams& info);
    virtual void clear();

protected:

    virtual void init(DetailGeometryParams& info);

    virtual bool acceptsWeight(float w) const;
    void placeObject(Ogre::Vector3 pos, Ogre::Quaternion or, float scale, Ogre::Vector3 color);
    Ogre::BillboardSet* getSet(std::string name);
    std::map<std::string, Ogre::BillboardSet*> mySets;

    DetailGeometryInfo info;
};