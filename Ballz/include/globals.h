#pragma once

class EventsManager;
class Player;
class GameStateManager;
class GeometryManager;
class PostProcessMgr;
class AudioLibrary;
class CameraShaker;

enum GameState { GAME, PAUSE, MENU, CONSOLE, UNDEFINED };
enum VisibilityFlag { VisibilityFlag_Water = 16 };
enum RenderQueueID { RenderQueue_Transparent = 81, RenderQueue_Particles = 91 };

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
#define DEBUG_VARIABLES_COUNT 7

extern void DebugPrint(const std::string line, bool logFile = false);
};

extern bool continueExecution;