
#include "stdafx.h"
#include "OgreStableHeaders.h"
#include "PSSMCamera.h"
#include "OgreSceneManager.h"
#include "OgreCamera.h"

namespace Ogre
{
//---------------------------------------------------------------------
PSSMShadowCameraSetup2::PSSMShadowCameraSetup2()
	: mSplitPadding(1.0f)
{
	calculateSplitPoints(3, 100, 100000);
	setOptimalAdjustFactor(0, 5);
	setOptimalAdjustFactor(1, 1);
	setOptimalAdjustFactor(2, 0);

	mat = Ogre::MaterialManager::getSingleton().getByName( "branch_summerpng" );

}
//---------------------------------------------------------------------
PSSMShadowCameraSetup2::~PSSMShadowCameraSetup2()
{
}
//---------------------------------------------------------------------
void PSSMShadowCameraSetup2::calculateSplitPoints(size_t splitCount, Real nearDist, Real farDist, Real lambda)
{
	if (splitCount < 2)
		OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, "Cannot specify less than 2 splits",
		            "PSSMShadowCameraSetup::calculateSplitPoints");

	mSplitPoints.resize(splitCount + 1);
	mOptimalAdjustFactors.resize(splitCount);
	mSplitCount = splitCount;

	mSplitPoints[0] = nearDist;
	for (size_t i = 1; i < mSplitCount; i++)
	{
		Real fraction = (Real)i / (Real)mSplitCount;
		Real splitPoint = lambda * nearDist * Math::Pow(farDist / nearDist, fraction) +
		                  (1.0f - lambda) * (nearDist + fraction * (farDist - nearDist));

		mSplitPoints[i] = splitPoint;
	}
	mSplitPoints[splitCount] = farDist;

}
//---------------------------------------------------------------------
void PSSMShadowCameraSetup2::setSplitPoints(const SplitPointList& newSplitPoints)
{
	if (newSplitPoints.size() < 3) // 3, not 2 since splits + 1 points
		OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, "Cannot specify less than 2 splits",
		            "PSSMShadowCameraSetup::setSplitPoints");
	mSplitCount = newSplitPoints.size() - 1;
	mSplitPoints = newSplitPoints;
	mOptimalAdjustFactors.resize(mSplitCount);
}
//---------------------------------------------------------------------
void PSSMShadowCameraSetup2::setOptimalAdjustFactor(size_t splitIndex, Real factor)
{
	if (splitIndex >= mOptimalAdjustFactors.size())
		OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, "Split index out of range",
		            "PSSMShadowCameraSetup::setOptimalAdjustFactor");
	mOptimalAdjustFactors[splitIndex] = factor;

}
//---------------------------------------------------------------------
Real PSSMShadowCameraSetup2::getOptimalAdjustFactor() const
{
	// simplifies the overriding of the LiSPSM opt adjust factor use
	return mOptimalAdjustFactors[mCurrentIteration];
}
//---------------------------------------------------------------------
void PSSMShadowCameraSetup2::getShadowCamera(const Ogre::SceneManager *sm, const Ogre::Camera *cam,
        const Ogre::Viewport *vp, const Ogre::Light *light, Ogre::Camera *texCam, size_t iteration) const
{
	// apply the right clip distance.
	Real nearDist = mSplitPoints[iteration];
	Real farDist = mSplitPoints[iteration + 1];

	// Add a padding factor to internal distances so that the connecting split point will not have bad artifacts.
	if (iteration > 0)
	{
		nearDist -= mSplitPadding;
	}
	if (iteration < mSplitCount - 1)
	{
		farDist += mSplitPadding;
	}

	mCurrentIteration = iteration;

	// Ouch, I know this is hacky, but it's the easiest way to re-use LiSPSM / Focussed
	// functionality right now without major changes
	Camera* _cam = const_cast<Camera*>(cam);
	Real oldNear = _cam->getNearClipDistance();
	Real oldFar = _cam->getFarClipDistance();
	_cam->setNearClipDistance(nearDist);
	_cam->setFarClipDistance(farDist);


	DefaultShadowCameraSetup2::getShadowCamera(sm, cam, vp, light, texCam, iteration);

	//Ogre::MaterialPtr mat = Ogre::MaterialManager::getSingleton().getByName( "branch_summerpng" );
	//mat->getTechnique(0)->getPass(0)->getShadowCasterVertexProgramParameters()->setNamedConstant( "animated", Real(1) );

	/*if(farDist>400)
		mat->getTechnique(0)->getPass(0)->getShadowCasterVertexProgramParameters()->setNamedConstant( "animated", 0.0f );
	else
		mat->getTechnique(0)->getPass(0)->getShadowCasterVertexProgramParameters()->setNamedConstant( "animated", 1.0f );
	*/

	// restore near/far
	_cam->setNearClipDistance(oldNear);
	_cam->setFarClipDistance(oldFar);


}
}