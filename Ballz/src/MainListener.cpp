#include "stdafx.h"
#include "MainListener.h"
#include "OgreGpuCommandBufferFlush.h"
#include "GameStateManager.h"
#include "Tasks.h"
#include "GeometryManager.h"
#include "GameScene.h"
#include "DefaultObjectContactCallback.h"
#include "TriggerObjectContactCallback.h"
#include "TriggerPlayerContactCallback.h"
#include "Gorilla.h"
#include "lvl_update.h"
#include "PostProcessMgr.h"
#include "NewtonListener.h"
#include "DebugKeys.h"
#include "Player.h"
#include "GUtils.h"
#include "win32\Win32Mouse.h"

using namespace Ogre;

MainListener::~MainListener()
{
	delete mEventMgr;
	delete geometryMgr;
	delete gameMgr;

	mBufferFlush.stop();

	delete postProcMgr;
}

MainListener::MainListener(OIS::Keyboard *keyboard, OIS::Mouse *mouse, SceneManager * sceneMgr, OgreNewt::World* nWorld, Ogre::Root *mRoot, Ogre::RenderWindow* mWin, size_t windowHnd) : editor(mouse)
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

	hwnd = windowHnd;
	nListener.init(nWorld);

	mEventMgr = new EventsManager();
	geometryMgr = new GeometryManager();
	mBufferFlush.start(1);

	mSceneMgr = sceneMgr;
	mWindow = mWin;
	mWorld = nWorld;
	mWorld->setSolverModel(3);
	mCamera = mSceneMgr->getCamera("Camera");

	Global::mSceneMgr = mSceneMgr;
	Global::mWorld = mWorld;
	Global::mWindow = mWindow;
	Global::mEventsMgr = mEventMgr;

	gameMgr = new GameStateManager(mCamera, mRoot->getRenderSystem());
	Global::gameMgr = gameMgr;
	Global::gameMgr->geometryMgr = geometryMgr;

	postProcMgr = new PostProcessMgr(mCamera);
	Global::mPPMgr = postProcMgr;

	mKeyboard = keyboard;
	mMouse = mouse;
	if (mMouse)
		mMouse->setEventCallback(this);
	if (mKeyboard)
		mKeyboard->setEventCallback(this);

	gameMgr->switchToMainMenu();
}

bool MainListener::frameStarted(const FrameEvent& evt)
{
	float tslf = std::min(0.1f, evt.timeSinceLastFrame);

	postProcMgr->update(tslf);
	geometryMgr->update();
	mEventMgr->update(tslf);

	mKeyboard->capture();
	mMouse->capture();

	gameMgr->update(tslf);

	if (gameMgr->gameState == GAME)
	{
		if(!editor.active)
			Global::player->update(tslf);
		nListener.frameStarted(tslf);
	}

	return continueExecution;
}

bool MainListener::keyPressed(const OIS::KeyEvent &arg)
{
	Global::mEventsMgr->listenersKeyPressed(arg);

	if (gameMgr->gameState == GAME && !editor.active)
		Global::player->pressedKey(arg);

	switch (arg.key)
	{

	case OIS::KC_K:
		editor.setActive(!editor.active);
		break;

	case OIS::KC_ESCAPE:
	{
		gameMgr->escapePressed();

		if (gameMgr->gameState == MENU)
		{
			continueExecution = false;
			if (editor.active)
				SetActiveWindow(NULL);
		}

		break;
	}

	default:
		break;
	}
	return true;
}

bool MainListener::keyReleased(const OIS::KeyEvent &arg)
{
	Global::mEventsMgr->listenersKeyReleased(arg);

	if (gameMgr->gameState == GAME && !editor.active)
		Global::player->releasedKey(arg);

	return true;
}

static int x = 0;
static int y = 0;

bool MainListener::mouseMoved(const OIS::MouseEvent &evt)
{
	POINT point;
	GetCursorPos(&point);
	ScreenToClient((HWND)hwnd, &point);

	OIS::MouseState state;
	state.X.rel = point.x - x;
	state.Y.rel = point.y - y;

	state.X.abs = x = point.x;
	state.Y.abs = y = point.y;

	OIS::MouseEvent e(nullptr, editor.editMode ? state : evt.state);

	Global::mEventsMgr->listenersMouseMoved(e);

	if (gameMgr->gameState == GAME && !editor.active)
		Global::player->movedMouse(e);
	else if (gameMgr->gameState == PAUSE || gameMgr->gameState == MENU)
		gameMgr->insideMenuMoved(x, y);

	return true;
}

bool MainListener::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	if (editor.active)
	{
		if (id == OIS::MB_Right)
			editor.editMode ? editor.setVievMode() : editor.setEditMode();
		if (id == OIS::MB_Left && editor.editMode)
		{
			auto cam = Global::player->pCamera->camera;
			auto mouseray = cam->getCameraToViewportRay(x / (float)mWindow->getWidth(), y / (float)mWindow->getHeight());
			GUtils::RayInfo rayInfo;
			if (GUtils::getRayInfo(cam->getDerivedPosition(), cam->getDerivedPosition() + mouseray.getDirection() * 100000, rayInfo))
				GUtils::MakeEntity("aspenLeafs.mesh", rayInfo.pos);
		}
	}
	else if (gameMgr->gameState == GAME)
		Global::player->pressedMouse(arg, id);
	else if (gameMgr->gameState == PAUSE || gameMgr->gameState == MENU)
		continueExecution = gameMgr->insideMenuPressed();

	return true;
}

bool MainListener::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	if (gameMgr->gameState == GAME && !editor.active)
		Global::player->releasedMouse(arg, id);

	return true;
}



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