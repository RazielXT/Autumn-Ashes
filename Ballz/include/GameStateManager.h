#pragma once

#include "stdafx.h"
#include "WorldMaterials.h"


enum GameState { PLAY, PAUSE, MENU, CONSOLE, UNDEFINED };

class GameStateManager
{
public:

    GameStateManager(Ogre::Camera* cam, Ogre::RenderSystem* rs, WorldMaterials* wMaterials);
    ~GameStateManager();

    void switchToMainMenu();
    void switchToLevel(int lvl);
    void restartLevel();

    void update(float tslf);
    bool insideMenuPressed();
    void insideMenuMoved(int x, int y);
    void escapePressed();

    GameState gameState;

private:

    int lastLVL;
    GuiOverlay* myMenu;
    WorldMaterials* wMaterials;
};