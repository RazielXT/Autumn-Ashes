#pragma once

#include "stdafx.h"

class MainListener;

class Application
{
public:

	void go();
	~Application();

private:

	Ogre::Root* mRoot = nullptr;
	Ogre::RenderWindow* mWindow = nullptr;
	OIS::Keyboard* mKeyboard = nullptr;
	OIS::Mouse* mMouse = nullptr;
	OIS::InputManager* mInputManager = nullptr;
	MainListener* mListener = nullptr;
	Ogre::SceneManager* mSceneMgr = nullptr;
	OgreNewt::World* mWorld = nullptr;

	void createRoot();

	void defineResources();

	void setupRenderSystem();

	void createRenderWindow();

	void initializeResourceGroups();

	void setupScene();

	void setupInputSystem();

	void createFrameListener();

	void startRenderLoop();
};
