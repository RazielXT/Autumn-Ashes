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
    TerrainHeightQueryData* queryData = (TerrainHeightQueryData*) userData;
    Real d=-333;

    OgreNewt::BasicRaycast ray(queryData->world, Vector3(x, queryData->offset_x, z), Vector3(x, queryData->offset_y, z),false);
    OgreNewt::BasicRaycast::BasicRaycastInfo info = ray.getFirstHit();

   // if (info.mBody && info.mBody->getType()<10)
	if (info.mCollisionID<10)
    {
        d = queryData->offset_x + (queryData->offset_y-queryData->offset_x)*info.mDistance;
    }

    return d;
}
}

#endif