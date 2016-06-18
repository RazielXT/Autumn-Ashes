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

const float MaxSlope = 0.65f;

inline float getTerrainHeight(const float x, const float z, void *userData )
{
	TerrainHeightQueryData* queryData = (TerrainHeightQueryData*) userData;
	Real d=-333;

	OgreNewt::BasicRaycast ray(queryData->world, Vector3(x, queryData->offset_maxY, z), Vector3(x, queryData->offset_minY, z),false);
	OgreNewt::BasicRaycast::BasicRaycastInfo& info = ray.getFirstHit();

	if (info.mBody && info.mCollisionID<10 && info.mNormal.y > MaxSlope)
	{
		d = queryData->offset_maxY + (queryData->offset_minY-queryData->offset_maxY)*info.mDistance;
	}

	return d;
}

inline float getTerrainHeightFiltered(const float x, const float z, void *userData)
{
	TerrainHeightQueryData* queryData = (TerrainHeightQueryData*)userData;
	Real d = -333;

	FilteredRaycast ray(queryData->world, Vector3(x, queryData->offset_maxY, z), Vector3(x, queryData->offset_minY, z), false, queryData->bodyTarget);

	OgreNewt::BasicRaycast::BasicRaycastInfo& info = ray.getFirstHit();

	if (info.mBody && info.mCollisionID < 10 && info.mNormal.y > MaxSlope)
	{
		d = queryData->offset_maxY + (queryData->offset_minY - queryData->offset_maxY)*info.mDistance;
	}

	return d;
}
}
