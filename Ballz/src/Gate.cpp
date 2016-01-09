#include "stdafx.h"
#include "GateLock.h"
#include "GUtils.h"
#include "GameStateManager.h"

std::map<std::string, Gate*> Gate::gates;

Gate* Gate::getGate(std::string id)
{
    if (gates.find(id) != gates.end())
        return gates[id];

    return nullptr;
}

void Gate::reset()
{
    for (auto g : gates)
    {
        delete g.second;
    }

    gates.clear();
}

Gate* Gate::createPhysicalGate(Ogre::Entity* ent, Ogre::Vector3 dir, std::string id, int timeout, int minUnlocks)
{
    Gate* g = makeGate(id, timeout, minUnlocks);
    g->field = false;
    g->unlockDir = dir;

    return g;
}

Gate* Gate::createFieldGate(Ogre::Entity* ent, std::string id, int timeout, int minUnlocks)
{
    Gate* g = makeGate(id, timeout, minUnlocks);
    g->field = true;

    g->ent = ent;
    g->body = GUtils::createConvexBody(ent);

    g->body->setMaterialGroupID(Global::gameMgr->wMaterials.plBlock_mat);

    return g;
}

Gate* Gate::makeGate(std::string id, int timeout, int minUnlocks)
{
    if (gates.find(id) != gates.end())
        throw;

    Gate* g = new Gate();
    g->minUnlocks = minUnlocks;
    g->timeout = timeout;

    gates[id] = g;

    auto locks = GateLock::getLocks(id);
    for (auto l : locks)
    {
        l->targetGate = g;
    }

    g->locks.insert(g->locks.begin(), locks.begin(), locks.end());

    return g;
}

void Gate::lock(bool notifyLocks)
{
    if (locked)
        return;

    ent->setVisible(true);
    body->setPositionOrientation(body->getPosition() + Ogre::Vector3(0, 100, 0), body->getOrientation());
    locked = true;

    if (notifyLocks)
    {
        for (auto l : locks)
        {
            l->lock();
        }
    }
}

void Gate::unlock()
{
    if (!locked)
        return;

    ent->setVisible(false);
    body->setPositionOrientation(body->getPosition() - Ogre::Vector3(0, 100, 0), body->getOrientation());
    locked = false;
}

void Gate::lockOne(bool notifyLocks)
{
    currentUnlocks--;

    if (!unlockCondition())
        lock(notifyLocks);
}

void Gate::unlockOne()
{
    currentUnlocks++;
    timeoutTimer = timeout;

    if (unlockCondition())
        unlock();
}

bool Gate::unlockCondition()
{
    if (!minUnlocks && currentUnlocks == locks.size())
        return true;

    if (minUnlocks && minUnlocks >= currentUnlocks)
        return true;

    return false;
}

void Gate::addLock(GateLock* lock)
{
    locks.push_back(lock);
}

