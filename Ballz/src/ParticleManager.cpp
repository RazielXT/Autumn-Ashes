#include "stdafx.h"
#include "ParticleManager.h"
#include "Player.h"

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
    Ogre::ParticleSystem* ps = nullptr;

    if (!particles.empty())
    {
        float closestDist = 99999;
        auto pos = Global::player->getCameraPosition();

        for (auto p : particles)
        {
            float cdist = p->getParentSceneNode()->getPosition().squaredDistance(pos);
            if (cdist < closestDist)
            {
                closestDist = cdist;
                ps = p;
            }
        }
    }

    return ps;
}

