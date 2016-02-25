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
	void init(std::string name, int size, bool editable, float minRenderDistance);

	Ogre::Vector3 position;
	float posessionRadius;
	float materialWPOffset;

	static void renderAll();

	struct CubemapedMats
	{
		std::string matOriginalName;
		SceneCubeMap* cm;
		std::string matName;
	};

	static std::vector<CubemapedMats> appliedMaterials;

	static Ogre::MaterialPtr applyCubemap(Ogre::MaterialPtr mat, Ogre::Vector3 pos);

	static void clearAll();

	static std::map<std::string, SceneCubeMap*> cubeMaps;

private:

	std::string getTexturePath(bool edited = false);
	std::string getTextureNamePrefix();

	void loadGpuTexture();

	std::string name;

	Ogre::TexturePtr texture;

	Ogre::SceneNode* mReflectCamNode;
	Ogre::Camera* mReflectCam;

	Ogre::Entity* mEntity;
	int visibilityFlag;
	int size;
	bool editable;
	bool detectedEdited;
	float minRenderDistance;

	static int id;
	static int matId;
};