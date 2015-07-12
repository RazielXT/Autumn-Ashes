#include "stdafx.h"
#include "ImpostorRenderer.h"

using namespace Ogre;

ImpostorTexture::ImpostorTextureResourceLoader::ImpostorTextureResourceLoader(ImpostorTexture& impostorTexture)
    : texture(impostorTexture)
{
}

void ImpostorTexture::ImpostorTextureResourceLoader::loadResource(Ogre::Resource *resource)
{
    if (resource->getLoadingState() == Ogre::Resource::LOADSTATE_UNLOADED)
    {
        texture.render();
    }
}

void ImpostorRenderEnvironment::PreRender()
{

}

#define IMPOSTOR_FILE_SAVE

void ImpostorTexture::render(Ogre::Entity* entity)
{
#ifdef IMPOSTOR_FILE_SAVE
    TexturePtr renderTexture;
#else
    TexturePtr renderTexture(texture);
    //if we're not using a file image we need to set up a resource loader, so that the texture is regenerated if it's ever unloaded (such as switching between fullscreen and the desktop in win32)
    loader = std::auto_ptr<ImpostorTextureResourceLoader>(new ImpostorTextureResourceLoader(*this));
#endif
    RenderTexture *renderTarget;
    Camera *renderCamera;
    Viewport *renderViewport;
    SceneNode *camNode;

    //Set up RTT texture
    Ogre::uint textureSize = 256;
    if (renderTexture.isNull())
    {
        static int id = 0;
        renderTexture = TextureManager::getSingleton().createManual(("ImpTex" + std::to_string(id)), "Impostors",
                        TEX_TYPE_2D, textureSize, textureSize, 0, PF_A8R8G8B8, TU_RENDERTARGET, loader.get());
    }
    renderTexture->setNumMipmaps(MIP_UNLIMITED);

    //Set up render target
    renderTarget = renderTexture->getBuffer()->getRenderTarget();
    renderTarget->setAutoUpdated(false);

    //Calculate the entity's bounding box and it's diameter
    auto boundingBox = entity->getBoundingBox();
    auto entityRadius = Math::boundingRadiusFromAABB(boundingBox);
    auto entityDiameter = 2.0f * entityRadius;
    auto entityCenter = boundingBox.getCenter();

    //Set up camera
    camNode = Global::mSceneMgr->getSceneNode("ImpostorPage::cameraNode");
    renderCamera = Global::mSceneMgr->createCamera("ImpostorCam");
    camNode->attachObject(renderCamera);
    renderCamera->setLodBias(1000.0f);
    renderViewport = renderTarget->addViewport(renderCamera);
    renderViewport->setOverlaysEnabled(false);
    renderViewport->setClearEveryFrame(true);
    renderViewport->setShadowsEnabled(false);
    renderViewport->setBackgroundColour(Ogre::ColourValue(1,1,1,0));

    //Set up scene node
    SceneNode* node = Global::mSceneMgr->getSceneNode("ImpostorPage::renderNode");

    Ogre::SceneNode* oldSceneNode = entity->getParentSceneNode();
    if (oldSceneNode)
    {
        oldSceneNode->detachObject(entity);
    }

    Ogre::SceneNode *n1 = node->createChildSceneNode();
    n1->attachObject(entity);
    n1->setPosition(Vector3(0, 0, 0));


    //Set up camera FOV
    const Real objDist = entityRadius * 100;
    const Real nearDist = objDist - (entityRadius + 1);
    const Real farDist = objDist + (entityRadius + 1);

    renderCamera->setAspectRatio(1.0f);
    renderCamera->setFOVy(Math::ATan(entityDiameter / objDist));
    renderCamera->setNearClipDistance(nearDist);
    renderCamera->setFarClipDistance(farDist);

    //Disable mipmapping (without this, masked textures look bad)
    MaterialManager *mm = MaterialManager::getSingletonPtr();
    FilterOptions oldMinFilter = mm->getDefaultTextureFiltering(FT_MIN);
    FilterOptions oldMagFilter = mm->getDefaultTextureFiltering(FT_MAG);
    FilterOptions oldMipFilter = mm->getDefaultTextureFiltering(FT_MIP);
    mm->setDefaultTextureFiltering(FO_POINT, FO_LINEAR, FO_NONE);

    //Disable fog
    FogMode oldFogMode = Global::mSceneMgr->getFogMode();
    ColourValue oldFogColor = Global::mSceneMgr->getFogColour();
    Real oldFogDensity = Global::mSceneMgr->getFogDensity();
    Real oldFogStart = Global::mSceneMgr->getFogStart();
    Real oldFogEnd = Global::mSceneMgr->getFogEnd();
    Global::mSceneMgr->setFog(FOG_NONE);

    // Get current status of the queue mode
    Ogre::SceneManager::SpecialCaseRenderQueueMode OldSpecialCaseRenderQueueMode = Global::mSceneMgr->getSpecialCaseRenderQueueMode();
    //Only render the entity
    Global::mSceneMgr->setSpecialCaseRenderQueueMode(Ogre::SceneManager::SCRQM_INCLUDE);
    Global::mSceneMgr->addSpecialCaseRenderQueue(51);

    uint8 oldRenderQueueGroup = entity->getRenderQueueGroup();
    entity->setRenderQueueGroup(51);
    bool oldVisible = entity->getVisible();
    entity->setVisible(true);
    Ogre::Real oldMaxDistance = entity->getRenderingDistance();
    entity->setRenderingDistance(0);

    bool needsRegen = true;
#ifdef IMPOSTOR_FILE_SAVE
    //Calculate the filename hash used to uniquely identity this render
    String strKey = entity->getName();
    char key[32] = { 0 };
    uint32 i = 0;
    for (String::const_iterator it = strKey.begin(); it != strKey.end(); ++it)
    {
        key[i] ^= *it;
        i = (i + 1) % sizeof(key);
    }
    for (i = 0; i < sizeof(key); ++i)
        key[i] = (key[i] % 26) + 'A';

    String tempdir = "";
    ResourceGroupManager::getSingleton().addResourceLocation(tempdir, "FileSystem", "BinFolder");

    String fileNamePNG = "Impostor." + String(key, sizeof(key)) + '.' + StringConverter::toString(textureSize) + ".png";
    String fileNameDDS = "Impostor." + String(key, sizeof(key)) + '.' + StringConverter::toString(textureSize) + ".dds";

    //Attempt to load the pre-render file if allowed
    needsRegen = true;
    if (!needsRegen)
    {
        try
        {
            texture = TextureManager::getSingleton().load(fileNameDDS, "BinFolder", TEX_TYPE_2D, MIP_UNLIMITED);
        }
        catch (...)
        {
            try
            {
                texture = TextureManager::getSingleton().load(fileNamePNG, "BinFolder", TEX_TYPE_2D, MIP_UNLIMITED);
            }
            catch (...)
            {
                needsRegen = true;
            }
        }
    }
#endif

    if (needsRegen)
    {

        //Position camera
        camNode->setPosition(0, 0, 10);
        //camNode->setOrientation(Quaternion(yaw, Vector3::UNIT_Y) * Quaternion(-pitch, Vector3::UNIT_X));

        //Render the impostor
        renderViewport->setDimensions(0,0,1,1);
        renderTarget->update();


#ifdef IMPOSTOR_FILE_SAVE
        //Save RTT to file with respecting the temp dir
        renderTarget->writeContentsToFile(tempdir + fileNamePNG);

        //Load the render into the appropriate texture view
        texture = TextureManager::getSingleton().load(fileNamePNG, "BinFolder", TEX_TYPE_2D, MIP_UNLIMITED);
#else
        texture = renderTexture;
#endif
    }


    entity->setVisible(oldVisible);
    entity->setRenderQueueGroup(oldRenderQueueGroup);
    entity->setRenderingDistance(oldMaxDistance);

    Global::mSceneMgr->removeSpecialCaseRenderQueue(51);
    // Restore original state
    Global::mSceneMgr->setSpecialCaseRenderQueueMode(OldSpecialCaseRenderQueueMode);

    //Re-enable mipmapping
    mm->setDefaultTextureFiltering(oldMinFilter, oldMagFilter, oldMipFilter);

    //Re-enable fog
    Global::mSceneMgr->setFog(oldFogMode, oldFogColor, oldFogDensity, oldFogStart, oldFogEnd);

    //Delete camera
    renderTarget->removeViewport(0);
    renderCamera->getSceneManager()->destroyCamera(renderCamera);

    //Delete scene node
    node->detachAllObjects();
    n1->detachAllObjects();
    node->removeAndDestroyAllChildren();
    if (oldSceneNode)
    {
        oldSceneNode->attachObject(entity);
    }

#ifdef IMPOSTOR_FILE_SAVE
    //Delete RTT texture
    assert(!renderTexture.isNull());
    String texName2(renderTexture->getName());

    renderTexture.setNull();
    if (TextureManager::getSingletonPtr())
        TextureManager::getSingleton().remove(texName2);
#endif
}