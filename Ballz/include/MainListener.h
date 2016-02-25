#pragma once

#include "stdafx.h"
#include "OgreGpuCommandBufferFlush.h"
#include "NewtonListener.h"

class EventsManager;
class GeometryManager;
class PostProcessMgr;
class GameStateManager;

class MainListener : public Ogre::FrameListener, public OIS::MouseListener, public OIS::KeyListener
{
public:

	~MainListener();
	MainListener(OIS::Keyboard *keyboard, OIS::Mouse *mouse, Ogre::SceneManager * sceneMgr, OgreNewt::World* nWorld, Ogre::Root *mRoot, Ogre::RenderWindow* mWin);

	bool frameStarted(const Ogre::FrameEvent& evt);

	virtual bool keyPressed(const OIS::KeyEvent &arg);
	virtual bool keyReleased(const OIS::KeyEvent &arg);
	virtual bool mouseMoved(const OIS::MouseEvent &e);
	virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

private:

	OIS::Keyboard *mKeyboard;
	OIS::Mouse *mMouse;
	Ogre::SceneManager * mSceneMgr;
	Ogre::Camera* mCamera;
	OgreNewt::World* mWorld;
	Ogre::GpuCommandBufferFlush mBufferFlush;
	Ogre::RenderWindow* mWindow;

	EventsManager* mEventMgr;
	GeometryManager* geometryMgr;
	PostProcessMgr* postProcMgr;
	GameStateManager* gameMgr;
	NewtonListener nListener;
};
