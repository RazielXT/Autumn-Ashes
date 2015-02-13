#ifndef GRASSHF
#define GRASSHF

#include "stdafx.h"

struct TerrainHeightQueryData
{
    float offset_x;
    float offset_y;
    OgreNewt::World* world;
};

using namespace Ogre;

namespace HeightFunction
{
inline float getTerrainHeight(const float x, const float z, void *userData )
{
    TerrainHeightQueryData* offsets = (TerrainHeightQueryData*) userData;
    Real d=-333;

    OgreNewt::BasicRaycast ray(offsets->world, Vector3(x, offsets->offset_x, z), Vector3(x, offsets->offset_y, z), true);
    OgreNewt::BasicRaycast::BasicRaycastInfo info = ray.getInfoAt(0);

    if (info.mBody  && info.mBody->getType()<10)
    {
        d = offsets->offset_x + (offsets->offset_y-offsets->offset_x)*info.mDistance;
    }

    return d;
}
}

#endif