#include "stdafx.h"
#include "ParticleManager.h"

void ParticleManager::clear()
{
    particles.clear();
}

void ParticleManager::addParticle(Ogre::ParticleSystem* ps)
{
    particles.push_back(ps);
}

Ogre::ParticleSystem* ParticleManager::getClosestParticle()
{
    if (particles.empty())
        return nullptr;
    else
        return particles[0];
}

