#include "stdafx.h"
#include "GameStateManager.h"
#include "Player.h"
#include "levelsInit.h"
#include "DebugKeys.h"

SceneParser SceneParser::instance;

GameStateManager::GameStateManager(Ogre::Camera* cam, Ogre::RenderSystem* rs, WorldMaterials* wMaterials)
{
    gameConfig.loadCfg();

    myMenu = new GuiOverlay(&gameConfig, cam, Global::mWindow, rs, Global::soundEngine);
    this->wMaterials = wMaterials;

    LevelInfo info;
    info.path = "../../media/menu/menu.scene";
    info.init = createMenuLevel;
    info.fogColor = Ogre::ColourValue(0.5f, 0.55f, 0.65f, 0.5f);
    info.fogStartDistance = 80;
    info.fogEndDistance = 150;
    info.ColorShift = Ogre::Vector4(1.08f, 1.12f, 1.16f, 1);
    info.ContSatuSharpNoise = 0;
    info.ambientColor = ColourValue(0.35f, 0.35f, 0.35f);
    info.skyboxName = "TCENoonSkyBox";
    info.bloomStr = 10;
    info.bloomDepth = 0.35f;
    levels[0] = info;

    info.path = "../../media/park/park.scene";
    info.init = createLevelTuto;
    info.bloomStr = info.bloomDepth = 1;
    levels[1] = info;

    info.path = "../../media/caves.scene";
    info.init = createLevel1_1;
    levels[2] = info;

    info.path = "../../media/valley/valley.scene";
    info.init = createLevel2;
    levels[3] = info;

    info.path = "../../media/testLvl/test.scene";
    info.init = createTestLevel;
    info.ambientColor = ColourValue(0.25f, 0.35f, 0.4f);
    info.ColorShift = Ogre::Vector4(0.9f, 1.0f, 1.05f, 0);
    levels[4] = info;

    info.path = "../../media/testLvl2/test.scene";
    info.init = createTestLevel2;
    info.ambientColor = ColourValue(0.2f, 0.2f, 0.1f);
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
        Global::player = NULL;
    }

    geometryMgr->clear();
    Global::mWorld->destroyAllBodies();
    Global::mSceneMgr->clearScene();
    Global::mEventsMgr->clear();
    Global::soundEngine->removeAllSoundSources();
    Global::mPPMgr->resetValues();

    if (gameState == GAME)
    {
        Player* p = new Player(wMaterials);
        Global::player = p;
    }

    dbg->reloadVariables();

    auto& lvlInfo = levels[lvl];

    Global::mWorld->setWorldSize(Vector3(-2000, -500, -2000), Vector3(2000, 500, 2000));
    Global::mSceneMgr->setAmbientLight(lvlInfo.ambientColor);
    PostProcessMgr* postProcMgr = Global::mPPMgr;
    postProcMgr->ColouringShift = lvlInfo.ColorShift;
    postProcMgr->ContSatuSharpNoise = lvlInfo.ContSatuSharpNoise;
    postProcMgr->bloomStrDep.y = lvlInfo.bloomStr;
    postProcMgr->bloomStrDep.x = lvlInfo.bloomDepth;
    Global::mSceneMgr->setSkyBox(true, lvlInfo.skyboxName);
    Global::mSceneMgr->setFog(FOG_LINEAR, lvlInfo.fogColor, 1, lvlInfo.fogStartDistance, lvlInfo.fogEndDistance);

    SceneParser::instance.loadScene(lvlInfo.path);

    lvlInfo.init();

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

void GameStateManager::addDebugKey(std::string name, float* target, float step /* = 0.2f */)
{
    dbg->debugVars.push_back(DebugVar(name, target, step));
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
            myMenu->setDebugValue(Global::mWindow->getLastFPS(), Global::debug, dbg->debugVars, dbg->debugVarsLine);
            myMenu->setDebugValue(Global::mWindow->getLastFPS(), Global::debug, dbg->debugVars, dbg->debugVarsLine);
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
