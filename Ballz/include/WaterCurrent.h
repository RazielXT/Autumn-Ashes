#pragma once

#include "stdafx.h"
#include "EventTask.h"

class WaterCurrent
{
public:

	static WaterCurrent* get()
	{
		if (!instance)
			instance = new WaterCurrent();

		return instance;
	}

	void addCurrent(Ogre::Vector3 start, Ogre::Vector3 end, float power, float minWidth);
	Ogre::Vector3 findCurrent(Ogre::Vector3 pos);

private:

	static WaterCurrent* instance;

	struct WCurrent
	{
		Ogre::Vector3 pos;
		float sqRad;
		Ogre::Vector3 dir;
	};

	std::vector<WCurrent> currents;
};