#include "stdafx.h"

float Global::timestep = 1;
AudioLibrary* Global::audioLib = nullptr;
EventsManager* Global::mEventsMgr = nullptr;
Ogre::SceneManager* Global::mSceneMgr = nullptr;
CameraShaker* Global::shaker = nullptr;
Ogre::RenderWindow* Global::mWindow = nullptr;
OgreNewt::World* Global::mWorld = nullptr;
irrklang::ISoundEngine* Global::soundEngine = nullptr;
Player* Global::player = nullptr;
PlayerCamera* Global::camera = nullptr;
GameStateManager* Global::gameMgr = nullptr;
PostProcessMgr* Global::mPPMgr = nullptr;
std::map<std::string,void*>* Global::globalData = new std::map<std::string,void*>();

bool continueExecution = true;
