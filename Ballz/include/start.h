#include "stdafx.h"
#include "mShC.h"

struct shadowListener: public Ogre::SceneManager::Listener
{
    // this is a callback we'll be using to set up our shadow camera
    void shadowTextureCasterPreViewProj(Ogre::Light *light, Ogre::Camera *cam, size_t)
    {
        // basically, here we do some forceful camera near/far clip attenuation
        // yeah.  simplistic, but it works nicely.  this is the function I was talking
        // about you ignoring above in the Mgr declaration.
        float range = light->getAttenuationRange();
        cam->setNearClipDistance(0.1);
        cam->setFarClipDistance(200);
        // we just use a small near clip so that the light doesn't "miss" anything
        // that can shadow stuff.  and the far clip is equal to the lights' range.
        // (thus, if the light only covers 15 units of objects, it can only
        // shadow 15 units - the rest of it should be attenuated away, and not rendered)
    }

    // these are pure virtual but we don't need them...  so just make them empty
    // otherwise we get "cannot declare of type Mgr due to missing abstract
    // functions" and so on
    void shadowTexturesUpdated(size_t) {}
    void shadowTextureReceiverPreViewProj(Ogre::Light*, Ogre::Frustum*) {}
    void preFindVisibleObjects(Ogre::SceneManager*, Ogre::SceneManager::IlluminationRenderStage, Ogre::Viewport*) {}
    void postFindVisibleObjects(Ogre::SceneManager*, Ogre::SceneManager::IlluminationRenderStage, Ogre::Viewport*) {}
} shadowCameraUpdater;


void Application::setupScene()
{
    Ogre::LogManager::getSingleton().createLog("Loading.log");
    Ogre::LogManager::getSingleton().createLog("RuntimeEvents.log");

    mSceneMgr = mRoot->createSceneManager(ST_GENERIC, "mSceneMgr");
    Camera *cam = mSceneMgr->createCamera("Camera");
    Viewport *vp = mWindow->addViewport(cam);
    cam->setNearClipDistance(0.4);
	float aspect = mWindow->getWidth() / (float)mWindow->getHeight();

    m_World = new OgreNewt::World();
    m_World->setWorldSize(Vector3(-3000,-3000,-3000),Vector3(3000,3000,3000));
    cam->setFOVy(Degree(70));
	cam->setAspectRatio(aspect);
    cam->setPosition(Vector3(0,0,0));
    cam->lookAt(Vector3(0,0,0));

    mSceneMgr->setAmbientLight(ColourValue(0.6, 0.6, 0.6));
    mSceneMgr->setShadowTechnique(SHADOWTYPE_TEXTURE_ADDITIVE_INTEGRATED);
    mSceneMgr->setShadowTextureCountPerLightType(Ogre::Light::LT_DIRECTIONAL, 4);
    mSceneMgr->setShadowTextureSettings(512,4, PF_FLOAT32_R);
    mSceneMgr->setShadowTextureSelfShadow(true);
    mSceneMgr->setShadowTextureCasterMaterial("PSSM/shadow_caster");
    mSceneMgr->setShadowFarDistance(450);

    //shadow camera setup
    PSSMShadowCameraSetup2* pssmSetup = new PSSMShadowCameraSetup2();
    pssmSetup->calculateSplitPoints(4,1,450);
    pssmSetup->setSplitPadding(2);
    Ogre::PSSMShadowCameraSetup::SplitPointList splitPointList = pssmSetup->getSplitPoints();
    splitPointList[0]=1;
    splitPointList[1]=15;
    splitPointList[2]=35;
    splitPointList[3]=140;
    splitPointList[4]=450;
    pssmSetup->setSplitPoints(splitPointList);
    mSceneMgr->setShadowCasterRenderBackFaces(false);
    mSceneMgr->setShadowCameraSetup(ShadowCameraSetupPtr(pssmSetup));

    //MaterialManager::getSingleton().setDefaultAnisotropy(4);
    MaterialManager::getSingleton().setDefaultTextureFiltering(TFO_TRILINEAR);
    /*

    MaterialPtr mat = MaterialManager::getSingleton().getByName("01m");
    mat = MaterialManager::getSingleton().getByName("cubem");
    //mat->getTechnique(0)->getPass(0)->getFragmentProgramParameters()->setNamedConstant("pssmSplitPoints", splitPoints);
    mat = MaterialManager::getSingleton().getByName("domm");
    mat->getTechnique(0)->getPass(1)->getFragmentProgramParameters()->setNamedConstant("pssmSplitPoints", splitPoints);


    //VSM
    /*
    mSceneMgr->setShadowTextureSelfShadow(true);
    mSceneMgr->setShadowTextureCasterMaterial("shadow_caster");
    mSceneMgr->setShadowTextureCount(1);
    mSceneMgr->setShadowTextureSize(512);
    mSceneMgr->setShadowTexturePixelFormat(Ogre::PF_FLOAT16_RGB);
    mSceneMgr->setShadowCasterRenderBackFaces(false);

    const unsigned numShadowRTTs = mSceneMgr->getShadowTextureCount();
    for (unsigned i = 0; i < numShadowRTTs; ++i)
    {
     Ogre::TexturePtr tex = mSceneMgr->getShadowTexture(i);
     Ogre::Viewport *vp = tex->getBuffer()->getRenderTarget()->getViewport(0);
     vp->setBackgroundColour(Ogre::ColourValue(1, 1, 1, 1));
     vp->setClearEveryFrame(true);
    }

    mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_TEXTURE_MODULATIVE_INTEGRATED);
    shadowListener* u=new  shadowListener();
    mSceneMgr->addListener(u);
    */



}