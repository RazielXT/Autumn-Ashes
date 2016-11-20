#pragma once
#include "EditorItem.h"

struct EditorEntity : public EditorItem
{
	virtual void setPosition(Ogre::Vector3&);
	virtual void setScale(Ogre::Vector3&);
	virtual Ogre::Vector3 getPosition();

	virtual void move(Ogre::Vector3& move);
	virtual void rotate(Ogre::Vector3& axis, Ogre::Radian& angle);
	virtual void addScale(Ogre::Vector3& scale);
	virtual void duplicate();
	virtual void remove();

	virtual void add(Ogre::Entity* ent);
	virtual void select(Ogre::Entity* ent);
	virtual void deselect();

	std::vector<Ogre::Entity*> selected;

	virtual void sendUiInfoMessage(EditorUiHandler* handler);
};