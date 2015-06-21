#pragma once

#include "stdafx.h"
#include "BasicGeometryPreset.h"

class RockGeometryPreset : public BasicGeometryPreset
{
public:

    virtual ~RockGeometryPreset() {}

protected:

    virtual bool acceptsWeight(float w) const;
    virtual void placeObject(Ogre::Vector3 pos, Ogre::Quaternion or, float scale, Ogre::Vector3 color);
    virtual void init(GeometryPresetInfo& info);

};