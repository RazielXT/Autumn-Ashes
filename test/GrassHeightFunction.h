#ifndef GRASSHF
#define GRASSHF

#include "stdafx.h"

struct TerrainHeightQueryData
{
	float normal[3];

    float offset_maxY;
    float offset_minY;
    OgreNewt::Body* bodyTarget;
    OgreNewt::World* world;
};

using namespace Ogre;

namespace HeightFunction
{
inline float getTerrainHeight(const float x, const float z, void *userData )
{
    TerrainHeightQueryData* queryData = (TerrainHeightQueryData*) userData;
    Real d=-333;
	queryData->normal[0] = 0;
	queryData->normal[1] = 1;
	queryData->normal[2] = 0;

    OgreNewt::BasicRaycast ray(queryData->world, Vector3(x, queryData->offset_maxY, z), Vector3(x, queryData->offset_minY, z),false);
    OgreNewt::BasicRaycast::BasicRaycastInfo& info = ray.getFirstHit();

    if (info.mBody && info.mCollisionID<10)
    {
        d = queryData->offset_maxY + (queryData->offset_minY-queryData->offset_maxY)*info.mDistance;

		auto normal = info.getNormal();
		queryData->normal[0] = normal.x;
		queryData->normal[1] = normal.y;
		queryData->normal[2] = normal.z;
    }

    return d;
}

inline float getTerrainHeightFiltered(const float x, const float z, void *userData)
{
    TerrainHeightQueryData* queryData = (TerrainHeightQueryData*)userData;
    Real d = -333;
	queryData->normal[0] = 0;
	queryData->normal[1] = 1;
	queryData->normal[2] = 0;

    OgreNewt::BasicRaycast ray(queryData->world, Vector3(x, queryData->offset_maxY, z), Vector3(x, queryData->offset_minY, z), true);

    for (int i = 0; i < ray.getHitCount(); i++)
    {
        OgreNewt::BasicRaycast::BasicRaycastInfo& info = ray.getInfoAt(i);

        if (info.mBody == queryData->bodyTarget)
        {
            d = queryData->offset_maxY + (queryData->offset_minY - queryData->offset_maxY)*info.mDistance;

			auto normal = info.getNormal();
			queryData->normal[0] = normal.x;
			queryData->normal[1] = normal.y;
			queryData->normal[2] = normal.z;

            break;
        }
    }

    return d;
}
}

#endif