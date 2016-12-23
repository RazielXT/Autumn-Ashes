#pragma once
#include "Ogre.h"
#include "OgreNewt.h"

struct EditorPainter;

struct PaitedItem
{
	PaitedItem(EditorPainter* p) : painter(p)  {};
	~PaitedItem() {};

	virtual void paint(Ogre::Vector3 pos, float w, float size) = 0;
	virtual void stoppedPainting() = 0;

	virtual void fillPaint(float w) = 0;

	virtual OgreNewt::Body* getPaintTarget() = 0;

protected:

	EditorPainter* painter;
};