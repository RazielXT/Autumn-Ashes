#include "stdafx.h"

float Global::timestep = 1;
float Global::tslf = 0;
AudioLibrary* Global::audioLib = nullptr;
EventsManager* Global::eventsMgr = nullptr;
Ogre::SceneManager* Global::sceneMgr = nullptr;
Ogre::RenderWindow* Global::window = nullptr;
OgreNewt::World* Global::nWorld = nullptr;
irrklang::ISoundEngine* Global::soundEngine = nullptr;
Player* Global::player = nullptr;
CameraHelp* Global::camera = nullptr;
GameStateManager* Global::gameMgr = nullptr;
PostProcessMgr* Global::ppMgr = nullptr;
std::map<std::string,void*>* Global::globalData = new std::map<std::string,void*>();
EditorControl* Global::editor = nullptr;

bool continueExecution = true;

const char* Path::Levels = "../../media/levels/";
const char* Path::General = "../../media/general/";
const char* Path::Audio = "../../media/audio/";
const char* Path::LutTextures = "../../media/lut/";
