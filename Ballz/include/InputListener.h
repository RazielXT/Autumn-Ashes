#pragma once

#include "stdafx.h"
#include <algorithm>
#include <vector>

class InputListener
{
public:

    GameState executionState = GAME;

    virtual void pressedKey(const OIS::KeyEvent &arg) {};
	virtual void releasedKey(const OIS::KeyEvent &arg) {};
    virtual void movedMouse(const OIS::MouseEvent &e) {};

    inline void registerInputListening()
    {
        Global::mEventsMgr->addInputListener(this);
    }

    inline void unregisterInputListening()
    {
        Global::mEventsMgr->removeInputListener(this);
    }

    virtual ~InputListener()
    {
        unregisterInputListening();
    }
};