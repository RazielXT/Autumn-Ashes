#pragma once

#include <algorithm>
#include <vector>
#include "EventsManager.h"

class InputListener
{
public:

	GameState executionState = GAME;

	virtual void pressedKey(const OIS::KeyEvent &arg) {};
	virtual void releasedKey(const OIS::KeyEvent &arg) {};
	virtual void movedMouse(const OIS::MouseEvent &e) {};
	virtual void mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {};
	virtual void mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {};

	inline void registerInputListening()
	{
		Global::eventsMgr->addInputListener(this);
	}

	inline void unregisterInputListening()
	{
		Global::eventsMgr->removeInputListener(this);
	}

	virtual ~InputListener()
	{
		unregisterInputListening();
	}
};