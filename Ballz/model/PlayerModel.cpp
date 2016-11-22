#include "stdafx.h"
#include "PlayerModel.h"
#include "Player.h"
#include "MUtils.h"

PlayerModel::PlayerModel(Player* player, Ogre::SceneNode* node)
{
	p = player;

	model = Global::sceneMgr->createEntity("mario.mesh");
	model->setMaterialName("redConcrete");
	model->setCastShadows(true);

	modelNode = node->createChildSceneNode();
	modelNode->attachObject(model);
	modelNode->setScale(0.04f, 0.04f, 0.04f);
	modelNode->setPosition(0, -2, 0);

	animation.init(model);
	animation.fadeTo("idle", 0);
}

PlayerModel::~PlayerModel()
{

}

void PlayerModel::notifyEvent(AnimEvent evt)
{

}

void PlayerModel::update(float time)
{
	animation.update(time);

	static bool moving = false;

	if (p->moving != moving)
	{
		moving = p->moving;
		animation.fadeTo(moving ? "walk" : "idle", 0.3f);
	}

	auto dir = p->pCamera->getFacingDirection();
	dir.y = 0;

	modelNode->setOrientation(MUtils::quaternionFromDir(dir)*Ogre::Quaternion(Ogre::Radian(Ogre::Math::PI), Ogre::Vector3::UNIT_Y));
}

void PlayerModel::setTransformation(bool enabled)
{

}

void PlayerModel::setRotation(Ogre::Quaternion q, float time /*= 0*/)
{

}

