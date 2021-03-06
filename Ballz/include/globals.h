#pragma once

class EventsManager;
class Player;
class PlayerCamera;
class GameStateManager;
class GeometryManager;
class PostProcessMgr;
class AudioLibrary;
class CameraShaker;
struct CameraHelp;
class EditorControl;

#define MAX_DEBUG_LINES 15
#define DEBUG_VARIABLES_COUNT 7

enum GameState { GAME, PAUSE, MENU, UNDEFINED };

enum class SelectionMode { Select, Move, Rotate, Scale };

enum VisibilityFlag
{
	VisibilityFlag_Normal = 1,
	VisibilityFlag_Instanced = 2,
	VisibilityFlag_Transparent = 4,
	VisibilityFlag_SoftParticles = 8,
	VisibilityFlag_Water = 16,
	VisibilityFlag_NeedCompositor = VisibilityFlag_Water | VisibilityFlag_SoftParticles | VisibilityFlag_Transparent,
	VisibilityFlag_Temp = 32
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
extern Ogre::SceneManager* sceneMgr;
extern OgreNewt::World* nWorld;
extern irrklang::ISoundEngine* soundEngine;
extern Ogre::RenderWindow* window;

extern EventsManager* eventsMgr;
extern GameStateManager* gameMgr;
extern PostProcessMgr* ppMgr;

extern CameraHelp* camera;
extern Player* player;
extern float timestep;
extern float tslf;
extern AudioLibrary* audioLib;
extern CameraShaker* shaker;

extern std::map<std::string, void*>* globalData;
extern EditorControl* editor;
};

namespace Path
{
extern const char* Levels;
extern const char* Audio;
extern const char* General;
extern const char* LutTextures;
};

extern bool continueExecution;