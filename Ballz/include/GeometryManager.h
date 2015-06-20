#pragma once

#include "stdafx.h"
#include <vector>
#include "GeometryPreset.h"


class GeometryManager
{
public:

    void addPagedGeometry(Forests::PagedGeometry *g);
    void clear();
    void update();

    void addDetailGeometry(Ogre::Entity* maskEnt, std::vector<GeometryPresetInfo>& geometries, OgreNewt::Body* targetarget, float rayDistance);

private:

    GeometryPreset* getPreset(std::string name);

    void generateGeometryMask(Ogre::Entity* maskEnt, MaskGrid& grid, Ogre::Vector2& size);

    std::vector<Forests::PagedGeometry *> pagedGeometries;
};

