#pragma once

#include "stdafx.h"
#include "GeometryPreset.h"

class RockGeometryPreset : public GeometryPreset
{
public:

    virtual void addGeometry(MaskGrid& grid, GeometryMaskInfo& gridInfo, GeometryPresetInfo& info);

private:

    void placeRock(Ogre::Vector3 pos, Ogre::Quaternion or);
};