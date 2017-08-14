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
	void returnToPlayer();

	void setStrafeMode(bool enable);

	virtual void pressedKey(const OIS::KeyEvent &arg);
	virtual void releasedKey(const OIS::KeyEvent &arg);
	virtual void movedMouse(const OIS::MouseEvent &e);
	virtual void mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	virtual void mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

	virtual bool update(float tslf) override;

	bool strafe = false;
	bool front, back, left, right, shift, alt, ctrl, space;
	Ogre::SceneNode* camNode = nullptr;
};