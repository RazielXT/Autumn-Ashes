#pragma once

#include "stdafx.h"
#include "DetailGeometry.h"

class BasicDetailGeometry : public DetailGeometry
{
public:

    virtual ~BasicDetailGeometry() {}

    virtual void addGeometry(MaskGrid& grid, GeometryMaskInfo& gridInfo, DetailGeometryInfo& info);
    virtual void clear();

protected:

    virtual void init(DetailGeometryInfo& info);
    std::vector<std::string> possibleEntities;

    virtual bool acceptsWeight(float w) const;
    void placeObject(Ogre::Vector3 pos, Ogre::Quaternion or, float scale, Ogre::Vector3 color);

    void updateMaterial(Ogre::Entity* ent, Ogre::Vector3& color);
    std::map<std::string, Ogre::Material*> materials;
    static std::vector<std::string> darkenVCMeshes;
    static std::vector<std::string> darkenVCMeshesDone;

    Ogre::StaticGeometry* sg = nullptr;

    Ogre::Vector2 stepSize;
    float generalScale = 1;
    float maxSteepY;
    float maxDistance;
};