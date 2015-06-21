#pragma once

#include "stdafx.h"
#include "GeometryPreset.h"

class BasicGeometryPreset : public GeometryPreset
{
public:

    virtual ~BasicGeometryPreset() {}

    virtual void addGeometry(MaskGrid& grid, GeometryMaskInfo& gridInfo, GeometryPresetInfo& info);

protected:

    virtual void init(GeometryPresetInfo& info);
    virtual bool acceptsWeight(float w) const;
    virtual void placeObject(Ogre::Vector3 pos, Ogre::Quaternion or, float scale, Ogre::Vector3 color) = 0;

    void updateMaterial(Ogre::Entity* ent, Ogre::Vector3& color);
    std::map<std::string, Ogre::Material*> materials;


    Ogre::Vector2 stepSize;
    float maxSteepY;

};