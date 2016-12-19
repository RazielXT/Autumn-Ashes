#include "stdafx.h"
#include "EditorPainter.h"
#include "PaintedItem.h"
#include "EditorControl.h"
#include "SceneInteraction.h"
#include "GUtils.h"

void EditorPainter::init()
{
	visual = GUtils::MakeEntity("basicSphere.mesh")->getParentSceneNode();
}

void EditorPainter::setItem(PaitedItem* i)
{
	item = i;

	parent->mode = item ? EditorControl::EditorMode::Paint : EditorControl::EditorMode::Select;
}

void EditorPainter::mousePressed()
{
	painting = item != nullptr;

	if (!applyPaint())
		parent->mode = EditorControl::EditorMode::Select;
}

void EditorPainter::mouseReleased()
{
	painting = false;
}

void EditorPainter::mouseMoved()
{
	if (painting && item)
	{
		applyPaint();
	}
}

void EditorPainter::setSize(float s)
{
	size = s;
}

void EditorPainter::setWeight(float w)
{
	weight = w;
}

bool EditorPainter::applyPaint()
{
	auto cam = Global::camera->cam;
	auto mouseray = SceneInteraction::getMouseRay();
	GUtils::RayInfo rayInfo;

	if (GUtils::getRayFilteredInfo(cam->getDerivedPosition(), cam->getDerivedPosition() + mouseray.getDirection() * 100000, rayInfo, item->getPaintTarget()))
	{
		item->paint(rayInfo.pos, weight, size);
		return true;
	}

	return false;
}

