#include "stdafx.h"
#include "PlayerSliding.h"
#include "Player.h"

PlayerSliding::PlayerSliding(Player* player) : p(player)
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

	slidesAutoTarget = new SlidesAutoTargetAsync();
}

PlayerSliding::~PlayerSliding()
{
	delete slidesAutoTarget;
}

void PlayerSliding::slideStarted(Slide* slide)
{
	currentSlide = slide;
}

void PlayerSliding::update(float tslf, bool readyToSlide)
{
	if (currentSlide)
	{
		if (!currentSlide->update(tslf))
			currentSlide = nullptr;
	}

	//todo allow autoslide

	slidesAutoTarget->getAutoTarget(p->mCamera->getDerivedPosition(), p->getFacingDirection(), tslf, 20.0f, currentSlide);
}

void PlayerSliding::pressedKey(const OIS::KeyEvent &arg)
{
	switch (arg.key)
	{
	case OIS::KC_LSHIFT:
		break;
	}
}

void PlayerSliding::releasedKey(const OIS::KeyEvent &arg)
{
	switch (arg.key)
	{

	}
}

void movedMouse(const OIS::MouseEvent &e)
{

}

void PlayerSliding::hideSlideTarget()
{
	targetBillboardSet->setVisible(false);
}

bool PlayerSliding::showPossibleSlideTarget()
{
	auto& target = slidesAutoTarget->targetInfo;

	if (target.targetSlide && target.targetSlidePosOffset >= 0)
	{
		targetBillboardSet->setVisible(true);
		billboardNode->setPosition(target.targetSlidePos);

		return true;
	}

	hideSlideTarget();
	return false;
}
