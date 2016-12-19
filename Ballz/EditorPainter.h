#pragma once
#include "stdafx.h"

struct PaitedItem;
struct EditorControl;

struct EditorPainter
{
	EditorPainter(EditorControl* p) : parent(p) {}
	
	void init();

	void setItem(PaitedItem* item);
	PaitedItem* item = nullptr;

	void mousePressed();

	void mouseReleased();

	void mouseMoved();
	
	void setSize(float size);
	void setWeight(float w);

private:

	float size = 1;
	float weight = 1;

	bool applyPaint();
	bool painting = false;

	EditorControl* parent;
	Ogre::SceneNode* visual = nullptr;
};