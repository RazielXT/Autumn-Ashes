#pragma once

#include "stdafx.h"
#include "WorldMaterials.h"
#include "GeometryManager.h"
#include "MaterialEditsLibrary.h"

class GuiOverlay;
class DebugKeys;

enum StateSwitching
{
    SS_MAINMENU = -3,
    SS_RESTART = -2
};

struct LevelInfo
{
    std::string name;
    std::string path;
    std::function<void()> init;

    std::string skyboxName;

    Ogre::ColourValue fogColor;
    float fogStartDistance;
    float fogEndDistance;

    Ogre::ColourValue ambientColor;

    Ogre::Vector4 ContSatuSharpNoise;
    Ogre::Vector4 ColorShift;
    float bloomStr;
    float bloomDepth;
};

class GameStateManager
{
public:

    GameStateManager(Ogre::Camera* cam, Ogre::RenderSystem* rs, WorldMaterials* wMaterials);
    ~GameStateManager();

    LevelInfo* getLvlInfo(int id);
    LevelInfo* getCurrentLvlInfo();

    void switchToMainMenu();
    void switchToLevel(int lvl);

    void restartLevel();
    void reloadLevel();
    void reloadSceneSettings();

    void switchState(int target, float time = 1.0f);

    void update(float tslf);
    bool insideMenuPressed();
    void insideMenuMoved(int x, int y);
    void escapePressed();

    GameConfig gameConfig;
    GameState gameState;
    GuiOverlay* myMenu;
    WorldMaterials* wMaterials;
    GeometryManager* geometryMgr;
    MaterialEditsLibrary materialEdits;

    std::map<Ogre::String, OgreNewt::Body*>* loadedBodies;

    void addDebugKey(std::string name, float* target, float step = 0.2f);


private:

    std::map<int, LevelInfo> levels;

    void updateStateSwitching(float tslf);
    int stateTarget=0;
    bool switchingState = false;
    float switchStateTimer;

    int lastLVL;

    DebugKeys* dbg;
};