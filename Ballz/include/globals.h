#pragma once

class EventsManager;
class Player;
class PlayerCamera;
class GameStateManager;
class GeometryManager;
class PostProcessMgr;
class AudioLibrary;
class CameraShaker;

#define MAX_DEBUG_LINES 15
#define DEBUG_VARIABLES_COUNT 7

enum GameState { GAME, PAUSE, MENU, CONSOLE, UNDEFINED };

enum VisibilityFlag
{
	VisibilityFlag_Normal = 1,
	VisibilityFlag_Water = 16,
	VisibilityFlag_Transparent = 4,
	VisibilityFlag_SoftParticles = 8,
	VisibilityFlag_NeedCompositor = VisibilityFlag_Water | VisibilityFlag_SoftParticles | VisibilityFlag_Transparent
};

enum RenderQueueID
{
	RenderQueue_Grass = 70,
	RenderQueue_BasicTransparent = 71,
	RenderQueue_Water = 81,
	RenderQueue_Transparent = 82,
	RenderQueue_SoftParticles = 91
};

namespace Global
{
extern Ogre::SceneManager* mSceneMgr;
extern OgreNewt::World* mWorld;
extern irrklang::ISoundEngine* soundEngine;
extern Ogre::RenderWindow* mWindow;

extern EventsManager* mEventsMgr;
extern GameStateManager* gameMgr;
extern PostProcessMgr* mPPMgr;

extern PlayerCamera* camera;
extern Player* player;
extern float timestep;
extern AudioLibrary* audioLib;
extern CameraShaker* shaker;

extern std::map<std::string, void*>* globalData;

};

extern bool continueExecution;