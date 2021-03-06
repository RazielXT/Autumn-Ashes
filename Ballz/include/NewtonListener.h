#pragma once

#include "stdafx.h"

class NewtonListener
{
public:

	NewtonListener() : m_World(nullptr) {}

	void init(OgreNewt::World* W)
	{
		m_World=W;
		m_update = 1.0f/60.0f;
		m_elapsed = 0.0f;
	}

	bool frameStarted(const float tslf)
	{
		m_elapsed += tslf*1.5f;

		if ((m_elapsed>m_update)&&(m_elapsed<1.0f))
		{
			while(m_elapsed>m_update)
			{
				m_World->update(m_update*Global::timestep);
				m_elapsed-=m_update;
			}
		}
		else
		{
			if(m_elapsed>=m_update)
			{
				m_World->update(m_elapsed*Global::timestep);
				m_elapsed=0.0f;
			}
		}

		return continueExecution;
	}

private:

	OgreNewt::World* m_World;
	Ogre::Real m_elapsed;
	Ogre::Real m_update;
};