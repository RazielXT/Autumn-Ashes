#pragma once

#include "Volume.h"
#include <vector>

class VolumeDetection;

class VolumeDetectionManager : public EventTask
{
public:

	static VolumeDetectionManager instance;

	void reset();
	void registerVolume(VolumeDetection* volume);

	virtual bool update(Ogre::Real tslf);

private:

	std::vector<VolumeDetection*> registeredVolumes;
};


class VolumeDetection
{
	friend VolumeDetectionManager;

protected:

	VolumeDetection()
	{
		VolumeDetectionManager::instance.registerVolume(this);
	}

	void registerDetection()
	{
		detectionRegistered = true;
	}

	void unregisterDetection()
	{
		detectionRegistered = false;
	}

	virtual void playerInside() = 0;

	bool detectionRegistered = false;
	Volume volume;
};
