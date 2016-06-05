#include "stdafx.h"
#include "SceneCubeMap.h"
#include "GameStateManager.h"
#include "PlayerCamera.h"

using namespace Ogre;

int SceneCubeMap::id = 0;
int SceneCubeMap::matId = 0;
std::map<std::string, SceneCubeMap*> SceneCubeMap::cubeMaps;
std::vector<SceneCubeMap::CubemapedMats> SceneCubeMap::appliedMaterials;

SceneCubeMap::SceneCubeMap()
{
}

SceneCubeMap::~SceneCubeMap()
{
	appliedMaterials.clear();

	texture.setNull();
	Global::mSceneMgr->destroyCamera(mReflectCam);
	//mat.setNull();
}

void SceneCubeMap::renderAll()
{
	for (auto cubemap : cubeMaps)
	{
		cubemap.second->update();
	}

	//double reflection
	for (auto cubemap : cubeMaps)
	{
		cubemap.second->update();
	}

	exportEditsList();
}

void SceneCubeMap::exportEditsList()
{
	std::string list;

	for (auto cubemap : cubeMaps)
	{
		if (cubemap.second->editable)
		{
			list += cubemap.second->getTextureFullName() + "\t";
			list += cubemap.second->editFilter + "\t";
			list += std::to_string(cubemap.second->filterAngle) + "\t";
			list += std::to_string(cubemap.second->editGamma) + "\n";
		}
	}

	std::ofstream file(getTexturePath(true) + "cmList.txt");

	if(file.good())
		file << list;
}

Ogre::MaterialPtr SceneCubeMap::applyCubemap(Ogre::MaterialPtr mat, Ogre::Vector3 pos)
{
	SceneCubeMap* r = nullptr;
	float closestToCenter;
	float closestToRadius;

	for (auto cubemap : cubeMaps)
	{
		auto cm = cubemap.second;

		float distToCenter = cm->position.distance(pos);
		float distToRadius = std::max(0.0f, distToCenter - cm->posessionRadius);

		if (!r || (distToRadius < closestToRadius || (distToRadius == 0 && distToCenter < closestToCenter)))
		{
			r = cm;
			closestToRadius = distToRadius;
			closestToCenter = distToCenter;
		}
	}

	if (!r)
		return mat;


	for (auto appliedMat : appliedMaterials)
	{
		if (appliedMat.matOriginalName == mat->getName() && appliedMat.cm == r)
			return Ogre::MaterialManager::getSingletonPtr()->getByName(appliedMat.matName);
	}

	//create new
	auto newMat = mat->clone(mat->getName() + "_CM" + std::to_string(matId++));

	auto pass = newMat->getTechnique(0)->getPass(1);
	Ogre::TextureUnitState* t = pass->getTextureUnitState("envCubeMap");
	if (r->detectedEdited)
		t->setTextureName(r->getTextureFullName() + ".dds", Ogre::TEX_TYPE_CUBE_MAP);
	else
		t->setTexture(r->texture);

	pass->getFragmentProgramParameters()->setNamedConstant("cubemapWPOffset", Ogre::Vector4(r->position.x, r->position.y, r->position.z, r->materialWPOffset));

	CubemapedMats matInfo;
	matInfo.matName = newMat->getName();
	matInfo.cm = r;
	matInfo.matOriginalName = mat->getName();
	appliedMaterials.push_back(matInfo);

	return newMat;
}

void SceneCubeMap::clearAll()
{
	for (auto cubemap : cubeMaps)
		delete cubemap.second;

	cubeMaps.clear();

	id = 0;
	matId = 0;
}

void SceneCubeMap::init(std::string name, int size, float minRenderDistance)
{
	this->size = size;
	this->name = name;
	this->minRenderDistance = minRenderDistance;
	cubeMaps[name] = this;

	auto sceneMgr = Global::mSceneMgr;
	auto camera = Global::camera->camera;
	auto window = Global::mWindow;

	auto idString = Ogre::StringConverter::toString(id++);
	mReflectCam = sceneMgr->createCamera("GenCubeCam" + idString);
	mReflectCam->setNearClipDistance(minRenderDistance);
	mReflectCam->setFarClipDistance(camera->getFarClipDistance());
	mReflectCam->setFOVy(Ogre::Radian(Ogre::Math::PI / 2));
	mReflectCam->setAspectRatio(1);
	mReflectCam->lookAt(0, 0, -1);

	unsigned int iFlag = VisibilityFlag_NeedCompositor;
	mReflectCam->setVisibilityFlags(VisibilityFlag_Normal);
	//mReflectCam->setVisibilityFlags(2);

	mReflectCamNode = Global::mSceneMgr->getRootSceneNode()->createChildSceneNode();
	mReflectCamNode->attachObject(mReflectCam);
	//mReflectCamNode->pitch(Degree(-90));

	loadGpuTexture();
}

void SceneCubeMap::setAsEditable(std::string editFilter_, float filterAngle_, float gamma)
{
	editable = true;
	editFilter = editFilter_;
	filterAngle = filterAngle_;
	editGamma = gamma;
}

std::string SceneCubeMap::getTexturePath(bool edited)
{
	auto path = Global::gameMgr->getCurrentLvlPath() + "cubemaps/";

	if (!edited)
		path += "generated/";

	return path;
}

std::string SceneCubeMap::getTextureFullName()
{
	return name + "_" + Global::gameMgr->getCurrentLvlInfo()->name;
}

void SceneCubeMap::loadGpuTexture()
{
	if (editable)
	{
		std::ifstream ifile(getTexturePath(true) + getTextureFullName() + ".dds");
		if (ifile)
		{
			detectedEdited = true;
			return;
		}
	}

	detectedEdited = false;

	auto idString = Ogre::StringConverter::toString(id);

	if (texture.isNull())
		texture = TextureManager::getSingleton().createManual("SceneCM" + idString,
		          ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, TEX_TYPE_CUBE_MAP,
		          size, size, 0, PF_R8G8B8A8, TU_RENDERTARGET);


	for (int i = 0; i < 6; i++)
	{
		Ogre::RenderTarget *renderTarget = texture->getBuffer(i)->getRenderTarget();
		renderTarget->setAutoUpdated(false);

		Viewport *v = renderTarget->addViewport(mReflectCam);
		v->setClearEveryFrame(true);
		v->setBackgroundColour(ColourValue::Black);
		v->setShadowsEnabled(true);
		v->setVisibilityMask(VisibilityFlag_Normal);
		//v->setMaterialScheme("CubemapRender");
	}
}

bool SceneCubeMap::update()
{
	if (detectedEdited)
		return false;

	mReflectCam->setPosition(position);

	if (editable)
	{
		CreateDirectory(getTexturePath(true).data(), NULL);
		CreateDirectory(getTexturePath(false).data(), NULL);
	}

	for (int i = 0; i < 6; i++)
	{
		mReflectCam->setOrientation(Ogre::Quaternion::IDENTITY);

		switch (i)
		{
		case 0:
			mReflectCam->yaw(Ogre::Radian(-Ogre::Math::PI / 2));
			break;
		case 1:
			mReflectCam->yaw(Ogre::Radian(Ogre::Math::PI / 2));
			break;
		case 2:
			mReflectCam->pitch(Ogre::Radian(Ogre::Math::PI / 2));
			break;
		case 3:
			mReflectCam->pitch(Ogre::Radian(-Ogre::Math::PI / 2));
			break;
		case 4:
			break;
		case 5:
			mReflectCam->yaw(Ogre::Radian(Ogre::Math::PI));
			break;
		}

		Ogre::RenderTarget *renderTarget = texture->getBuffer(i)->getRenderTarget();
		renderTarget->update();

		if (editable)
			renderTarget->writeContentsToFile(getTexturePath() + getTextureFullName() + "_" + std::to_string(i) + ".jpg");
	}


	return true;
}
