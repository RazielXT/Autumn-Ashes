
#include "stdafx.h"
#include "OgrePrerequisites.h"
#include "OgreMovablePlane.h"
#include "OgreSharedPtr.h"


namespace Ogre
{

class DefaultShadowCameraSetup2 : public ShadowCameraSetup
{
public:
	/// Default constructor
	DefaultShadowCameraSetup2();
	/// Destructor
	virtual ~DefaultShadowCameraSetup2();

	/// Default shadow camera setup
	virtual void getShadowCamera (const SceneManager *sm, const Camera *cam,
	                              const Viewport *vp, const Light *light, Camera *texCam, size_t iteration) const;
};

}
