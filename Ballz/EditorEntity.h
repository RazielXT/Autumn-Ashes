#pragma once
#include "EditorItem.h"

struct EditorEntity : public EditorItem
{
	virtual void reset();

	virtual void setPosition(Ogre::Vector3&);
	virtual void setScale(Ogre::Vector3&);
	virtual Ogre::Vector3 getPosition();

	virtual std::vector<Ogre::Vector3> getIndividualPositions();
	virtual void setIndividualPositions(std::vector<Ogre::Vector3>&);

	virtual void move(Ogre::Vector3& move);
	virtual void rotate(Ogre::Vector3& axis, Ogre::Radian& angle);
	virtual void addScale(Ogre::Vector3& scale);
	virtual void duplicate();
	virtual void remove();

	virtual void add(Ogre::Entity* ent);
	virtual void select(Ogre::Entity* ent);
	virtual void deselect();
	virtual bool filter(std::string& name);

	std::vector<Ogre::Entity*> selected;

	virtual void sendUiInfoMessage(EditorUiHandler* handler);

private:

	void setEntityPosition(Ogre::Entity*,Ogre::Vector3&);
	void setEntityOrientation(Ogre::Entity* ent, Ogre::Quaternion& q);
};