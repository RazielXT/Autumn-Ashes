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

    /*if (lvl < 4)
    {
    	Vector3 pos = p->getBody()->getOgreNode()->_getDerivedPosition();
    	Vector3 or = mCamera->getDerivedOrientation()*Vector3(0, 0, 1);
    	engine->setListenerPosition(irrklang::vec3df(pos.x, pos.y, pos.z), irrklang::vec3df(or.x, or.y, or.z));
    }*/
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
        switchToLevel(i);
    }
    if (i == -3)
    {
        if (gameState == PAUSE)
            switchToMainMenu();
        else
            continueExecution = false;
    }
    if (i == -1)
    {
        Global::mPPMgr->radialHorizBlurVignette.z = 0.0;
        gameState = PLAY;
    }
    if (i == -2)
    {
        gameState = PLAY;
        myMenu->clearMenu();
        restartLevel();
    }

    return continueExecution;
}

void GameStateManager::insideMenuMoved(int x, int y)
{
    myMenu->mouseMoveUpdate(x,y);
}

void GameStateManager::update(float tslf)
{
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
