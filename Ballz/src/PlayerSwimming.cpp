#include "stdafx.h"
#include "PlayerSwimming.h"
#include "Player.h"

int size = 1;

PlayerSwimming::PlayerSwimming(Player* player) : p(player)
{
	initWaterDepthReading();

	bubbles = Global::mSceneMgr->createParticleSystem("WaterBubbles", "WaterBubbles");
	bubbles->setRenderQueueGroup(91);
	bubbles->setVisibilityFlags(8);
	
	bubblesNode = Global::mSceneMgr->getRootSceneNode()->createChildSceneNode();
	bubblesNode->attachObject(bubbles);
}

void PlayerSwimming::initWaterDepthReading()
{
	auto sceneMgr = Global::mSceneMgr;
	auto camera = sceneMgr->getCamera("Camera");
	auto window = Global::mWindow;

	texture = TextureManager::getSingleton().createManual("waterDepth",
		ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, TEX_TYPE_2D,
		size, size, 0, PF_FLOAT32_R, TU_RENDERTARGET);

	rttTex = texture->getBuffer()->getRenderTarget();

	mWaterCam = sceneMgr->createCamera("waterDepthCam");
	mWaterCam->setNearClipDistance(camera->getNearClipDistance());
	mWaterCam->setFarClipDistance(camera->getFarClipDistance());
	mWaterCam->setAspectRatio(1);

	mWaterCam->setProjectionType(PT_ORTHOGRAPHIC);
	mWaterCam->setOrthoWindow(size, size);
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

	v->setVisibilityMask(16);
	v->setSkiesEnabled(false);
}

void PlayerSwimming::readWaterDepth()
{
    size_t width = size;
    size_t height = size;
    Ogre::Image::Box imageBox;

    float depth;
    Ogre::PixelBox tempPb(size, size, 1, Ogre::PF_FLOAT32_R, &depth);

    texture->getBuffer()->blitToMemory(tempPb);

    float playerDepth = p->getCameraPosition().y - 0.2f;

    static bool inWater = false;

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
	Global::mPPMgr->ColouringShift = Ogre::Vector4(1.5f, 1.15f, 1.05f, 0);
}

void PlayerSwimming::leftWater()
{
	Global::mPPMgr->ColouringShift = Ogre::Vector4(1, 1, 1, 0);
}

void PlayerSwimming::update()
{
    readWaterDepth();
}
