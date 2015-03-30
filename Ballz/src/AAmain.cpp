#include "stdafx.h"
#include "player.h"
#include "MainListener.h"

using namespace Ogre;

class Application
{
public:
    void go()
    {
        createRoot();
        defineResources();
        setupRenderSystem();
        createRenderWindow();
        initializeResourceGroups();
        setupScene();
        setupInputSystem();
        createFrameListener();
        startRenderLoop();
    }

    ~Application()
    {
        if (mInputManager)
        {
            mInputManager->destroyInputObject(mKeyboard);
            mInputManager->destroyInputObject(mMouse);
            OIS::InputManager::destroyInputSystem(mInputManager);
        }

        if (m_World)
            delete m_World;
        if (mListener)
            delete mListener;
        if (mRoot)
            delete mRoot;
    }

private:
    Root *mRoot = nullptr;
    Ogre::RenderWindow* mWindow = nullptr;
    OIS::Keyboard *mKeyboard = nullptr;
    OIS::Mouse *mMouse = nullptr;
    OIS::InputManager *mInputManager = nullptr;
    MainListener *mListener = nullptr;
    SceneManager *mSceneMgr = nullptr;
    OgreNewt::World* m_World = nullptr;

    void createRoot()
    {
        mRoot = new Root();
    }

    void defineResources()
    {
        String secName, typeName, archName;
        ConfigFile cf;
        cf.load("resources.cfg");

        ConfigFile::SectionIterator seci = cf.getSectionIterator();
        while (seci.hasMoreElements())
        {
            secName = seci.peekNextKey();
            ConfigFile::SettingsMultiMap *settings = seci.getNext();
            ConfigFile::SettingsMultiMap::iterator i;
            for (i = settings->begin(); i != settings->end(); ++i)
            {
                typeName = i->first;
                archName = i->second;
                ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
            }
        }
    }


    void setupRenderSystem()
    {

        const Ogre::RenderSystemList& lRenderSystemList = mRoot->getAvailableRenderers();
        Ogre::RenderSystem *lRenderSystem = lRenderSystemList[0];

        mRoot->setRenderSystem(lRenderSystem);
    }

    void createRenderWindow()
    {
        mRoot->initialise(false);

        {
            Ogre::String lWindowTitle = "Ballz";

            Ogre::ConfigFile cfgFile;
            cfgFile.loadDirect("config.ini");
            std::string wString = cfgFile.getSetting("width", "renderer");
            std::string hString = cfgFile.getSetting("height", "renderer");
            std::string fsString = cfgFile.getSetting("fullscreen", "renderer");
            int lSizeX = Ogre::StringConverter::parseInt(wString);
            int lSizeY = Ogre::StringConverter::parseInt(hString);
            bool lFullscreen = Ogre::StringConverter::parseBool(fsString);

            Ogre::NameValuePairList lParams;
            // we use our own FXAA
            lParams["FSAA"] = "0";
            lParams["vsync"] = "false";

            lParams["useNVPerfHUD"] = "true";

            mWindow = mRoot->createRenderWindow(lWindowTitle, lSizeX, lSizeY, lFullscreen, &lParams);
        }
    }

    void initializeResourceGroups()
    {
        TextureManager::getSingleton().setDefaultNumMipmaps(5);
        ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
    }

    void setupScene();


    void setupInputSystem()
    {
        size_t windowHnd = 0;
        std::ostringstream windowHndStr;
        OIS::ParamList pl;
        RenderWindow *win = mWindow;

        win->getCustomAttribute("WINDOW", &windowHnd);
        windowHndStr << windowHnd;
        pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
        mInputManager = OIS::InputManager::createInputSystem(pl);

        try
        {
            mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject(OIS::OISKeyboard, true));
            mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject(OIS::OISMouse, true));
            //mJoy = static_cast<OIS::JoyStick*>(mInputManager->createInputObject(OIS::OISJoyStick, false));
        }
        catch (const OIS::Exception &e)
        {
            throw Exception(42, e.eText, "Application::setupInputSystem");
        }
    }

    void createFrameListener()
    {
        mListener = new MainListener(mKeyboard, mMouse, mSceneMgr,m_World,mRoot,mWindow);
        mRoot->addFrameListener(mListener);
    }

    void startRenderLoop()
    {
        mRoot->startRendering();
    }
};

#if OGRE_PLATFORM == PLATFORM_WIN32 || OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"

INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
int main(int argc, char **argv)
#endif
{
    try
    {
        Application app;
        app.go();
    }
    catch(Exception& e)
    {
#if OGRE_PLATFORM == PLATFORM_WIN32 || OGRE_PLATFORM == OGRE_PLATFORM_WIN32
        MessageBoxA(NULL, e.getFullDescription().c_str(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
        fprintf(stderr, "An exception has occurred: %s\n",
                e.getFullDescription().c_str());
#endif
    }

    return 0;
}

#include "start.h"