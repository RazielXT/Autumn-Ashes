#pragma once
#include "stdafx.h"
#include <OgreFrameListener.h>
#include <Ogre.h>
#include <OIS.h>
#include <list>
#include <vector>

using namespace Ogre;
using namespace std;

class OgreConsole: public Singleton<OgreConsole>
{
public:
    OgreConsole();
    ~OgreConsole();

    void   init(Ogre::SceneManager *mgr);
    void   shutdown();

    void   setVisible(bool visible);
    bool   isVisible()
    {
        return visible;
    }

    void   print(const String &text);

    bool frameStarted(const Ogre::FrameEvent &evt);

    void onKeyPressed(const OIS::KeyEvent &arg);

    void addCommand(const String &command, void (*)(std::vector<String>&));
    void removeCommand(const String &command);

private:
    bool            visible;
    bool            initialized;
    SceneManager   *scene;
    SceneNode      *node;
    OverlayElement *textbox;
    Overlay      *overlay;

    float            height;
    bool            update_overlay;
    size_t               start_line;
    std::list<String>      lines;
    String            prompt;
    String            lastPrompt;
    std::map<String,void (*)(std::vector<String>&)>  commands;
};