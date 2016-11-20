#pragma once
#include "stdafx.h"
#include "EditorComm.h"

struct EditorUiHandler;

struct EditorItem
{
	virtual void setPosition(Ogre::Vector3&) = 0;
	virtual void setScale(Ogre::Vector3&) = 0;

	virtual Ogre::Vector3 getPosition() = 0;

	virtual void move(Ogre::Vector3& move) = 0;
	virtual void rotate(Ogre::Vector3& axis, Ogre::Radian& angle) = 0;
	virtual void addScale(Ogre::Vector3& scale) = 0;

	virtual void deselect() = 0;
	virtual void duplicate() = 0;
	virtual void remove() = 0;

	virtual void sendUiInfoMessage(EditorUiHandler*) = 0;
};