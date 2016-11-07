#include "stdafx.h"
#include "EditorCamera.h"
#include "Player.h"

EditorCamera::EditorCamera()
{
	executionState = GAME;
}

void EditorCamera::enable()
{
	registerInputListening();

	if (!camNode)
	{
		Global::mEventsMgr->addCachedTask(this);
		camNode = Global::mSceneMgr->getRootSceneNode()->createChildSceneNode(Global::player->camPosition, Global::player->pCamera->getOrientation());
		camNode->attachObject(Global::player->pCamera->detachCamera());
	}

	front = back = left = right = shift = space = strafe = false;
}

void EditorCamera::disable()
{
	Global::player->pCamera->attachCamera();
	unregisterInputListening();
	Global::player->setPosition(camNode->getPosition());
	Global::mSceneMgr->destroySceneNode(camNode);
	camNode = nullptr;
}

void EditorCamera::setStrafeMode(bool enable)
{
	strafe = enable;
}

void EditorCamera::pressedKey(const OIS::KeyEvent &arg)
{
	switch (arg.key)
	{
	case OIS::KC_A:
		left = true;
		break;
	case OIS::KC_D:
		right = true;
		break;
	case OIS::KC_W:
		front = true;
		break;
	case OIS::KC_S:
		back = true;
		break;
	case OIS::KC_LSHIFT:
		shift = true;
		break;
	case OIS::KC_SPACE:
		space = true;
		break;
	}
}

void EditorCamera::releasedKey(const OIS::KeyEvent &arg)
{
	switch (arg.key)
	{
	case OIS::KC_A:
		left = false;
		break;
	case OIS::KC_D:
		right = false;
		break;
	case OIS::KC_W:
		front = false;
		break;
	case OIS::KC_S:
		back = false;
		break;
	case OIS::KC_LSHIFT:
		shift = false;
		break;
	case OIS::KC_SPACE:
		space = false;
		break;
	}
}

void EditorCamera::movedMouse(const OIS::MouseEvent &evt)
{
	if (strafe)
	{
		int speed = shift ? 400 : 50;
		Ogre::Vector3 move(-evt.state.X.rel, -evt.state.Y.rel, 0);
		move = camNode->getOrientation()*move;
		move *= speed;

		camNode->setPosition(camNode->getPosition() + move);
	}
	else
	{
		camNode->yaw(Ogre::Degree(-evt.state.X.rel * 0.15f), Ogre::Node::TS_WORLD);
		camNode->pitch(Ogre::Degree(-evt.state.Y.rel * 0.15f), Ogre::Node::TS_LOCAL);
	}
}

void EditorCamera::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	if (id == OIS::MB_Middle)
		strafe = true;
}

void EditorCamera::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	if (id == OIS::MB_Middle)
		strafe = false;
}

bool EditorCamera::update(float tslf)
{
	if (!camNode)
		return false;

	int speed = shift ? 400 : 50;
	Ogre::Vector3 direction;

	if (left) direction += Ogre::Vector3(-1, 0, 0);
	if (right) direction += Ogre::Vector3(1, 0, 0);
	if (front) direction += Ogre::Vector3(0, 0, -1);
	if (back) direction += Ogre::Vector3(0, 0, 1);
	if (space) direction += Ogre::Vector3(0, 1, 0);

	direction = camNode->getOrientation()*direction;
	direction.normalise();
	direction *= speed * tslf;

	if(!strafe)
		camNode->setPosition(camNode->getPosition() + direction);

	return true;
}

