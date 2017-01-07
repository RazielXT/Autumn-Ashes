#pragma once

#include "stdafx.h"
#include <vector>

class ParticleManager
{
public:

	void clear();
	Ogre::ParticleSystem* getClosestParticle();
	std::vector<Ogre::ParticleSystem*> getAllParticles();

	void addParticle(Ogre::ParticleSystem* ps);

private:

	std::vector<Ogre::ParticleSystem *> particles;

};

