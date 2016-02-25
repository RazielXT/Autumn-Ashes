#pragma once

#include "Gate.h"
#include "VolumeDetection.h"

class GateLock : public VolumeDetection
{
public:

	static std::vector<GateLock*> getLocks(std::string id);
	static GateLock* createLock(Ogre::SceneNode* node, std::string type, std::string id, int timeout);

	void lock();
	void unlock();

	Gate* targetGate = nullptr;
	int timeout = 0;
	bool locked = true;

private:

	virtual void playerInside();

	static std::map<std::string, std::vector<GateLock*>> locks;
};