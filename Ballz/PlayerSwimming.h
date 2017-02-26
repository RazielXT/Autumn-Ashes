#pragma once

class Player;
class WaterCurrent;

class PlayerSwimming
{
	friend class Player;

	Player* player;

	Ogre::ParticleSystem* bubbles;
	Ogre::SceneNode* bubblesNode;

	Ogre::ParticleSystem* splash;
	Ogre::SceneNode* splashNode;

	Ogre::ParticleSystem* dust;
	Ogre::SceneNode* dustNode;

public:

	WaterCurrent* currents = nullptr;

	PlayerSwimming(Player* player);
	~PlayerSwimming();

	void enteredWater(Ogre::Vector3 position);
	void leftWater();
};