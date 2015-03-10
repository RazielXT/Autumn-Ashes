#include "stdafx.h"
#include "OgreGpuCommandBufferFlush.h"
#include "InputListener.h"
#include "Tasks.h"
#include "EventsManager.h"
#include "PagingManager.h"
#include "SceneParser.h"
#include "DefaultObjectContactCallback.h"
#include "TriggerObjectContactCallback.h"
#include "TriggerPlayerContactCallback.h"
#include "Gorilla.h"
#include "lvl_update.h"
#include "Console.h"
#include "PostProcessMgr.h"
#include "GameStateManager.h"
#include "NewtonListener.h"
#include "DebugKeys.h"

using namespace Ogre;

class MainListener : public FrameListener, public OIS::MouseListener, public OIS::KeyListener
{
public:

    void reloadShaders()
    {
        auto it = HighLevelGpuProgramManager::getSingleton().getResourceIterator();

        while (it.hasMoreElements())
        {
            HighLevelGpuProgramPtr gpuProgram = it.getNext();
            gpuProgram->reload();
        }
    }

    ~MainListener()
    {
        delete Global::audioLib;
        delete mEventMgr;
        delete pagingMgr;

        engine->drop();
        mBufferFlush.stop();

        delete postProcMgr;

        delete wMaterials;

    }

    MainListener(OIS::Keyboard *keyboard, OIS::Mouse *mouse,SceneManager * sceneMgr,OgreNewt::World* nWorld, Ogre::Root *mRoot,Ogre::RenderWindow* mWin)
    {
        /*
        W->setMinimumFrameRate(30);
        W->setThreadCount(2);
        W->setUpdateFPS(60,1);
        W->setMultithreadSolverOnSingleIsland(1);
        W->setPlatformArchitecture(2);
        W->setFrictionModel(1);
        W->setSolverModel(1);
        */

        nListener.init(nWorld);

        mEventMgr = new EventsManager();
        pagingMgr = new PagingManager();
        mBufferFlush.start(1);

        engine = irrklang::createIrrKlangDevice();
        engine->setListenerPosition(irrklang::vec3df(0,0,0), irrklang::vec3df(0,0,1));

        mSceneMgr=sceneMgr;
        mWindow=mWin;
        mWorld=nWorld;
        mWorld->setSolverModel(3);
        mCamera=mSceneMgr->getCamera("Camera");
        wMaterials = new WorldMaterials();

        postProcMgr = new PostProcessMgr(mCamera);
        postProcMgr->setToScaryBloom();

        Global::mSceneMgr = mSceneMgr;
        Global::mWorld = mWorld;
        Global::mWindow = mWindow;
        Global::mEventsMgr = mEventMgr;
        Global::soundEngine = engine;
        Global::mPPMgr = postProcMgr;
        Global::mPagingMgr = pagingMgr;

        Global::audioLib = new AudioLibrary(engine);

        gameMgr = new GameStateManager(mCamera, mRoot->getRenderSystem(), wMaterials);
        Global::gameMgr = gameMgr;

        mKeyboard=keyboard;
        mMouse=mouse;
        if (mMouse)
            mMouse->setEventCallback(this);
        if (mKeyboard)
            mKeyboard->setEventCallback(this);

        wMaterials->init(mWorld);
        gameMgr->switchToMainMenu();

        (*Global::globalData)["MatFlag"] = wMaterials->flag_mat;

        dbg.registerInputListening();
    }

    void default_callback(OgreNewt::Body* me, float timeStep, int threadIndex )
    {
        me->addForce(Vector3(0,-6,0));
    }

    bool frameStarted(const FrameEvent& evt)
    {
        float tslf = evt.timeSinceLastFrame;

        postProcMgr->update(tslf);
        pagingMgr->update();
        mEventMgr->update(tslf);

        mKeyboard->capture();
        mMouse->capture();

        gameMgr->update(tslf);

        if(gameMgr->gameState==GAME)
        {
            Global::player->update(tslf);

            Vector3 pos=mCamera->getDerivedPosition();
            Vector3 or=mCamera->getDerivedOrientation()*Vector3(0,0,1);
            engine->setListenerPosition(irrklang::vec3df(pos.x,pos.y,pos.z), irrklang::vec3df(or.x,or.y,or.z));

            nListener.frameStarted(tslf);
        }

        return continueExecution;
    }

    virtual bool keyPressed(const OIS::KeyEvent &arg)
    {
        Global::mEventsMgr->listenersKeyPressed(arg);

        if (gameMgr->gameState == GAME)
            Global::player->pressedKey(arg);

        switch (arg.key)
        {
        case OIS::KC_M:
        {
            if (gameMgr->gameState == GAME)
                makecube();
        }
        break;

        case OIS::KC_G:
        {
            PostProcessMgr* mgr = Global::mPPMgr;
            Ogre::LogManager::getSingleton().getLog("RuntimeEvents.log")->logMessage("PP: " + Ogre::StringConverter::toString(mgr->ColouringShift.x) + "," + Ogre::StringConverter::toString(mgr->ColouringShift.y) + "," + Ogre::StringConverter::toString(mgr->ColouringShift.z), Ogre::LML_NORMAL);

            break;
        }

        case OIS::KC_NUMPAD6:
        {
            reloadShaders();

            break;
        }
        case OIS::KC_ESCAPE:
        {
            gameMgr->escapePressed();

            if (gameMgr->gameState == MENU)
                continueExecution = false;

            break;
        }

        default:
            break;
        }
        return true;
    }

    virtual bool keyReleased(const OIS::KeyEvent &arg)
    {
        if(gameMgr->gameState==GAME)
            Global::player->releasedKey(arg);

        return true;
    }

    virtual bool mouseMoved(const OIS::MouseEvent &e)
    {
        Global::mEventsMgr->listenersMouseMoved(e);

        if (gameMgr->gameState == GAME)
            Global::player->movedMouse(e);
        else
            gameMgr->insideMenuMoved(e.state.X.rel, e.state.Y.rel);


        return true;
    }

    virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
    {

        if(gameMgr->gameState==GAME)
            Global::player->pressedMouse(arg, id);
        else
            continueExecution = gameMgr->insideMenuPressed();

        return true;
    }

    virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
    {
        if(gameMgr->gameState==GAME)
            Global::player->releasedMouse(arg, id);

        return true;
    }

    void makecube()
    {
        Ogre::Vector3 size( 2, 2,2 );
        Ogre::Real mass = 0.3;

        Entity* ent = mSceneMgr->createEntity( "boxEL.mesh" );
        SceneNode* node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
        node->attachObject( ent );
        node->setScale( size );
        ent->setCastShadows(true);
        ent->setMaterialName( "murbob" );
        OgreNewt::ConvexCollisionPtr col = OgreNewt::ConvexCollisionPtr(new OgreNewt::CollisionPrimitives::Box( mWorld, size, 0 ));
        OgreNewt::Body* body = new OgreNewt::Body( mWorld, col );

        Ogre::Vector3 inertia, offset;
        col->calculateInertialMatrix(inertia, offset);

#ifdef OGRENEWT_NO_COLLISION_SHAREDPTR
        delete col;
#endif

        body->setMassMatrix( mass, mass*inertia );
        body->setCenterOfMass(offset);
        body->attachNode( node );
        body->setType(Grabbable);
        body->setCustomForceAndTorqueCallback<MainListener>(&MainListener::default_callback, this);
        body->setPositionOrientation( Global::player->body->getPosition()+Vector3(0,5,0), Ogre::Quaternion::IDENTITY );
    }


private:
    OIS::Keyboard *mKeyboard;
    OIS::Mouse *mMouse;
    irrklang::ISoundEngine* engine;
    SceneManager * mSceneMgr;
    Camera* mCamera;
    OgreNewt::World* mWorld;
    Player* p;
    Ogre::GpuCommandBufferFlush mBufferFlush;
    WorldMaterials* wMaterials;
    //OgreNewt::MaterialID* stoji_mat, *ide_mat, *flag_mat,*playerIgnore_mat,*trig_mat,*action_mat, *selfIgnore_mat;
    Ogre::RenderWindow* mWindow;
    OgreNewt::Body* vytah;
    EventsManager* mEventMgr;
    PagingManager* pagingMgr;
    PostProcessMgr* postProcMgr;
    GameStateManager* gameMgr;
    NewtonListener nListener;
    DebugKeys dbg;
};





/*

OgreNewt::Body* vytah;
void vcallback(OgreNewt::Body* me, float timeStep, int threadIndex )
{
Real h=me->getPosition().y;
if(h>40) {h=40; me->setVelocity(Vector3(0,0,0));}
Real force=(40-h)/1;
me->addForce(Vector3(0,force,0));
}

void vcallback2(OgreNewt::Body* me, float timeStep, int threadIndex )
{
me->setVelocity(Vector3(0,vytah->getVelocity().y*-1,0));
}

Entity* ent = mSceneMgr->createEntity( "vytahmesh", "vytah.mesh" );
OgreNewt::ConvexCollisionPtr col = OgreNewt::ConvexCollisionPtr(new OgreNewt::CollisionPrimitives::ConvexHull(mWorld,ent,1));

vytah = new OgreNewt::Body( mWorld, col );

SceneNode* vnode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
vnode->attachObject( ent );

Vector3 inertia, offset;
col->calculateInertialMatrix(inertia, offset);
#ifdef OGRENEWT_NO_COLLISION_SHAREDPTR
//no longer need the collision shape object
delete col;
#endif
vytah->setMassMatrix( 4, inertia );
vytah->setCenterOfMass(offset);
vytah->setCustomForceAndTorqueCallback<MainListener>(&MainListener::vcallback, this);
//pbody->setContinuousCollisionMode(1);
vytah->setPositionOrientation( Ogre::Vector3(0.0,40,0.0), Ogre::Quaternion::IDENTITY );
//vytah->setLinearDamping(0.1);
vytah->attachNode( vnode );
OgreNewt::UpVector* uv= new OgreNewt::UpVector(vytah, Vector3::UNIT_Y);
OgreNewt::UpVector* uv2= new OgreNewt::UpVector(vytah, Vector3::UNIT_Z);


ent = mSceneMgr->createEntity( "vytahmesh2", "vytah.mesh" );
col = OgreNewt::ConvexCollisionPtr(new OgreNewt::CollisionPrimitives::ConvexHull(mWorld,ent,1));

OgreNewt::Body* vytah2 = new OgreNewt::Body( mWorld, col );

SceneNode* vnode2 = mSceneMgr->getRootSceneNode()->createChildSceneNode();
vnode2->attachObject( ent );

col->calculateInertialMatrix(inertia, offset);
#ifdef OGRENEWT_NO_COLLISION_SHAREDPTR
//no longer need the collision shape object
delete col;
#endif
vytah2->setMassMatrix( 4, inertia );
vytah2->setCenterOfMass(offset);
vytah2->setCustomForceAndTorqueCallback<MainListener>(&MainListener::vcallback2, this);
//pbody->setContinuousCollisionMode(1);
vytah2->setPositionOrientation( Ogre::Vector3(10.0,0.0,0.0), Ogre::Quaternion::IDENTITY );
//vytah->setLinearDamping(0.1);
vytah2->attachNode( vnode2 );
OgreNewt::UpVector* uv12= new OgreNewt::UpVector(vytah2, Vector3::UNIT_Y);
OgreNewt::UpVector* uv22= new OgreNewt::UpVector(vytah2, Vector3::UNIT_Z);
*/