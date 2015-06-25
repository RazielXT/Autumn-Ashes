#pragma once

#include "stdafx.h"
#include "GeometryPreset.h"

class BasicGeometryPreset : public GeometryPreset
{
public:

    virtual ~BasicGeometryPreset() {}

    virtual void addGeometry(MaskGrid& grid, GeometryMaskInfo& gridInfo, GeometryPresetInfo& info);
    virtual void clear();

protected:

    virtual void init(GeometryPresetInfo& info);
    std::vector<std::string> possibleEntities;

    virtual bool acceptsWeight(float w) const;
    void placeObject(Ogre::Vector3 pos, Ogre::Quaternion or, float scale, Ogre::Vector3 color);

    void updateMaterial(Ogre::Entity* ent, Ogre::Vector3& color);
    std::map<std::string, Ogre::Material*> materials;

    Ogre::StaticGeometry* sg = nullptr;

    Ogre::Vector2 stepSize;
    float maxSteepY;

};