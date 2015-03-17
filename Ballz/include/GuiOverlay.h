#ifndef GUIOVR_H
#define GUIOVR_H

#include "Gorilla.h"
#include "ListLoop.h"

enum mMenuEnum {START,OPTIONS,QUIT,RESUME,RESTART};
enum menusEnum {MAINM,STARTM,OPTIONSM,QUITM,TOSM,FRSM,TOOM,FROM,TOQM,FRQM};
enum oMenuEnum {RESOLUTION,FULLSCREEN,SHADOWS,SSAO};

struct resolution
{
    std::string res;
    int w;
    int h;
};

enum UiInfo
{
    Ui_Pickup,
    Ui_Use,
    Ui_Climb,
    Ui_Target
};

struct lvlButton
{
    lvlButton(Gorilla::Rectangle* r, int id, bool unlocked, float pos)
    {
        this->r = r;
        this->id = id;
        this->unlocked = unlocked;
        this->pos = pos;
    }

    float pos;
    Gorilla::Rectangle* r;
    int id;
    bool unlocked;
};

struct GameConfig
{
    int width;
    int height;
    int shadow;
    bool ssao;
    bool fs;
};



class GuiOverlay
{
public:

    GuiOverlay(Ogre::SceneManager * sceneMgr, Ogre::Camera* mCam,Ogre::RenderWindow* mWin,Ogre::RenderSystem* rs,irrklang::ISoundEngine* eng);
    ~GuiOverlay()
    {
        clear();
    }

    void setMainMenu();
    void setIngameMenu();

    void showDebug(bool show);
    void setDebugValue(Ogre::Real value1, Ogre::Real value2);
    void updateMainMenu(Ogre::Real time);
    int mainMenuPressed();
    int pressedKey(const OIS::KeyEvent &arg);
    void mouseMoveUpdate(int x,int y);
    void clearMenu();

    void showIngameText(Ogre::String text);
    void showUseGui(UiInfo id);
    void updateIngame(Ogre::Real time);
    void updateIngameMenu(Ogre::Real time);

private:

    void updateOptionsMove(Ogre::Real time);
    void updateLevelsMove(Ogre::Real time);
    void saveCfg();
    void showLevels();
    void closeLevels();
    void showOptions();
    void closeOptions();
    void createMainMenuButtons();
    void createOptionMenuButtons();
    void createIngameMenuButtons();
    void createLevelsMenuButtons();

    void clear();

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
    irrklang::ISoundEngine* engine;
    Ogre::Real atm;
    Ogre::SceneManager * mSceneMgr;
    std::vector<Gorilla::Caption*> mMenuButtons;
    std::vector<Gorilla::Caption*> oMenuButtons;
    std::vector<lvlButton*> lMenuButtons;
    lvlButton* firstLevelButton, *curLvlButton;
    Gorilla::Silverback*    mSilverback;
    Gorilla::Screen*        mScreen, *mScreenLvls, *mScreenIngame;
    Gorilla::Layer*         mLayer, *mouseLayer, *mLvlsLayer;
    Gorilla::Caption*       fpsCaption;
    Gorilla::Caption*       infoTextCaption;
    Gorilla::Caption*       useTextCaption;
    Gorilla::Caption*       debugCaption;
    Gorilla::Rectangle*     mousePointer;
    GameConfig gConfig;
    bool ingamemenu;
    float infoTextTimer;
    bool shownInfoText;
    bool shownUseGui, wantUseGui;
    char currUseGui;
};


#endif