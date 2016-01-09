#include "stdafx.h"
#include "GateLock.h"
#include "Energy.h"
#include "GUtils.h"

std::map<std::string, std::vector<GateLock*>> GateLock::locks;

GateLock* GateLock::createLock(Ogre::SceneNode* node, std::string type, std::string id, int timeout)
{
    GateLock* lock = new GateLock();
    lock->timeout = timeout;

    if (type == "GroundTouch")
    {
        Ogre::Vector3 pos = node->getPosition();
        GUtils::getVerticalRayPos(pos, 0, 10);
        node->setPosition(pos);

        pos.y += 1;

        lock->volume.fromSphere(2, pos);
        lock->registerDetection();
    }
    if (type == "GroundEnergy")
    {
        Ogre::Vector3 pos = node->getPosition();
        GUtils::getVerticalRayPos(pos, 0, 10);
        node->setPosition(pos);

        SceneEnergies::createEnergyPlacement(pos, Ogre::Vector3(0, 1, 0), std::bind(&GateLock::unlock, lock), std::bind(&GateLock::lock, lock));
    }
    if (type == "WallEnergy")
    {
        GUtils::RayInfo ray;
        Ogre::Vector3 pos = node->getPosition();

        if (GUtils::getRayInfo(pos, pos + node->getOrientation()*Ogre::Vector3(10, 0, 0), ray))
        {
            node->setPosition(ray.pos);

            SceneEnergies::createEnergyPlacement(ray.pos, ray.normal, std::bind(&GateLock::unlock, lock), std::bind(&GateLock::lock, lock));
        }
        else
            throw;
    }

    auto gate = Gate::getGate(id);
    if (gate)
    {
        lock->targetGate = gate;
        gate->addLock(lock);
    }

    locks[id].push_back(lock);

    return lock;
}

void GateLock::lock()
{
    if (!locked && targetGate)
    {
        locked = true;
        targetGate->lockOne(false);
    }
}

void GateLock::unlock()
{
    if (locked && targetGate)
    {
        locked = false;
        targetGate->unlockOne();
    }
}

std::vector<GateLock*> GateLock::getLocks(std::string id)
{
    if (locks.find(id) != locks.end())
        return locks[id];

    return std::vector<GateLock*>();
}

void GateLock::playerInside()
{
    unlock();
}

