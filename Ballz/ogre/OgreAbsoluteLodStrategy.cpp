#include "stdafx.h"
#include "OgreAbsoluteLodStrategy.h"

#include "OgreViewport.h"

#include <limits>

namespace Ogre
{
//-----------------------------------------------------------------------
PixelCountLodStrategy2::PixelCountLodStrategy2(const String& name)
	: LodStrategy(name)
{ }
//---------------------------------------------------------------------
Real PixelCountLodStrategy2::getBaseValue() const
{
	// Use the maximum possible value as base
	return std::numeric_limits<Real>::max();
}
//---------------------------------------------------------------------
Real PixelCountLodStrategy2::transformBias(Real factor) const
{
	// No transformation required for pixel count strategy
	return factor;
}
//---------------------------------------------------------------------
ushort PixelCountLodStrategy2::getIndex(Real value, const Mesh::MeshLodUsageList& meshLodUsageList) const
{
	// Values are descending
	return getIndexDescending(value, meshLodUsageList);
}
//---------------------------------------------------------------------
ushort PixelCountLodStrategy2::getIndex(Real value, const Material::LodValueList& materialLodValueList) const
{
	// Values are descending
	return getIndexDescending(value, materialLodValueList);
}
//---------------------------------------------------------------------
void PixelCountLodStrategy2::sort(Mesh::MeshLodUsageList& meshLodUsageList) const
{
	// Sort descending
	sortDescending(meshLodUsageList);
}
//---------------------------------------------------------------------
bool PixelCountLodStrategy2::isSorted(const Mesh::LodValueList& values) const
{
	// Check if values are sorted descending
	return isSortedDescending(values);
}

/************************************************************************/
/*  AbsolutPixelCountLodStrategy                                        */
/************************************************************************/

//-----------------------------------------------------------------------
template<> AbsolutePixelCountLodStrategy* Singleton<AbsolutePixelCountLodStrategy>::msSingleton = 0;
AbsolutePixelCountLodStrategy* AbsolutePixelCountLodStrategy::getSingletonPtr(void)
{
	return msSingleton;
}
AbsolutePixelCountLodStrategy& AbsolutePixelCountLodStrategy::getSingleton(void)
{
	assert( msSingleton );
	return ( *msSingleton );
}
//-----------------------------------------------------------------------
AbsolutePixelCountLodStrategy::AbsolutePixelCountLodStrategy()
	: PixelCountLodStrategy2("pixel_count")
{ }
//-----------------------------------------------------------------------
Real AbsolutePixelCountLodStrategy::getValueImpl(const MovableObject *movableObject, const Ogre::Camera *camera) const
{
	// Get viewport
	const Viewport *viewport = camera->getViewport();

	// Get viewport area
	Real viewportArea = static_cast<Real>(viewport->getActualWidth() * viewport->getActualHeight());

	// Get area of unprojected circle with object bounding radius
	Real boundingArea = Math::PI * Math::Sqr(movableObject->getBoundingRadius());

	// Base computation on projection type
	switch (camera->getProjectionType())
	{
	case PT_PERSPECTIVE:
	{
		// Get camera distance
		Real distanceSquared = movableObject->getParentNode()->getSquaredViewDepth(camera);

		// Check for 0 distance
		if (distanceSquared <= std::numeric_limits<Real>::epsilon())
			return getBaseValue();

		// Get projection matrix (this is done to avoid computation of tan(FOV / 2))
		const Matrix4& projectionMatrix = camera->getProjectionMatrix();

		// Estimate pixel count
		return (boundingArea * viewportArea * projectionMatrix[0][0] * projectionMatrix[1][1]) / distanceSquared;
	}
	case PT_ORTHOGRAPHIC:
	{
		// Compute orthographic area
		Real orthoArea = camera->getOrthoWindowHeight() * camera->getOrthoWindowWidth();

		// Check for 0 orthographic area
		if (orthoArea <= std::numeric_limits<Real>::epsilon())
			return getBaseValue();

		// Estimate pixel count
		return (boundingArea * viewportArea) / orthoArea;
	}
	default:
	{
		// This case is not covered for obvious reasons
		throw;
	}
	}
}
//-----------------------------------------------------------------------

/************************************************************************/
/* ScreenRatioPixelCountLodStrategy                                     */
/************************************************************************/

//-----------------------------------------------------------------------
template<> ScreenRatioPixelCountLodStrategy* Singleton<ScreenRatioPixelCountLodStrategy>::msSingleton = 0;
ScreenRatioPixelCountLodStrategy* ScreenRatioPixelCountLodStrategy::getSingletonPtr(void)
{
	return msSingleton;
}
ScreenRatioPixelCountLodStrategy& ScreenRatioPixelCountLodStrategy::getSingleton(void)
{
	assert( msSingleton );
	return ( *msSingleton );
}
//-----------------------------------------------------------------------
ScreenRatioPixelCountLodStrategy::ScreenRatioPixelCountLodStrategy()
	: PixelCountLodStrategy2("screen_ratio_pixel_count")
{ }
//-----------------------------------------------------------------------
Real ScreenRatioPixelCountLodStrategy::getValueImpl(const MovableObject *movableObject, const Ogre::Camera *camera) const
{
	// Get absolute pixel count
	Real absoluteValue = AbsolutePixelCountLodStrategy::getSingletonPtr()->getValueImpl(movableObject, camera);

	// Get viewport area
	const Viewport *viewport = camera->getViewport();
	Real viewportArea = static_cast<Real>(viewport->getActualWidth() * viewport->getActualHeight());

	// Return ratio of screen size to absolutely covered pixel count
	return absoluteValue / viewportArea;
}
//-----------------------------------------------------------------------

} // namespace
