#include "stdafx.h"
#include "GameStateManager.h"
#include "player.h"
#include "levels.h"

SceneParser SceneParser::instance;

GameStateManager::GameStateManager(Ogre::Camera* cam, Ogre::RenderSystem* rs, WorldMaterials* wMaterials)
{
    myMenu = new GuiOverlay(Global::mSceneMgr, cam, Global::mWindow, rs, Global::soundEngine);
    this->wMaterials = wMaterials;
}

void GameStateManager::switchToMainMenu()
{
    if (Global::player)
    {
        delete Global::player;
        Global::player = NULL;
    }

    Global::mWorld->destroyAllBodies();
    Global::mPagingMgr->clear();
    Global::mSceneMgr->clearScene();
    Global::mEventsMgr->clear();
    Global::soundEngine->removeAllSoundSources();
    Global::mPPMgr->resetValues();
    myMenu->setMainMenu();

    gameState = MENU;
    createMenuLevel(wMaterials);
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

    gameState = PLAY;
    Global::mWorld->destroyAllBodies();
    Global::mPagingMgr->clear();
    Global::mSceneMgr->clearScene();
    Global::mEventsMgr->clear();
    Global::soundEngine->removeAllSoundSources();


    Player* p = new Player(wMaterials);
    auto postProcMgr = Global::mPPMgr;
    p->injectPostProcess(&postProcMgr->hurtEffect, &postProcMgr->ivp, &postProcMgr->pvp, &postProcMgr->mbAmount);
    Global::player = p;

    switch (lvl)
    {
    case 1:
        createLevelTuto(wMaterials);
        break;
    case 2:
        createLevel1_1(wMaterials);
        break;
    case 3:
        createLevel2(wMaterials);
        break;
    default:
        switchToMainMenu();
    }
}

void GameStateManager::restartLevel()
{
    switchToLevel(lastLVL);
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
        gameState = PLAY;
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

	Global::mPPMgr->colourOut(Vector3(0, 0, 0), time);
	myMenu->clearMenu();
}

void GameStateManager::updateStateSwitching(float tslf)
{
	if (switchStateTimer > 0)
	{
		switchStateTimer -= tslf;
		return;
	}
	else
	if (!switchingState)
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
    case PLAY:
        myMenu->setDebugValue(Global::mWindow->getLastFPS(), 0);
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
    if (gameState == PLAY)
    {
        myMenu->clearMenu();
        myMenu->setIngameMenu();
        Global::mPPMgr->radialHorizBlurVignette.z = 1.0;
        gameState = PAUSE;
    }
    else if (gameState == PAUSE)
    {
        myMenu->clearMenu();
        Global::mPPMgr->radialHorizBlurVignette.z = 0.0;
        gameState = PLAY;
    }
}
