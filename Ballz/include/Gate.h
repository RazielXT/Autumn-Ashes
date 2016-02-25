#pragma once

#include <stdafx.h>
#include <string>
#include <map>
#include <vector>

class GateLock;

class Gate
{
public:

	static Gate* createPhysicalGate(Ogre::Entity* ent, Ogre::Vector3 dir, std::string id, int timeout, int minUnlocks);
	static Gate* createFieldGate(Ogre::Entity* ent, std::string id, int timeout, int minUnlocks);

	static Gate* getGate(std::string id);
	static void reset();

	void lockOne(bool notifyLocks);
	void unlockOne();

	void lock(bool notifyLocks);
	void unlock();

	bool locked = true;

	void addLock(GateLock* lock);

private:

	bool unlockCondition();

	Ogre::Entity* ent;
	OgreNewt::Body* body;
	bool field;
	Ogre::Vector3 unlockDir;

	int minUnlocks = 0;
	int timeout = 0;
	int timeoutTimer = 0;
	int currentUnlocks = 0;


	static Gate* makeGate(std::string id, int timeout, int minUnlocks);

	std::vector<GateLock*> locks;

	static std::map<std::string, Gate*> gates;
};