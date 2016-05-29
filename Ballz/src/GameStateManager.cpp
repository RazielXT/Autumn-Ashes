#include "stdafx.h"
#include "GameStateManager.h"
#include "Player.h"
#include "levelsInit.h"
#include "DebugKeys.h"
#include "VolumeDetection.h"
#include "GUtils.h"
#include "Energy.h"
#include "Gate.h"

void LevelInfo::applyPostProcessing()
{
	PostProcessMgr* postProcMgr = Global::mPPMgr;

	postProcMgr->vars.ColouringShift = ColorShift;
	postProcMgr->vars.ContSatuSharpNoise = ContSatuSharpNoise;
	postProcMgr->vars.bloomStrDepAddSize.z = bloomAdd;
	postProcMgr->vars.bloomStrDepAddSize.x = bloomStr;
	postProcMgr->vars.bloomStrDepAddSize.y = bloomDepth;
	postProcMgr->vars.bloomStrDepAddSize.w = bloomSize;
}

void LevelInfo::applyFog()
{
	Global::mSceneMgr->setFog(Ogre::FOG_LINEAR, fogColor, 1, fogStartDistance, fogEndDistance);
}

GameStateManager::GameStateManager(Ogre::Camera* cam, Ogre::RenderSystem* rs) : audioLib(cam)
{
	Global::audioLib = &audioLib;

	gameConfig.loadCfg();

	myMenu = new GuiOverlay(&gameConfig, cam, Global::mWindow, rs);

	wMaterials.init();

	LevelInfo info;
	info.name = "MainMenu";
	info.path = "../../media/menu/";
	info.sceneFile = "menu.scene";
	info.prefix = "test";
	info.init = createMenuLevel;
	info.fogColor = Ogre::ColourValue(0.5f, 0.55f, 0.65f, 0.5f);
	info.fogStartDistance = 80;
	info.fogEndDistance = 150;
	info.ColorShift = Ogre::Vector4(1.08f, 1.12f, 1.16f, 1);
	info.ContSatuSharpNoise = 0;
	info.ambientColor = ColourValue(0.35f, 0.35f, 0.35f);
	info.sunColor = ColourValue(1, 1, 1);
	info.skyboxName = "TCENoonSkyBox";
	info.bloomStr = 1;
	info.bloomDepth = 0.35f;
	levels[0] = info;

	info.name = "Park";
	info.path = "../../media/park/";
	info.sceneFile = "park.scene";
	info.init = createLevelTuto;
	info.bloomStr = 1.5f;
	info.bloomDepth = 0.5f;
	levels[1] = info;

	info.name = "Caves";
	info.path = "../../media/park/";
	info.sceneFile = "park.scene";
	info.init = createLevel1_1;
	levels[2] = info;

	info.name = "Valley";
	info.path = "../../media/valley/";
	info.sceneFile = "valley.scene";
	info.init = createLevel2;
	levels[3] = info;

	info.name = "Test1";
	info.path = "../../media/testLvl/";
	info.sceneFile = "test.scene";
	info.init = createTestLevel;
	info.fogColor = Ogre::ColourValue(0.5f, 0.55f, 0.65f, 0.5f);
	info.fogStartDistance = 80;
	info.fogEndDistance = 150;
	info.ColorShift = Ogre::Vector4(1.0f, 1.0f, 1.02f, 1);
	info.ContSatuSharpNoise = Ogre::Vector4(0, 0, 0, 0);
	info.ambientColor = ColourValue(0.35f, 0.35f, 0.35f);
	info.skyboxName = "TCENoonSkyBox";
	info.bloomStr = 1.1f;
	info.bloomDepth = 0.38f;
	info.bloomAdd = 0.45f;
	info.bloomSize = 1.5f;
	levels[4] = info;

	info.name = "Test2";
	info.path = "../../media/testLvl2/";
	info.sceneFile = "test.scene";
	info.init = createTestLevel2;
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

	GameScene::loadScene(lvlInfo.path + lvlInfo.sceneFile);

	loadSceneSettings();
	sceneEdits.loadChanges();
	lvlInfo.init();

	geometryMgr->update();
	SceneCubeMap::renderAll();

	Global::mPPMgr->fadeIn(Vector3(0, 0, 0), 2.f, true);
}

void GameStateManager::loadSceneSettings()
{
	auto& lvlInfo = levels[lastLVL];

	Global::mSceneMgr->setAmbientLight(lvlInfo.ambientColor);
	Global::mSceneMgr->setSkyBox(true, lvlInfo.skyboxName);

	lvlInfo.applyPostProcessing();
	lvlInfo.applyFog();
}

void GameStateManager::restartLevel()
{
	switchToLevel(lastLVL);
}

void GameStateManager::reloadLevel()
{
	GameScene::reloadScene(levels[lastLVL].path + levels[lastLVL].sceneFile);
}

void GameStateManager::reloadMeshes()
{
	GameScene::reloadMeshes(levels[lastLVL].path, levels[lastLVL].prefix);
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

	Global::mWorld->setWorldSize(Vector3(-2000, -500, -2000), Vector3(2000, 500, 2000));
}
