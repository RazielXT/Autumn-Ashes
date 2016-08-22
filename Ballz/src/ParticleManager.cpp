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
		float closestDist = 0;
		auto pos = Global::camera->getPosition();

		for (auto p : particles)
		{
			float cdist = p->getParentSceneNode()->getPosition().squaredDistance(pos);
			if (closestDist == 0 || cdist < closestDist)
			{
				closestDist = cdist;
				ps = p;
			}
		}
	}

	return ps;
}

