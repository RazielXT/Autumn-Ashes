#include "stdafx.h"
#include "PlayerSliding.h"
#include "Player.h"
#include "Slide.h"

using namespace Ogre;

PlayerSliding::PlayerSliding(Player* p) : player(p)
{
	targetBillboardSet = Global::mSceneMgr->createBillboardSet("autoTargetBillSet");
	targetBillboardSet->setMaterialName("chimneySmoke");
	auto targetBillboard = targetBillboardSet->createBillboard(Vector3(0, 0, 0));
	targetBillboard->setDimensions(5, 5);

	billboardNode = Global::mSceneMgr->getRootSceneNode()->createChildSceneNode();
	billboardNode->attachObject(targetBillboardSet);

	targetBillboardSet->setVisible(false);
	targetBillboardSet->setCastShadows(false);
	targetBillboardSet->setRenderQueueGroup(RENDER_QUEUE_OVERLAY);
}

PlayerSliding::~PlayerSliding()
{
}

void PlayerSliding::setSlide(Slide* slide)
{
	if (currentSlide)
		currentSlide->instantDetach(false);

	currentSlide = slide;
}

void PlayerSliding::slidingStarted()
{
	player->pAudio.slide(true);
}

void PlayerSliding::slidingEnd()
{
	player->pAudio.slide(false);
}

void PlayerSliding::update(float tslf, bool readyToSlide)
{
	autoAttachUnavailableTimer -= tslf;

	if (currentSlide)
	{
		if (!currentSlide->update(tslf))
		{
			currentSlide = nullptr;
			autoAttachUnavailableTimer = 0.5f;
		}
	}

	//todo allow autoslide
	bool allowAutoAttach = readyToSlide && autoAttachUnavailableTimer <= 0;

	player->autoTarget->setSlidingInfo(false, currentSlide);
}

void PlayerSliding::pressedKey(const OIS::KeyEvent &arg)
{
	if (currentSlide)
		currentSlide->pressedKey(arg);
}

void PlayerSliding::releasedKey(const OIS::KeyEvent &arg)
{
	if (currentSlide)
		currentSlide->releasedKey(arg);
}

void PlayerSliding::movedMouse(const OIS::MouseEvent &e)
{
	if (currentSlide)
		currentSlide->movedMouse(e);
}

void PlayerSliding::hideSlideTarget()
{
	targetBillboardSet->setVisible(false);
}

bool PlayerSliding::showPossibleSlideTarget()
{
	auto& target = player->autoTarget->targetInfo;

	if (target.targetSlide)
	{
		targetBillboardSet->setVisible(true);
		//billboardNode->setPosition(target.targetSlidePos);

		return true;
	}

	hideSlideTarget();
	return false;
}

bool PlayerSliding::foundTarget()
{
	auto& target = player->autoTarget->targetInfo;

	return (target.targetSlide != nullptr);
}

void PlayerSliding::portToTarget()
{
	auto& targetInfo = player->autoTarget->targetInfo;

	if (targetInfo.targetSlide)
		targetInfo.targetSlide->start(targetInfo.targetSlideOffset, false, 0.1f);
}
