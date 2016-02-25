#pragma once

#include "stdafx.h"
#include "FilteredRaycast.h"

struct TerrainHeightQueryData
{
	float offset_maxY;
	float offset_minY;
	OgreNewt::Body* bodyTarget;
	OgreNewt::World* world;
};

using namespace Ogre;

namespace HeightFunction
{
inline float getTerrainHeight(const float x, const float z, float* terrain, void *userData )
{
	TerrainHeightQueryData* queryData = (TerrainHeightQueryData*) userData;
	Real d=-333;
	if (terrain)
	{
		terrain[0] = 0;
		terrain[1] = 1;
		terrain[2] = 0;
	}

	OgreNewt::BasicRaycast ray(queryData->world, Vector3(x, queryData->offset_maxY, z), Vector3(x, queryData->offset_minY, z),false);
	OgreNewt::BasicRaycast::BasicRaycastInfo& info = ray.getFirstHit();

	if (info.mBody && info.mCollisionID<10 && info.mNormal.y > 0.65f)
	{
		d = queryData->offset_maxY + (queryData->offset_minY-queryData->offset_maxY)*info.mDistance;

		if (terrain)
		{
			terrain[0] = info.mNormal.x;
			terrain[1] = info.mNormal.y;
			terrain[2] = info.mNormal.z;
		}
	}

	return d;
}

inline float getTerrainHeightFiltered(const float x, const float z, float* terrain, void *userData)
{
	TerrainHeightQueryData* queryData = (TerrainHeightQueryData*)userData;
	Real d = -333;
	if (terrain)
	{
		terrain[0] = 0;
		terrain[1] = 1;
		terrain[2] = 0;
	}

	FilteredRaycast ray(queryData->world, Vector3(x, queryData->offset_maxY, z), Vector3(x, queryData->offset_minY, z), false, queryData->bodyTarget);

	OgreNewt::BasicRaycast::BasicRaycastInfo& info = ray.getFirstHit();

	if (info.mBody && info.mCollisionID < 10 && info.mNormal.y > 0.65f)
	{
		d = queryData->offset_maxY + (queryData->offset_minY - queryData->offset_maxY)*info.mDistance;

		if (terrain)
		{
			terrain[0] = info.mNormal.x;
			terrain[1] = info.mNormal.y;
			terrain[2] = info.mNormal.z;
		}
	}

	return d;
}
}
