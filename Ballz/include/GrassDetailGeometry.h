#pragma once

#include "stdafx.h"
#include "DetailGeometry.h"


class GrassDetailGeometry : public DetailGeometry
{
public:

    virtual ~GrassDetailGeometry() {}

    virtual void addGeometry(MaskGrid& grid, GeometryMaskInfo& gridInfo, DetailGeometryParams& info);
    virtual void clear();

protected:

    std::vector<Ogre::Entity*> entities;

    virtual void init(DetailGeometryParams& info);

    virtual bool acceptsWeight(float w) const;

    bool placeObject(GeometryMaskInfo& gridInfo, Ogre::Vector3 pos, float scale, Ogre::Vector3 color);
    Ogre::Mesh* makeMesh();

    struct GrassDefinition
    {
        Ogre::Vector3 p1;
        Ogre::Vector3 p2;
        float height;

        Ogre::ColourValue vc;
    };
    std::vector<GrassDefinition> grassBuffer;

    Ogre::Vector2 stepSize;
    std::string material;
    float stepDeviation = 1;
    float generalScale = 1;
    float minSteepY;
    float maxSteepY;
    float maxDistance;
};