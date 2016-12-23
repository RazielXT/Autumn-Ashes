#include "stdafx.h"
#include "EditorPainter.h"
#include "PaintedItem.h"
#include "EditorControl.h"
#include "SceneInteraction.h"
#include "GUtils.h"

void EditorPainter::init()
{
	auto e = GUtils::MakeEntity("basicSphere.mesh");
	e->setMaterialName("lowYellow");
	e->setRenderQueueGroup(RenderQueue_BasicTransparent);

	visual = e->getParentSceneNode();
	visual->setVisible(false);
}

void EditorPainter::setItem(PaitedItem* i)
{
	item = i;

	mode = None;
}

void EditorPainter::mousePressed()
{
	painting = item != nullptr;

	mouseMoved();
	//if (!applyPaint())
	//	setMode(None);
}

void EditorPainter::mouseReleased()
{
	if (painting)
	{
		painting = false;

		if (item)
			item->stoppedPainting();
	}
}

void EditorPainter::mouseMoved()
{
	if (item && mode != None)
	{
		auto pos = getPaintPos();

		if (pos == Ogre::Vector3::ZERO)
			return;

		visual->setPosition(pos);

		if (painting)
		{
			applyPaint(pos);
		}
	}
}

void EditorPainter::setMode(PaintMode m)
{
	mode = m;

	if(visual)
		visual->setVisible(mode != None);
}

void EditorPainter::handleChangeMessage(SelectionInfoChange* change)
{
	if (change->change == SelectionInfoChange::Id::PaintSizeChange)
	{
		setSize(*(float*)change->data);
	}
	if (change->change == SelectionInfoChange::Id::PaintWChange)
	{
		setWeight(*(float*)change->data);
	}
	if (change->change == SelectionInfoChange::Id::PaintAdd)
	{
		Global::editor->setPaintMode(EditorPainter::Add);
		Ogre::Vector3 v = *(Ogre::Vector3*)change->data;
		setWeight(v.x);
		setSize(v.y);
	}
	if (change->change == SelectionInfoChange::Id::PaintRemove)
	{
		Global::editor->setPaintMode(EditorPainter::Remove);
		Ogre::Vector3 v = *(Ogre::Vector3*)change->data;
		setWeight(v.x);
		setSize(v.y);
	}
	if (change->change == SelectionInfoChange::Id::PaintOff)
	{
		Global::editor->setPaintMode(EditorPainter::None);
	}
	if (change->change == SelectionInfoChange::Id::PaintFill)
	{
		item->fillPaint(*(float*)change->data);
	}
}

void EditorPainter::setSize(float s)
{
	size = s;

	visual->setScale(Ogre::Vector3(s, s, s));
}

void EditorPainter::setWeight(float w)
{
	weight = w;
}

bool EditorPainter::applyPaint()
{
	auto pos = getPaintPos();

	if (pos != Ogre::Vector3::ZERO)
	{
		applyPaint(pos);
		return true;
	}

	return false;
}

void EditorPainter::applyPaint(Ogre::Vector3 pos)
{
	item->paint(pos, (mode == Remove ? -weight : weight)*Global::tslf, size);
}

Ogre::Vector3 EditorPainter::getPaintPos()
{
	auto cam = Global::camera->cam;
	auto mouseray = SceneInteraction::getMouseRay();
	GUtils::RayInfo rayInfo;

	if (GUtils::getRayFilteredInfo(cam->getDerivedPosition(), cam->getDerivedPosition() + mouseray.getDirection() * 100000, rayInfo, item->getPaintTarget()))
		return rayInfo.pos;
	else
		return Ogre::Vector3::ZERO;
}

