#pragma once

#include "stdafx.h"
#include "InputListener.h"
#include "EventTask.h"

class EditorCamera : public InputListener, EventTask
{
public:

	EditorCamera();

	void enable();
	void disable();

protected:

	virtual void pressedKey(const OIS::KeyEvent &arg);
	virtual void releasedKey(const OIS::KeyEvent &arg);
	virtual void movedMouse(const OIS::MouseEvent &e);

	virtual bool update(float tslf) override;

	bool front, back, left, right, shift, space;
	Ogre::SceneNode* camNode = nullptr;
};