#pragma once

#include "stdafx.h"
#include "WorldMaterials.h"
#include "GeometryManager.h"
#include "ParticleManager.h"
#include "SceneEditsLibrary.h"

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
	std::string directory;
};

class GameStateManager
{
public:

	GameStateManager(Ogre::Camera* cam, Ogre::RenderSystem* rs);
	~GameStateManager();

	LevelInfo* getLvlInfo(int id);
	LevelInfo* getCurrentLvlInfo();
	std::string getCurrentLvlPath();
	std::string getCurrentLvlScenePath();
	std::vector<LevelInfo> getLevels();

	void switchToMainMenu();
	void switchToLevel(int lvl);
	void switchToLevel(std::string& lvl);

	void restartLevel();
	void reloadLevel();
	void reloadMeshes();

	void switchState(int target, float time = 1.0f);

	void update(float tslf);
	bool insideMenuPressed();
	void insideMenuMoved(int x, int y);
	void escapePressed();

	GameConfig gameConfig;
	GameState gameState;
	GuiOverlay* myMenu;
	WorldMaterials wMaterials;
	GeometryManager* geometryMgr;
	ParticleManager particleMgr;
	SceneEditsLibrary sceneEdits;

	std::map<Ogre::String, OgreNewt::Body*>* loadedBodies;

private:

	void clearLevel();

	std::vector<LevelInfo> levels;

	void updateStateSwitching(float tslf);
	int stateTarget=0;
	bool switchingState = false;
	float switchStateTimer;

	int lastLVL = -1;

	AudioLibrary audioLib;

	DebugKeys* dbg;
};