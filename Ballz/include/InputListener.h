#pragma once

#include "stdafx.h"
#include <algorithm>
#include <vector>

class InputListener
{
public:

	GameState executionState = PLAY;
    virtual void keyInput(const OIS::KeyEvent &arg) = 0;

    void unregisterKeyListening()
    {
        if (Global::mEventsMgr)
        {
            Global::mEventsMgr->removeInputListener(this);
        }
    }

    virtual ~InputListener()
    {
        unregisterKeyListening();
    }
};