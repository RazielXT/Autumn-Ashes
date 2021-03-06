#pragma once

#include "stdafx.h"
#include "FilteredRaycast.h"

using namespace Ogre;

namespace GUtils
{

extern std::vector<std::string> debug;
extern void DebugPrint(const std::string line, bool logFile = false);

inline Ogre::Entity* TargetHeight(Ogre::Entity* entity, float target)
{
	auto node = entity->getParentSceneNode();
	auto cscale = node->getScale();
	auto current = entity->getBoundingBox().getSize().y*cscale.y;

	auto w = target / current;

	node->setScale(cscale*w);

	return entity;
}

inline Ogre::Entity* MakeEntity(const std::string name, Ogre::Vector3 pos = Ogre::Vector3::ZERO, Ogre::Vector3 scale = Ogre::Vector3::UNIT_SCALE)
{
	auto ent = Global::sceneMgr->createEntity(name);
	auto node = Global::sceneMgr->getRootSceneNode()->createChildSceneNode(pos);
	node->attachObject(ent);
	node->setScale(scale);

	return ent;
}

extern OgreNewt::Body* createConvexBody(Entity* ent, float mass = 0, float linearDamping = 0, float angularDamping = 0);

inline std::string v2s(Ogre::Vector3& v)
{
	return std::to_string(v.x) + " , " + std::to_string(v.y) + " , " + std::to_string(v.z);
}

inline bool isPathFree(Vector3 start, Vector3 end)
{
	OgreNewt::BasicRaycast ray(Global::nWorld, start, end, false);

	return (ray.getHitCount() == 0);
}

struct RayInfo
{
	float offset;
	Vector3 pos;
	Vector3 normal;
	OgreNewt::Body* body;
};

inline bool getRayFilteredInfo(Vector3 start, Vector3 end, RayInfo& minfo, OgreNewt::Body* target)
{
	FilteredRaycast ray(Global::nWorld, start, end, false, target);

	if(ray.getHitCount()>0)
	{
		OgreNewt::BasicRaycast::BasicRaycastInfo& info = ray.getFirstHit();

		minfo.normal = info.getNormal();
		minfo.pos = end* info.getDistance() + start*(1 - info.getDistance());
		minfo.body = info.getBody();
		minfo.offset = info.getDistance();

		return true;
	}

	return false;
}


inline bool getRayFilteredInfo(Vector3 start, Vector3 dir, float len, RayInfo& minfo, OgreNewt::Body* target)
{
	dir.normalise();
	Vector3 end = start + dir*len;

	return getRayFilteredInfo(start, end, minfo, target);
}

inline bool getRayInfo(Vector3 start, Vector3 end, RayInfo& minfo)
{
	OgreNewt::BasicRaycast ray(Global::nWorld, start, end, false);

	if (ray.getHitCount() > 0)
	{
		OgreNewt::BasicRaycast::BasicRaycastInfo& info = ray.getFirstHit();
		minfo.normal = info.getNormal();
		minfo.pos = end* info.getDistance() + start*(1 - info.getDistance());
		minfo.body = info.getBody();
		minfo.offset = info.getDistance();

		return true;
	}
	else
		return false;
}

inline bool getRayInfo(Vector3 start, Vector3 dir, float len, RayInfo& minfo)
{
	//dir.normalise();
	Vector3 end = start + dir*len;

	return getRayInfo(start, end, minfo);
}

inline bool getRayPortInfo(Vector3 start, Vector3 dir, float len, float offset, RayInfo& minfo)
{
	float offsetSub = offset / len;
	Vector3 end = start + dir*len;
	OgreNewt::BasicRaycast ray(Global::nWorld, start, end, false);

	if (ray.getHitCount() > 0)
	{
		OgreNewt::BasicRaycast::BasicRaycastInfo& info = ray.getFirstHit();
		minfo.normal = info.getNormal();
		minfo.offset = info.getDistance();
		minfo.pos = start + std::max(0.0f, minfo.offset - offsetSub)*len*dir;
		minfo.body = info.getBody();

		return true;
	}
	else
	{
		minfo.normal = Ogre::Vector3(0, 1, 0);
		minfo.pos = end;
		minfo.body = nullptr;
		minfo.offset = 1;

		return false;
	}
}

inline bool getVerticalRayPos(Vector3& pos, float yMaxOffset, float yMinOffset)
{
	OgreNewt::BasicRaycast ray(Global::nWorld, Vector3(pos.x, pos.y + yMaxOffset, pos.z), Vector3(pos.x, pos.y - yMinOffset, pos.z), false);

	if (ray.getHitCount() > 0)
	{
		OgreNewt::BasicRaycast::BasicRaycastInfo& info = ray.getFirstHit();
		pos.y += (yMaxOffset + yMinOffset)*(1 - info.getDistance()) - yMinOffset;

		return true;
	}
	else
		return false;
}

}