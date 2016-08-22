#pragma once

#include "stdafx.h"


namespace Ogre
{

/** \addtogroup Core
*  @{
*/
/** \addtogroup LOD
*  @{
*/
/** Abstract base class for level of detail strategy based on pixel count approximations from bounding sphere projection. */
class PixelCountLodStrategy2 : public LodStrategy
{
protected:
	/// @copydoc LodStrategy::getValueImpl
	virtual Real getValueImpl(const MovableObject *movableObject, const Camera *camera) const = 0;

public:
	/** Default constructor. */
	PixelCountLodStrategy2(const String& name);

	/// @copydoc LodStrategy::getBaseValue
	virtual Real getBaseValue() const;

	/// @copydoc LodStrategy::transformBias
	virtual Real transformBias(Real factor) const;

	/// @copydoc LodStrategy::getIndex
	virtual ushort getIndex(Real value, const Mesh::MeshLodUsageList& meshLodUsageList) const;

	/// @copydoc LodStrategy::getIndex
	virtual ushort getIndex(Real value, const Material::LodValueList& materialLodValueList) const;

	/// @copydoc LodStrategy::sort
	virtual void sort(Mesh::MeshLodUsageList& meshLodUsageList) const;

	/// @copydoc LodStrategy::isSorted
	virtual bool isSorted(const Mesh::LodValueList& values) const;
};
/** @} */
/** @} */

class AbsolutePixelCountLodStrategy : public PixelCountLodStrategy2, public Singleton<AbsolutePixelCountLodStrategy>
{
public:
	/** Default constructor. */
	AbsolutePixelCountLodStrategy();

	/// @copydoc LodStrategy::getValueImpl
	Real getValueImpl(const MovableObject *movableObject, const Camera *camera) const;

	/** Override standard Singleton retrieval.
	@remarks
	Why do we do this? Well, it's because the Singleton
	implementation is in a .h file, which means it gets compiled
	into anybody who includes it. This is needed for the
	Singleton template to work, but we actually only want it
	compiled into the implementation of the class based on the
	Singleton, not all of them. If we don't change this, we get
	link errors when trying to use the Singleton-based class from
	an outside dll.
	@par
	This method just delegates to the template version anyway,
	but the implementation stays in this single compilation unit,
	preventing link errors.
	*/
	static AbsolutePixelCountLodStrategy& getSingleton(void);
	/** Override standard Singleton retrieval.
	@remarks
	Why do we do this? Well, it's because the Singleton
	implementation is in a .h file, which means it gets compiled
	into anybody who includes it. This is needed for the
	Singleton template to work, but we actually only want it
	compiled into the implementation of the class based on the
	Singleton, not all of them. If we don't change this, we get
	link errors when trying to use the Singleton-based class from
	an outside dll.
	@par
	This method just delegates to the template version anyway,
	but the implementation stays in this single compilation unit,
	preventing link errors.
	*/
	static AbsolutePixelCountLodStrategy* getSingletonPtr(void);
};
/** @} */
/** @} */

class ScreenRatioPixelCountLodStrategy : public PixelCountLodStrategy2, public Singleton<ScreenRatioPixelCountLodStrategy>
{
public:
	/** Default constructor. */
	ScreenRatioPixelCountLodStrategy();

	/// @copydoc LodStrategy::getValueImpl
	Real getValueImpl(const MovableObject *movableObject, const Camera *camera) const;

	/** Override standard Singleton retrieval.
	@remarks
	Why do we do this? Well, it's because the Singleton
	implementation is in a .h file, which means it gets compiled
	into anybody who includes it. This is needed for the
	Singleton template to work, but we actually only want it
	compiled into the implementation of the class based on the
	Singleton, not all of them. If we don't change this, we get
	link errors when trying to use the Singleton-based class from
	an outside dll.
	@par
	This method just delegates to the template version anyway,
	but the implementation stays in this single compilation unit,
	preventing link errors.
	*/
	static ScreenRatioPixelCountLodStrategy& getSingleton(void);
	/** Override standard Singleton retrieval.
	@remarks
	Why do we do this? Well, it's because the Singleton
	implementation is in a .h file, which means it gets compiled
	into anybody who includes it. This is needed for the
	Singleton template to work, but we actually only want it
	compiled into the implementation of the class based on the
	Singleton, not all of them. If we don't change this, we get
	link errors when trying to use the Singleton-based class from
	an outside dll.
	@par
	This method just delegates to the template version anyway,
	but the implementation stays in this single compilation unit,
	preventing link errors.
	*/
	static ScreenRatioPixelCountLodStrategy* getSingletonPtr(void);
};
/** @} */
/** @} */

} // namespace
