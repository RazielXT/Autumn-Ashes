#include "stdafx.h"
#include "GameStateManager.h"
#include "Player.h"
#include "levelsInit.h"

SceneParser SceneParser::instance;

GameStateManager::GameStateManager(Ogre::Camera* cam, Ogre::RenderSystem* rs, WorldMaterials* wMaterials)
{
    gameConfig.loadCfg();

    myMenu = new GuiOverlay(&gameConfig, cam, Global::mWindow, rs, Global::soundEngine);
    this->wMaterials = wMaterials;

    LevelInfo info;
    info.path = "../../media/menu/menu.scene";
    info.init = std::bind(&GameStateManager::switchToMainMenu, this);
    levels[0] = info;

    info.path = "../../media/park/park.scene";
    info.init = createLevelTuto;
    levels[1] = info;

    info.path = "../../media/caves.scene";
    info.init = createLevel1_1;
    levels[2] = info;

    info.path = "../../media/valley/valley.scene";
    info.init = createLevel2;
    levels[3] = info;

    info.path = "../../media/testLvl/test.scene";
    info.init = createTestLevel;
    levels[4] = info;
}

GameStateManager::~GameStateManager()
{
    delete myMenu;
}

void GameStateManager::switchToMainMenu()
{
    if (Global::player)
    {
        delete Global::player;
        Global::player = NULL;
    }

    geometryMgr->clear();
    Global::mWorld->destroyAllBodies();
    Global::mSceneMgr->clearScene();
    Global::mEventsMgr->clear();
    Global::soundEngine->removeAllSoundSources();
    Global::mPPMgr->resetValues();
    myMenu->setMainMenu();

    gameState = MENU;
    createMenuLevel();
}

void GameStateManager::switchToLevel(int lvl)
{
    if (gameState == PAUSE)
        myMenu->clearMenu();

    lastLVL = lvl;

    if (Global::player)
    {
        delete Global::player;
        Global::player = NULL;
    }

    if (gameState == MENU)
        myMenu->clearMenu();

    gameState = GAME;
    geometryMgr->clear();

    Global::mWorld->destroyAllBodies();
    Global::mSceneMgr->clearScene();
    Global::mEventsMgr->clear();
    Global::soundEngine->removeAllSoundSources();


    Player* p = new Player(wMaterials);
    auto postProcMgr = Global::mPPMgr;

    Global::player = p;

    levels[lvl].init();

    Global::mPPMgr->fadeIn(Vector3(0, 0, 0), 2.f, true);
}

void GameStateManager::restartLevel()
{
    switchToLevel(lastLVL);
}

void GameStateManager::reloadLevel()
{
    SceneParser::instance.reloadScene(levels[lastLVL].path);
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
        Global::mPPMgr->radialHorizBlurVignette.z = 0.0;
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
            myMenu->setDebugValue(Global::mWindow->getLastFPS(), Global::debug);
            myMenu->updateIngame(tslf);
            break;
        case PAUSE:
            myMenu->updateIngameMenu(tslf);
            break;
        case MENU:
            myMenu->updateMainMenu(tslf);
            break;
        }

    if (Global::fallSoundOffsetH > 0)
    {
        Global::fallSoundOffsetH -= tslf;
    }
    if (Global::fallSoundOffsetL > 0)
    {
        Global::fallSoundOffsetL -= tslf;
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
        Global::mPPMgr->radialHorizBlurVignette.z = 2.0;

        lastNoise = Global::mPPMgr->ContSatuSharpNoise.z;
        lastCont = Global::mPPMgr->ContSatuSharpNoise.x;
        Global::mPPMgr->ContSatuSharpNoise.z = 1;
        Global::mPPMgr->ContSatuSharpNoise.x = 1;

        gameState = PAUSE;
    }
    else if (gameState == PAUSE)
    {
        myMenu->clearMenu();
        Global::mPPMgr->radialHorizBlurVignette.z = 0.0;
        Global::mPPMgr->ContSatuSharpNoise.z = lastNoise;
        Global::mPPMgr->ContSatuSharpNoise.x = lastCont;

        gameState = GAME;
    }
}
