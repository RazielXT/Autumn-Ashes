#pragma once

#include "MovingAnimation.h"
#include "OgreQuaternion.h"
#include "AnimationBlender.h"

class Player;

enum class AnimEvent
{
	Jump,
	Float,
	Swim,
	Slide,
	Wallrun,
	Fly
};

class PlayerModel
{
public:

	PlayerModel(Player* player, Ogre::SceneNode* node);
	~PlayerModel();

	void notifyEvent(AnimEvent evt);

	void update(float time);

	void setTransformation(bool enabled);

	void setRotation(Ogre::Quaternion q, float time = 0);

private:

	AnimationBlender animation;
	MovingAnimation moving;

	Player* p;
	Ogre::Entity* model;
	Ogre::SceneNode* modelNode;
};