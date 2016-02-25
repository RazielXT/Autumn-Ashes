#pragma once

#include "stdafx.h"

class DynamicRope
{
public:

	DynamicRope(Ogre::Vector3 pos, int size, int parts, bool climbable);

private:


};

/*

#pragma once

#include "stdafx.h"
#include "EventTask.h"
#include "InputListener.h"

class Rope : public EventTask, public InputListener
{
public:

//take cam, disable player, create physics with force+joint, connect event
virtual bool start();

//update push force, update camera pos from offset
virtual bool update(Ogre::Real tslf);

//dirs + space
virtual void pressedKey(const OIS::KeyEvent &arg);
virtual void releasedKey(const OIS::KeyEvent &arg);
virtual void movedMouse(const OIS::MouseEvent &e);

//push force
void move_callback(OgreNewt::Body* me, float timeStep, int threadIndex);

//jump + unregister + return player
virtual bool release();

private:

};

*/