#include "stdafx.h"
#include "SceneCubeMap.h"
#include "GameStateManager.h"

using namespace Ogre;

int SceneCubeMap::id = 0;
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

void SceneCubeMap::init(std::string name, int size, bool editable, float minRenderDistance)
{
    this->size = size;
    this->editable = editable;
    this->name = name;
    this->minRenderDistance = minRenderDistance;
    cubeMaps[name] = this;

    auto sceneMgr = Global::mSceneMgr;
    auto camera = sceneMgr->getCamera("Camera");
    auto window = Global::mWindow;

    auto idString = Ogre::StringConverter::toString(id++);
    mReflectCam = sceneMgr->createCamera("GenCubeCam" + idString);
    mReflectCam->setNearClipDistance(minRenderDistance);
    mReflectCam->setFarClipDistance(camera->getFarClipDistance());
    mReflectCam->setFOVy(Ogre::Radian(Ogre::Math::PI / 2));
    mReflectCam->setAspectRatio(1);
    mReflectCam->lookAt(0, 0, -1);
    //mReflectCam->setVisibilityFlags(2);

    mReflectCamNode = Global::mSceneMgr->getRootSceneNode()->createChildSceneNode();
    mReflectCamNode->attachObject(mReflectCam);
    //mReflectCamNode->pitch(Degree(-90));

    loadGpuTexture();
}

std::string SceneCubeMap::getTexturePath(bool edited)
{
    String prefix = getTextureNamePrefix();

    if (edited)
        prefix = "..\\..\\media\\cubemaps\\precomputed\\" + prefix;
    else
        prefix = "..\\..\\media\\cubemaps\\" + prefix;

    return prefix;
}

std::string SceneCubeMap::getTextureNamePrefix()
{
    String prefix = Global::gameMgr->getCurrentLvlInfo()->name + "_" + name;
    return prefix;
}

void SceneCubeMap::loadGpuTexture()
{
    if (editable)
    {
        std::ifstream ifile(getTexturePath(true) + "_lf.png");
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
        //v->setMaterialScheme("CubemapRender");
    }
}

bool SceneCubeMap::update()
{
    if (detectedEdited)
        return false;

    mReflectCam->setPosition(position);

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
            renderTarget->writeContentsToFile(getTexturePath() + "_" + std::to_string(i) + ".jpg");
    }


    return true;
}
