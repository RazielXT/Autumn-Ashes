#pragma once

#include "stdafx.h"
#include "OgreTexture.h"

#include "stdafx.h"

class SceneCubeMap
{
public:

	SceneCubeMap();
	~SceneCubeMap();

	bool update();
	void init();

private:

	Ogre::TexturePtr texture;

	Ogre::SceneNode* mReflectCamNode;
	Ogre::Camera* mReflectCam;

	Ogre::Entity* mEntity;
	int visibilityFlag;

	static int id;
};