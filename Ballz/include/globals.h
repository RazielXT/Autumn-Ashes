#ifndef GLOBALVALUES
#define GLOBALVALUES

class EventsManager;
class InputListener;
class Player;
class GameStateManager;
class PagingManager;
class PostProcessMgr;
class AudioLibrary;

struct Global
{
    static Ogre::SceneManager* mSceneMgr;
    static OgreNewt::World* mWorld;
    static irrklang::ISoundEngine* soundEngine;
    static Ogre::RenderWindow* mWindow;

    static EventsManager* mEventsMgr;
    static GameStateManager* gameMgr;
    static PagingManager* mPagingMgr;
    static PostProcessMgr* mPPMgr;

    static Player* player;
    static float timestep;
    static AudioLibrary* audioLib;
    static float fallSoundOffsetH;
    static float fallSoundOffsetL;

    static std::map<std::string,void*>* globalData;

    static float debug;
};

#endif GLOBALVALUES