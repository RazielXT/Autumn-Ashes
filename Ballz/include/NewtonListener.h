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

    bool frameStarted(const FrameEvent& evt)
    {
        m_elapsed+=evt.timeSinceLastFrame*1.5f;

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
    Real m_elapsed;
    Real m_update;
};