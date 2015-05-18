#include "stdafx.h"
#include "ReflectionTask.h"

using namespace Ogre;

int ReflectionTask::id = 0;

ReflectionTask::ReflectionTask()
{
}

ReflectionTask::~ReflectionTask()
{
    texture.setNull();
    Global::mSceneMgr->destroyCamera(mReflectCam);
    //mat.setNull();
}

void ReflectionTask::init(Ogre::Entity* water, int visibilityFlag)
{
    this->visibilityFlag = visibilityFlag;
    this->mEntity = water;
}

bool ReflectionTask::start()
{
    auto sceneMgr = Global::mSceneMgr;
    auto camera = sceneMgr->getCamera("Camera");
    auto window = Global::mWindow;

    auto idString = Ogre::StringConverter::toString(id++);

    MovablePlane *mPlane = new MovablePlane("ReflectPlane" + idString);
    mPlane->d = 0;
    mPlane->normal = Vector3::UNIT_Y;

    MeshManager::getSingleton().createPlane("ReflectionPlane" + idString,
                                            ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                                            *mPlane, 2000, 2000,
                                            1, 1, true, 1, 1, 1, Vector3::UNIT_Z);
    //Entity *mPlaneEnt = sceneMgr->createEntity("Plane" + idString, "valleyCone005.mesh", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    //mPlaneEnt->setVisible(false);

    texture = TextureManager::getSingleton().createManual("reflection" + idString,
              ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, TEX_TYPE_2D,
              window->getViewport(0)->getActualWidth(), window->getViewport(0)->getActualHeight(), 0, PF_R8G8B8, TU_RENDERTARGET);

    RenderTarget *rttTex = texture->getBuffer()->getRenderTarget();

    mReflectCam = sceneMgr->createCamera("ReflectCam" + idString);
    mReflectCam->setNearClipDistance(camera->getNearClipDistance());
    mReflectCam->setFarClipDistance(camera->getFarClipDistance());
    mReflectCam->setAspectRatio(
        (Real)window->getViewport(0)->getActualWidth() /
        (Real)window->getViewport(0)->getActualHeight());
    mReflectCam->setFOVy(Global::mSceneMgr->getCamera("Camera")->getFOVy());

    Viewport *v = rttTex->addViewport(mReflectCam);
    v->setClearEveryFrame(true);
    v->setBackgroundColour(ColourValue::Black);
    v->setShadowsEnabled(false);

    MaterialPtr mat = mEntity->getSubEntity(0)->getMaterial();
    TextureUnitState* t = mat->getTechnique(0)->getPass(0)->getTextureUnitState("reflectionMap");

    if (t == nullptr)
    {
        mat = MaterialManager::getSingleton().getByName("waterMatVC");
    }

    mat = mat->clone(mat->getName() + idString);
    t = mat->getTechnique(0)->getPass(0)->getTextureUnitState("reflectionMap");
    t->setTexture(texture);
    mEntity->setMaterial(mat);

    /*MaterialPtr mat = MaterialManager::getSingleton().getByName("waterMatVC");
    TextureUnitState* t = mat->getTechnique(0)->getPass(0)->getTextureUnitState(3);
    t->setTexture(texture);// = mat->getTechnique(0)->getPass(0)->createTextureUnitState("RttTex");
    */

    // set up linked reflection
    mReflectCam->enableReflection(mPlane);
    // Also clip
    mReflectCam->enableCustomNearClipPlane(mPlane);
    // mPlaneEnt->setMaterialName("waterMatVC");

    SceneNode* mPlaneNode = mEntity->getParentSceneNode();

    // mPlaneNode->attachObject(mPlaneEnt);
    mPlaneNode->attachObject(mPlane);
    // mPlaneNode->translate(0, -81, 0);

    //(*Global::globalData)[mEntity->getName() + "_reflection"] = this;

    return 1;
}

bool ReflectionTask::update(Ogre::Real tslf)
{
    auto camera = Global::mSceneMgr->getCamera("Camera");
    mReflectCam->setPosition(camera->getDerivedPosition());

    auto or = camera->getDerivedOrientation();
    mReflectCam->setOrientation(or);

    return 1;
}
