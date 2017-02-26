#include "stdafx.h"
#include "PlayerModel.h"
#include "Player.h"
#include "MUtils.h"
#include "GUtils.h"

PlayerModel::PlayerModel(Player* player, Ogre::SceneNode* node)
{
	p = player;

	model = Global::sceneMgr->createEntity("mario.mesh");//mario
	model->setMaterialName("flame_body");
	model->setCastShadows(true);

	modelNode = node->createChildSceneNode();
	modelNode->attachObject(model);
	modelNode->setScale(0.04f, 0.04f, 0.04f);
	modelNode->setPosition(0, 1, 0);

	auto model2 = Global::sceneMgr->createEntity("limb.mesh");
	model2->setMaterialName("flame_body");
	model2->setCastShadows(true);

	auto modelNode2 = modelNode->createChildSceneNode();
	modelNode2->attachObject(model2);
	modelNode2->setPosition(150, 1, 0);
	modelNode2->setScale(0.5f, 0.5f, 0.5f);

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
	//return;

	static bool moving = false;

	if (p->moving != moving)
	{
		moving = p->moving;
		animation.fadeTo(moving ? "walk" : "idle", 0.3f);
	}

	if(moving)
		animation.update(time*std::min(1.f,p->bodyVelocityL/5));
	else
		animation.update(time);

	auto dir = p->pCamera->getFacingDirection();
	dir.y = 0;

	const float rotSpeed = 500;

	auto destination = MUtils::quaternionFromDir(dir)*Ogre::Quaternion(Ogre::Radian(Ogre::Math::PI), Ogre::Vector3::UNIT_Y);
	auto current = modelNode->getOrientation();

	Ogre::Vector3 lastDir = current*Ogre::Vector3(0, 0, -1);

	auto angle = lastDir.angleBetween(dir).valueDegrees();
	auto w = std::min(1.f, (time*rotSpeed)/angle);

	GUtils::DebugPrint(std::to_string(w) + "," + std::to_string(angle)+ "," +std::to_string(time*rotSpeed), true);

	modelNode->setOrientation(Quaternion::nlerp(w, current, destination));
}

void PlayerModel::setTransformation(bool enabled)
{

}

void PlayerModel::setRotation(Ogre::Quaternion q, float time /*= 0*/)
{

}

