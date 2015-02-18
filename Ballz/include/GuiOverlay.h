#ifndef GUIOVR_H
#define GUIOVR_H

#include "Gorilla.h"

enum mMenuEnum {START,OPTIONS,QUIT,RESUME,RESTART};
enum menusEnum {MAINM,STARTM,OPTIONSM,QUITM,TOSM,FRSM,TOOM,FROM,TOQM,FRQM};
enum oMenuEnum {RESOLUTION,FULLSCREEN,SHADOWS,SSAO};

struct MenuLoop
{
    struct MenuLoop* nextState;
    struct MenuLoop* prevState;
    mMenuEnum mState;
};

struct oMenuLoop
{
    struct oMenuLoop* nextState;
    struct oMenuLoop* prevState;
    oMenuEnum mState;
};

struct buttonLoop
{
    struct buttonLoop* nextState;
    struct buttonLoop* prevState;
    Gorilla::Caption* mButton;
};

struct resLoop
{
    struct resLoop* nextRes;
    struct resLoop* prevRes;
    std::string res;
    int w;
    int h;
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
    ~GuiOverlay () {}

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
    void showUseGui(char id);
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

    MenuLoop* mMenuState;
    MenuLoop* gMenuState;
    menusEnum currentMenu;
    oMenuEnum oMenuState;
    buttonLoop* cButton;
    buttonLoop* cOptionButtonA;
    buttonLoop* firstOptionButton;
    buttonLoop* firstOptionButtonA;
    resLoop* resolutionsLoop;
    char movingDir;
    bool moving, lvlMoving;
    Gorilla::Rectangle* ri;
    irrklang::ISoundEngine* engine;
    Ogre::StringVector mFoundResolutions;
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
    Gorilla::Rectangle*      mousePointer;
    struct GameConfig* gConfig;
    bool ingamemenu;
    float infoTextTimer;
    bool shownInfoText;
    bool shownUseGui, wantUseGui;
    char currUseGui;
};


#endif