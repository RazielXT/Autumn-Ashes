#pragma once

#include "stdafx.h"
#include <vector>

class ParticleManager
{
public:

	void clear();
	Ogre::ParticleSystem* getClosestParticle();

	void addParticle(Ogre::ParticleSystem* ps);

private:

	std::vector<Ogre::ParticleSystem *> particles;

};

