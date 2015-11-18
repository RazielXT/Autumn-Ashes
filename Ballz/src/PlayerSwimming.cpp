#include "stdafx.h"
#include "PlayerSwimming.h"
#include "Player.h"
#include "WaterCurrent.h"
#include "GameStateManager.h"

PlayerSwimming::PlayerSwimming(Player* player) : p(player)
{
    initWaterDepthReading();

    currents = WaterCurrent::get();
    /*  bubbles = Global::mSceneMgr->createParticleSystem("WaterBubbles", "WaterBubbles");
      bubbles->setRenderQueueGroup(91);
      bubbles->setVisibilityFlags(8);

      bubblesNode = Global::mSceneMgr->getRootSceneNode()->createChildSceneNode();
      bubblesNode->attachObject(bubbles);*/
}

PlayerSwimming::~PlayerSwimming()
{
    currents->reset();
    Global::mSceneMgr->destroyCamera(mWaterCam);
}

void PlayerSwimming::initWaterDepthReading()
{
    auto sceneMgr = Global::mSceneMgr;
    auto camera = sceneMgr->getCamera("Camera");
    auto window = Global::mWindow;

    texture = TextureManager::getSingleton().createManual("waterDepth",
              ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, TEX_TYPE_2D,
              1, 1, 0, PF_FLOAT32_R, TU_RENDERTARGET);

    rttTex = texture->getBuffer()->getRenderTarget();

    mWaterCam = sceneMgr->createCamera("waterDepthCam");
    mWaterCam->setNearClipDistance(camera->getNearClipDistance());
    mWaterCam->setFarClipDistance(camera->getFarClipDistance());
    mWaterCam->setAspectRatio(1);

    mWaterCam->setProjectionType(PT_ORTHOGRAPHIC);
    mWaterCam->setOrthoWindow(1.0f, 1.0f);
    mWaterCam->lookAt(0, 0, -1);
    //mReflectCam->setVisibilityFlags(2);
    Viewport *v = rttTex->addViewport(mWaterCam);
    v->setClearEveryFrame(true);
    v->setBackgroundColour(ColourValue::Black);
    v->setShadowsEnabled(false);
    v->setMaterialScheme("WaterDepth");

    mWaterCamNode = Global::mSceneMgr->getRootSceneNode()->createChildSceneNode();
    mWaterCamNode->attachObject(mWaterCam);
    mWaterCamNode->pitch(Degree(-90));

    v->setVisibilityMask(VisibilityFlag_Water);
    v->setSkiesEnabled(false);
}

void PlayerSwimming::readWaterDepth()
{
    float depth;
    Ogre::PixelBox tempPb(1, 1, 1, Ogre::PF_FLOAT32_R, &depth);

    texture->getBuffer()->blitToMemory(tempPb);

    float playerDepth = p->getCameraPosition().y - 0.2f;

    if (depth == 0 || playerDepth > depth)
    {
        if (inWater)
        {
            leftWater();
            inWater = false;
        }
    }
    else
    {
        if (!inWater)
        {
            enteredWater();
            inWater = true;
        }
    }

    mWaterCamNode->setPosition(p->bodyPosition + Vector3(0, 20, 0));
}

void PlayerSwimming::enteredWater()
{
    Global::mPPMgr->vars.ColouringShift = Ogre::Vector4(1.5f, 1.15f, 1.05f, 0);
    Global::mSceneMgr->setFog(FOG_LINEAR, Ogre::ColourValue(0.4, 0.4, 0.5, 0.85f), 1, 5, 25);

    Global::player->body->setLinearDamping(0.5f);
    Global::player->gravity = Ogre::Vector3(0, 0.3f, 0) + currents->getCurrent(p->bodyPosition);
}

void PlayerSwimming::leftWater()
{
    auto lvlInfo = Global::gameMgr->getCurrentLvlInfo();

    Global::mPPMgr->vars.ColouringShift = lvlInfo->ColorShift;
    Global::mSceneMgr->setFog(FOG_LINEAR, lvlInfo->fogColor, 1, lvlInfo->fogStartDistance, lvlInfo->fogEndDistance);

    Global::player->body->setLinearDamping(0);
    Global::player->gravity = Ogre::Vector3(0, -9.0f, 0);
}

void PlayerSwimming::update(float tslf)
{
    readWaterDepth();

    if (!inWater)
    {
        outOfWaterTimer += tslf;
    }
    else
        outOfWaterTimer = 0;


    float outOfWaterEffect = std::min(outOfWaterTimer / 2.0f, 1.0f);
    Global::mPPMgr->vars.ppDistortionIgnore = outOfWaterEffect;
    Global::mPPMgr->vars.ColouringShift.w = 1 - outOfWaterEffect;
}
