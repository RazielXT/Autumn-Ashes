#include "stdafx.h"
#include "SceneCubeMap.h"

using namespace Ogre;

int SceneCubeMap::id = 0;

SceneCubeMap::SceneCubeMap()
{
}

SceneCubeMap::~SceneCubeMap()
{
	texture.setNull();
	Global::mSceneMgr->destroyCamera(mReflectCam);
	//mat.setNull();
}

void SceneCubeMap::init()
{
	auto sceneMgr = Global::mSceneMgr;
	auto camera = sceneMgr->getCamera("Camera");
	auto window = Global::mWindow;

	auto idString = Ogre::StringConverter::toString(id++);

	int size = 64;

	auto sceneMgr = Global::mSceneMgr;
	auto camera = sceneMgr->getCamera("Camera");
	auto window = Global::mWindow;

	texture = TextureManager::getSingleton().createManual("GenCube" + idString,
		ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, TEX_TYPE_CUBE_MAP,
		size, size, 0, PF_R8G8B8A8, TU_RENDERTARGET);

	mReflectCam = sceneMgr->createCamera("waterDepthCam");
	mReflectCam->setNearClipDistance(4);
	mReflectCam->setFarClipDistance(camera->getFarClipDistance());
	mReflectCam->setFOVy(Ogre::Radian(Ogre::Math::PI / 2));
	mReflectCam->setAspectRatio(1);
	mReflectCam->lookAt(0, 0, -1);
	//mReflectCam->setVisibilityFlags(2);

	mReflectCamNode = Global::mSceneMgr->getRootSceneNode()->createChildSceneNode();
	mReflectCamNode->attachObject(mReflectCam);
	//mReflectCamNode->pitch(Degree(-90));

	for (int i = 0; i < 6; i++)
	{
		Ogre::RenderTarget *renderTarget = texture->getBuffer(i)->getRenderTarget();

		Viewport *v = renderTarget->addViewport(mReflectCam);
		v->setClearEveryFrame(true);
		v->setBackgroundColour(ColourValue::Black);
		v->setShadowsEnabled(false);

		renderTarget->setAutoUpdated(false);

		//renderTarget->addListener(this);
	}
}

bool SceneCubeMap::update()
{
	auto camera = Global::mSceneMgr->getCamera("Camera");
	mReflectCam->setPosition(camera->getDerivedPosition());

	auto or = camera->getDerivedOrientation();
	mReflectCam->setOrientation(or);

	return 1;
}
