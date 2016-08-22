#include "stdafx.h"
#include "GameStateManager.h"
#include "Player.h"
#include "levelsInit.h"
#include "DebugKeys.h"
#include "VolumeDetection.h"
#include "GUtils.h"
#include "Energy.h"
#include "Gate.h"

GameStateManager::GameStateManager(Ogre::Camera* cam, Ogre::RenderSystem* rs) : audioLib(cam)
{
	Global::audioLib = &audioLib;

	gameConfig.loadCfg();

	myMenu = new GuiOverlay(&gameConfig, cam, Global::mWindow, rs);

	wMaterials.init();

	LevelInfo info;
	info.name = "menu";
	info.init = createMenuLevel;
	info.lut = "normal.png";
	levels[0] = info;

	info.name = "park";
	info.init = createLevelTuto;
	levels[1] = info;

	info.name = "caves";
	info.init = createLevel1_1;
	levels[2] = info;

	info.name = "valley";
	info.init = createLevel2;
	levels[3] = info;

	info.name = "Test1";
	info.init = createTestLevel;
	levels[4] = info;

	info.name = "testLvl2";
	info.init = createTestLevel2;
	info.lut = "LUT_Filmic7.png";
	levels[5] = info;

	dbg = new DebugKeys();
	dbg->registerInputListening();
}

GameStateManager::~GameStateManager()
{
	delete dbg;
	delete myMenu;
}

void GameStateManager::switchToMainMenu()
{
	switchToLevel(0);
}

std::string GameStateManager::getCurrentLvlPath()
{
	return Path::Levels + levels[lastLVL].name + "/";
}

std::string GameStateManager::getCurrentLvlScenePath()
{
	return getCurrentLvlPath() + levels[lastLVL].name + ".scene";
}

LevelInfo* GameStateManager::getCurrentLvlInfo()
{
	return &levels[lastLVL];
}

LevelInfo* GameStateManager::getLvlInfo(int id)
{
	if (levels.find(id) != levels.end())
		return &levels[id];

	return nullptr;
}

void GameStateManager::switchToLevel(int lvl)
{
	if (lvl == 0)
		myMenu->setMainMenu();
	else if (gameState == PAUSE || gameState == MENU)
		myMenu->clearMenu();

	lastLVL = lvl;
	gameState = lvl == 0 ? MENU : GAME;

	if (Global::player)
	{
		delete Global::player;
		Global::player = nullptr;
	}

	clearLevel();

	if (gameState == GAME)
	{
		Global::player = new Player(&wMaterials);
	}

	auto& lvlInfo = levels[lvl];

	GameScene::loadScene(getCurrentLvlScenePath());

	sceneEdits.loadChanges();
	geometryMgr->postLoad();
	lvlInfo.init();

	SceneCubeMap::renderAll();

	Global::mPPMgr->setColorGradingPreset(lvlInfo.lut);
	Global::mPPMgr->fadeIn(Vector3(0, 0, 0), 2.f, true);
}

void GameStateManager::restartLevel()
{
	switchToLevel(lastLVL);
}

void GameStateManager::reloadLevel()
{
	GameScene::reloadScene(getCurrentLvlScenePath());
}

void GameStateManager::reloadMeshes()
{
	GameScene::reloadMeshes(getCurrentLvlPath(), levels[lastLVL].name);
}

bool GameStateManager::insideMenuPressed()
{
	bool continueExecution = true;
	int i = myMenu->mainMenuPressed();

	if (i > 0)
	{
		switchState(i);
	}
	if (i == SS_MAINMENU)
	{
		if (gameState == PAUSE)
			switchState(i);
		else
			continueExecution = false;
	}
	if (i == -1)
	{
		Global::mPPMgr->vars.radialHorizBlurVignette.z = 0.0;
		gameState = GAME;
	}
	if (i == SS_RESTART)
	{
		switchState(i);
	}

	return continueExecution;
}

void GameStateManager::insideMenuMoved(int x, int y)
{
	myMenu->mouseMoveUpdate(x,y);
}

void GameStateManager::switchState(int target, float time)
{
	stateTarget = target;
	switchStateTimer = time;

	Global::mPPMgr->fadeOut(Vector3(0, 0, 0), time);
	myMenu->clearMenu();
}

void GameStateManager::updateStateSwitching(float tslf)
{
	if (switchStateTimer > 0)
	{
		switchStateTimer -= tslf;
		return;
	}
	else if (!switchingState)
	{
		switchingState = true;
		return;
	}

	if (stateTarget > 0)
	{
		switchToLevel(stateTarget);
	}
	if (stateTarget == SS_MAINMENU)
	{
		switchToMainMenu();
	}
	if (stateTarget == SS_RESTART)
	{
		restartLevel();
	}

	stateTarget = 0;
	switchingState = false;
}

void GameStateManager::update(float tslf)
{
	if (stateTarget!=0)
		updateStateSwitching(tslf);
	else
		switch (gameState)
		{
		case GAME:
			audioLib.update(tslf);
			myMenu->updateIngame(tslf);
			break;
		case PAUSE:
			myMenu->updateIngameMenu(tslf);
			break;
		case MENU:
			myMenu->updateMainMenu(tslf);
			break;
		}
}

void GameStateManager::escapePressed()
{
	static float lastNoise = 0;
	static float lastCont = 0;

	if (gameState == GAME)
	{
		myMenu->clearMenu();
		myMenu->setIngameMenu();
		Global::mPPMgr->vars.radialHorizBlurVignette.z = 2.0;

		lastNoise = Global::mPPMgr->vars.ContSatuSharpNoise.z;
		lastCont = Global::mPPMgr->vars.ContSatuSharpNoise.x;
		Global::mPPMgr->vars.ContSatuSharpNoise.z = 1;
		Global::mPPMgr->vars.ContSatuSharpNoise.x = 1;

		gameState = PAUSE;
	}
	else if (gameState == PAUSE)
	{
		myMenu->clearMenu();
		Global::mPPMgr->vars.radialHorizBlurVignette.z = 0.0;
		Global::mPPMgr->vars.ContSatuSharpNoise.z = lastNoise;
		Global::mPPMgr->vars.ContSatuSharpNoise.x = lastCont;

		gameState = GAME;
	}
}

void GameStateManager::clearLevel()
{
	SceneCubeMap::clearAll();
	geometryMgr->clear();
	Global::mWorld->destroyAllBodies();
	Global::mSceneMgr->clearScene();
	Global::mEventsMgr->clear();
	Global::audioLib->reset();
	Global::mPPMgr->resetValues();
	VolumeDetectionManager::instance.reset();
	SceneEnergies::reset();
	Gate::reset();
	sceneEdits.clear();
	particleMgr.clear();

	Global::mWorld->setWorldSize(Vector3(-15000, -500, -15000), Vector3(15000, 2000, 15000));
}
