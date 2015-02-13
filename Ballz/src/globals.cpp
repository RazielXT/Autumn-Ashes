#include "stdafx.h"

float Global::timestep = 1;
AudioLibrary* Global::audioLib = 0;
float Global::fallSoundOffsetH = 0;
float Global::fallSoundOffsetL = 0;
EventsManager* Global::mEventsMgr = 0;
Ogre::SceneManager* Global::mSceneMgr = 0;
Ogre::RenderWindow* Global::mWindow = 0;
OgreNewt::World* Global::mWorld = 0;
irrklang::ISoundEngine* Global::soundEngine = 0;
PagingManager* Global::mPagingMgr = 0;
Player* Global::player = 0;
GameStateManager* Global::gameMgr = 0;
PostProcessMgr* Global::mPPMgr = 0;
std::map<std::string,void*>* Global::globalData = new std::map<std::string,void*>();
