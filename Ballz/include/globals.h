#pragma once

class EventsManager;
class InputListener;
class Player;
class GameStateManager;
class GeometryManager;
class PostProcessMgr;
class AudioLibrary;
class CameraShaker;

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

extern CameraShaker* shaker;

extern std::map<std::string, void*>* globalData;

extern std::vector<std::string> debug;

#define MAX_DEBUG_LINES 15
extern void DebugPrint(const std::string line, bool logFile = false);

};

extern bool continueExecution;