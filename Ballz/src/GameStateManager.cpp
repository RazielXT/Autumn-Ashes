#include "stdafx.h"
#include "GameStateManager.h"
#include "Player.h"
#include "levelsInit.h"
#include "DebugKeys.h"
#include "VolumeDetection.h"
#include "GUtils.h"
#include "Energy.h"
#include "Gate.h"
#include "..\EditorControl.h"
#include "SUtils.h"

GameStateManager::GameStateManager(Ogre::Camera* cam, Ogre::RenderSystem* rs) : audioLib(cam)
{
	Global::audioLib = &audioLib;

	gameConfig.loadCfg();

	myMenu = new GuiOverlay(&gameConfig, cam, Global::window, rs);

	wMaterials.init();

	LevelInfo info;
	info.name = "Menu";
	info.directory = "menu";
	levels.push_back(info);

	ConfigFile cf;
	cf.load(Path::Levels + std::string("levels.ini"));

	ConfigFile::SectionIterator seci = cf.getSectionIterator();
	while (seci.hasMoreElements())
	{
		LevelInfo info;
		info.name = seci.peekNextKey();
		ConfigFile::SettingsMultiMap *settings = seci.getNext();

		if (!info.name.empty())
		{
			for (auto i = settings->begin(); i != settings->end(); ++i)
			{
				if (i->first == "folder")
					info.directory = i->second;
			}

			levels.push_back(info);
		}
	}

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
	return Path::Levels + levels[lastLVL].directory + "/";
}

std::string GameStateManager::getCurrentLvlScenePath()
{
	return getCurrentLvlPath() + levels[lastLVL].directory + ".scene";
}

std::vector<LevelInfo> GameStateManager::getLevels()
{
	return levels;
}

LevelInfo* GameStateManager::getCurrentLvlInfo()
{
	return &levels[lastLVL];
}

LevelInfo* GameStateManager::getLvlInfo(int id)
{
	if (levels.size() > (size_t)id)
		return &levels[id];

	return nullptr;
}

void GameStateManager::switchToLevel(int lvl)
{
#ifdef EDITOR
	Global::editor->uiHandler.sendMsg(UiMessage {UiMessageId::StartLoading});
	gameState = GAME;
#else
	if (lvl == 0)
		myMenu->setMainMenu();
	else if (gameState == PAUSE || gameState == MENU)
		myMenu->clearMenu();

	gameState = lvl == 0 ? MENU : GAME;
#endif

	if (Global::player)
	{
		delete Global::player;
		Global::player = nullptr;
	}

	clearLevel();

	lastLVL = lvl;

	if (gameState == GAME)
	{
		Global::player = new Player(&wMaterials);
	}

	auto& lvlInfo = levels[lvl];
	ResourceGroupManager::getSingleton().addResourceLocation(getCurrentLvlPath(), "FileSystem", "Level");
	ResourceGroupManager::getSingleton().addResourceLocation(getCurrentLvlPath() + "cubemaps", "FileSystem", "Level");
	ResourceGroupManager::getSingleton().initialiseResourceGroup("Level");

	GameScene::loadScene(getCurrentLvlScenePath());

	sceneEdits.loadChanges();
	geometryMgr->postLoad();
	//lvlInfo.init();

	Global::editor->afterLoadInit();

	SceneCubeMap::renderAll();

	//Global::ppMgr->setColorGradingPreset(lvlInfo.lut);
	Global::ppMgr->fadeIn(Vector3(0, 0, 0), 2.f, true);
	Global::ppMgr->setAutoGodraySunDirection();

#ifdef EDITOR
	Global::editor->uiHandler.sendMsg(UiMessage { UiMessageId::EndLoading });
#endif
}

void GameStateManager::switchToLevel(std::string& lvl)
{
	for (size_t i = 0; i < levels.size(); i++)
	{
		if (levels[i].name == lvl)
		{
			switchToLevel(i);
			break;
		}
	}
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
		Global::ppMgr->vars.radialHorizBlurVignette.z = 0.0;
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

	Global::ppMgr->fadeOut(Vector3(0, 0, 0), time);
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
		Global::ppMgr->vars.radialHorizBlurVignette.z = 2.0;

		lastNoise = Global::ppMgr->vars.ContSatuSharpNoise.z;
		lastCont = Global::ppMgr->vars.ContSatuSharpNoise.x;
		Global::ppMgr->vars.ContSatuSharpNoise.z = 1;
		Global::ppMgr->vars.ContSatuSharpNoise.x = 1;

		gameState = PAUSE;
	}
	else if (gameState == PAUSE)
	{
		myMenu->clearMenu();
		Global::ppMgr->vars.radialHorizBlurVignette.z = 0.0;
		Global::ppMgr->vars.ContSatuSharpNoise.z = lastNoise;
		Global::ppMgr->vars.ContSatuSharpNoise.x = lastCont;

		gameState = GAME;
	}
}

void GameStateManager::clearLevel()
{
	SceneCubeMap::clearAll();
	geometryMgr->clear();
	Global::nWorld->destroyAllBodies();
	Global::sceneMgr->clearScene();
	Global::eventsMgr->clear();
	Global::audioLib->reset();
	Global::ppMgr->resetValues();
	VolumeDetectionManager::instance.reset();
	SceneEnergies::reset();
	Gate::reset();
	sceneEdits.clear();
	particleMgr.clear();

	Global::nWorld->setWorldSize(Vector3(-15000, -500, -15000), Vector3(15000, 2000, 15000));

	if (lastLVL != -1)
	{
		ResourceGroupManager::getSingleton().destroyResourceGroup("Level");
	}
}
