#pragma once

class EventsManager;
class InputListener;
class Player;
class GameStateManager;
class PagingManager;
class PostProcessMgr;
class AudioLibrary;

namespace Global
{
extern Ogre::SceneManager* mSceneMgr;
extern OgreNewt::World* mWorld;
extern irrklang::ISoundEngine* soundEngine;
extern Ogre::RenderWindow* mWindow;

extern EventsManager* mEventsMgr;
extern GameStateManager* gameMgr;
extern PostProcessMgr* mPPMgr;

extern Player* player;
extern float timestep;
extern AudioLibrary* audioLib;
extern float fallSoundOffsetH;
extern float fallSoundOffsetL;

extern std::map<std::string, void*>* globalData;

extern float debug;
};

extern bool continueExecution;