#pragma once

#include "stdafx.h"
#include "DetailGeometry.h"
#include "DetailGeometryInfo.h"
#include "DetailGeometryMaterial.h"

class BasicDetailGeometry : public DetailGeometry
{
public:

	virtual ~BasicDetailGeometry() {}

	virtual void addGeometry(MaskGrid& grid, GeometryMaskInfo& gridInfo, DetailGeometryParams& info);
	virtual void clear();

protected:

	virtual void init(DetailGeometryParams& info);

	virtual bool acceptsWeight(float w) const;
	void placeObject(Ogre::Vector3 pos, Ogre::Quaternion or, float scale, Ogre::Vector3 color);

	std::vector<Ogre::Entity*> temps;
	Ogre::StaticGeometry* sg = nullptr;
	DetailGeometryInfo info;
	DetailGeometryMaterial mats;
};