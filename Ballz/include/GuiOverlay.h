#pragma once

#include "Gorilla.h"
#include "ListLoop.h"
#include "DebugKeys.h"
#include "GameUiActions.h"

class GuiSceneEdit;
class GameUi;

class GuiOverlay
{
public:

	GuiOverlay(GameConfig* gameConfig, Ogre::Camera* mCam, Ogre::RenderWindow* mWin, Ogre::RenderSystem* rs);
	~GuiOverlay()
	{
		clear();
	}

	void setMainMenu();
	void setIngameMenu();

	void showParticleDebug();
	void showMaterialDebug();
	void showOptGroupDebug();
	void showLevelDebug();
	void showDetailGeometryDebug();

	void setDebugValue(Ogre::Real value1, std::vector<std::string>& values);
	void updateMainMenu(Ogre::Real time);
	int mainMenuPressed();
	bool pressedKey(const OIS::KeyEvent &arg);
	void mouseMoveUpdate(int x,int y);
	void clearMenu();

	void updateIngame(Ogre::Real time);
	void updateIngameMenu(Ogre::Real time);

	GameUi* gameUi;

private:

	GuiSceneEdit* editUi;

	GameConfig* gConfig;

	void updateOptionsMove(Ogre::Real time);
	void updateLevelsMove(Ogre::Real time);
	void showLevels();
	void closeLevels();
	void showOptions();
	void closeOptions();
	void createMainMenuButtons();
	void createOptionMenuButtons();
	void createIngameMenuButtons();
	void createLevelsMenuButtons();

	void clear();

	enum mMenuEnum { START, OPTIONS, QUIT, RESUME, RESTART };
	enum menusEnum { MAINM, STARTM, OPTIONSM, QUITM, TOSM, FRSM, TOOM, FROM, TOQM, FRQM };
	enum oMenuEnum { RESOLUTION, FULLSCREEN, SHADOWS, SSAO };

	struct resolution
	{
		std::string txt;
		int w;
		int h;
	};

	struct lvlButton
	{
		Gorilla::Rectangle* r;
		Gorilla::Caption* c;
		int id;
		bool unlocked;
		float pos;
	};

	ListLoop<mMenuEnum>* mMenuState;
	ListLoop<mMenuEnum>* gMenuState;
	menusEnum currentMenu;
	oMenuEnum oMenuState;

	ListLoop<Gorilla::Caption*>* cButton;
	ListLoop<Gorilla::Caption*>* cOptionButtonA;
	ListLoop<Gorilla::Caption*>* firstOptionButton;
	ListLoop<Gorilla::Caption*>* firstOptionButtonA;

	ListLoop<resolution>* resolutionsLoop;

	char movingDir;
	bool moving, lvlMoving;
	irrklang::ISoundEngine* soundEngine;
	Ogre::Real atm;
	Ogre::SceneManager * mSceneMgr;
	std::vector<Gorilla::Caption*> mMenuButtons;
	std::vector<Gorilla::Caption*> oMenuButtons;
	std::vector<lvlButton*> lMenuButtons;
	lvlButton* firstLevelButton, *curLvlButton;
	Gorilla::Silverback*    mSilverback;
	Gorilla::Screen*        mScreen, *mScreenLvls, *mScreenIngame;
	Gorilla::Layer*         mLayer, *gameLayer, *mouseLayer, *mLvlsLayer;
	Gorilla::Caption*       fpsCaption;
	Gorilla::Caption*       debugCaption[MAX_DEBUG_LINES];
	Gorilla::Rectangle*     mousePointer;

	bool ingamemenu;

};
