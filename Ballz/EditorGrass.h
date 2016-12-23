#pragma once
#include "EditorItem.h"
#include "PaintedItem.h"
#include "PagedGeometry.h"
#include "GeometryManager.h"

struct EditorPainter;

struct EditorGrass : public EditorItem, public PaitedItem
{
	EditorGrass(EditorPainter* painter);

	virtual void reset();

	virtual void setPosition(Ogre::Vector3&);
	virtual void setScale(Ogre::Vector3&);
	virtual Ogre::Vector3 getPosition();

	virtual std::vector<Ogre::Vector3> getIndividualPositions();
	virtual void setIndividualPositions(std::vector<Ogre::Vector3>&);

	virtual void editMouseReleased(SelectionMode mode);
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
	virtual void handleSelectionMessage(SelectionInfoChange*);

	virtual void paint(Ogre::Vector3 pos, float w, float size);
	virtual void stoppedPainting();
	virtual OgreNewt::Body* getPaintTarget();
	virtual void fillPaint(float w);

private:

	Forests::TBounds scaleBounds(Forests::TBounds& b, Ogre::Vector3& scale);
	void updateNode();
	virtual Ogre::AxisAlignedBox getVisualBounds();

	Ogre::SceneNode* boundsNode = nullptr;
	Ogre::Vector3 moveOffset;
	void setGrassPosition(GrassInfo&, Ogre::Vector3&);



	Ogre::Vector3 scaleOffset;
	Ogre::Vector3 scaleNodeOrigin;

};