#pragma once

#include "stdafx.h"
#include "..\EditorUiHandler.h"

class MainListener;

class Application
{
public:

	void go();
	~Application();

private:

	size_t windowHnd;
	Ogre::Root* mRoot = nullptr;
	Ogre::RenderWindow* mWindow = nullptr;
	OIS::Keyboard* mKeyboard = nullptr;
	OIS::Mouse* mMouse = nullptr;
	OIS::InputManager* mInputManager = nullptr;
	MainListener* mListener = nullptr;
	Ogre::SceneManager* mSceneMgr = nullptr;
	OgreNewt::World* mWorld = nullptr;
	EditorUiHandler editorHandler;

	void createRoot();

	void defineResources();

	void setupRenderSystem();

	void createRenderWindow();

	void initializeResourceGroups();

	void setupScene();

	static LRESULT CALLBACK windowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static LONG mOldWindowProc;
	static UINT dbgMsg;

	void setupInputSystem();

	void createFrameListener();

	void startRenderLoop();
};
