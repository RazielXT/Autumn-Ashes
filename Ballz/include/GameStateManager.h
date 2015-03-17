#pragma once

#include "stdafx.h"
#include "WorldMaterials.h"


enum GameState { GAME, PAUSE, MENU, CONSOLE, UNDEFINED };

enum StateSwitching
{
    SS_MAINMENU = -3,
    SS_RESTART = -2
};

class GameStateManager
{
public:

    GameStateManager(Ogre::Camera* cam, Ogre::RenderSystem* rs, WorldMaterials* wMaterials);
    ~GameStateManager();

    void switchToMainMenu();
    void switchToLevel(int lvl);
    void restartLevel();
    void switchState(int target, float time = 1.0f);

    void update(float tslf);
    bool insideMenuPressed();
    void insideMenuMoved(int x, int y);
    void escapePressed();

    GameState gameState;
    GuiOverlay* myMenu;

private:


    void updateStateSwitching(float tslf);
    int stateTarget=0;
    bool switchingState = false;
    float switchStateTimer;

    int lastLVL;

    WorldMaterials* wMaterials;
};