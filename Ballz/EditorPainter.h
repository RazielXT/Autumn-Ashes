#pragma once
#include "stdafx.h"

struct PaitedItem;
class EditorControl;
struct SelectionInfoChange;

struct EditorPainter
{
	EditorPainter(EditorControl* p) : parent(p) {}

	void init();

	void setItem(PaitedItem* item);
	PaitedItem* item = nullptr;

	void mousePressed();

	void mouseReleased();

	void mouseMoved();

	enum PaintMode
	{
		None, Add, Remove
	};

	void setMode(PaintMode mode);

	void handleChangeMessage(SelectionInfoChange* change);

private:

	void setSize(float size);
	void setWeight(float w);

	float size = 1;
	float weight = 1;
	PaintMode mode = None;

	bool applyPaint();
	void applyPaint(Ogre::Vector3 pos);
	Ogre::Vector3 getPaintPos();
	bool painting = false;

	EditorControl* parent;
	Ogre::SceneNode* visual = nullptr;
};