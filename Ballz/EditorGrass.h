#pragma once
#include "EditorItem.h"
#include "PagedGeometry.h"

struct GrassInfo
{
	Forests::PagedGeometry* pg;
	std::string name;
};

struct EditorGrass : public EditorItem
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

	virtual void add(GrassInfo& grass);
	virtual void select(GrassInfo& grass);
	virtual void deselect();
	virtual bool filter(std::string& name);

	std::vector<GrassInfo> selected;

	virtual void sendUiInfoMessage(EditorUiHandler* handler);

private:

	void setGrassPosition(GrassInfo&, Ogre::Vector3&);
};