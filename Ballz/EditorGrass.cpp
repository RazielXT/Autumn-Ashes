#include "stdafx.h"
#include "EditorGrass.h"
#include "EditorUiHandler.h"
#include "EditorPainter.h"
#include "GUtils.h"
#include "GameStateManager.h"
#include "GrassHeightFunction.h"
#include "EditorControl.h"


EditorGrass::EditorGrass(EditorPainter* painter) : PaitedItem(painter)
{
	moveOffset = Ogre::Vector3::ZERO;
	scaleOffset = Ogre::Vector3(1, 1, 1);
}

void EditorGrass::reset()
{
	selected.clear();
	boundsNode = nullptr;
}

void EditorGrass::setPosition(Ogre::Vector3& pos)
{
	auto translate = getPosition();
	translate = pos - translate;

	for (auto& e : selected)
	{
		auto ePos = e.pg->getSceneNode()->getPosition();
		setGrassPosition(e, ePos + translate);
	}
}

void EditorGrass::setScale(Ogre::Vector3& scale)
{
	for (auto& e : selected)
	{
		//e->getParentSceneNode()->setScale(scale);
	}
}

Ogre::Vector3 EditorGrass::getPosition()
{
	return getVisualBounds().getCenter() + moveOffset;
}

std::vector<Ogre::Vector3> EditorGrass::getIndividualPositions()
{
	std::vector<Ogre::Vector3> out;

	for (auto& e : selected)
	{
		out.push_back(e.pg->getSceneNode()->getPosition());
	}

	return out;
}

void EditorGrass::setIndividualPositions(std::vector<Ogre::Vector3>& in)
{
	if (selected.size() == in.size())
		for (size_t i = 0; i < selected.size(); i++)
		{
			selected[i].pg->getSceneNode()->setPosition(in[i]);
		}
}

Forests::TBounds EditorGrass::scaleBounds(Forests::TBounds& b, Ogre::Vector3& scale)
{
	auto bounds = Ogre::AxisAlignedBox(b.left, 0, b.top, b.right, 0, b.bottom);
	auto center = bounds.getCenter();
	auto size = bounds.getHalfSize()*scale;

	return Forests::TBounds(center.x - size.x, center.z - size.z, center.x + size.x, center.z + size.z);
}

void EditorGrass::editMouseReleased(SelectionMode mode)
{
	if (boundsNode)
		boundsNode->setVisible(false);

	if(mode == SelectionMode::Move)
		for (auto& g : selected)
		{
			auto b = g.pg->getBounds();
			b.bottom += moveOffset.z;
			b.top += moveOffset.z;
			b.right += moveOffset.x;
			b.left += moveOffset.x;

			auto lvls = g.pg->getDetailLevels();
			auto tr = lvls.front()->getTransition();
			auto len = lvls.front()->getFarRange();

			g.pg->removeDetailLevels();
			g.pg->setBounds(b);
			g.pg->addDetailLevel<Forests::GrassPage>((float)len, (float)tr);

			if(g.bake.layer)
			{
				auto bounds = g.bake.layer->mapBounds;
				g.bake.layer->setMapBounds(Forests::TBounds(bounds.left + moveOffset.x, bounds.top + moveOffset.z, bounds.right + moveOffset.x, bounds.bottom + moveOffset.z));

				g.bake.pos += moveOffset;
				Global::gameMgr->geometryMgr->bakeLight(g.bake);

				if (g.density.grid.data)
				{
					g.density.relocate(g);
				}
			}

			g.pg->reloadGeometry();

			g.terrainQuery->offset_maxY += moveOffset.y;
			g.terrainQuery->offset_minY += moveOffset.y;
		}

	moveOffset = Ogre::Vector3::ZERO;

	if (mode == SelectionMode::Scale)
		for (auto& g : selected)
		{
			auto lvls = g.pg->getDetailLevels();
			auto tr = lvls.front()->getTransition();
			auto len = lvls.front()->getFarRange();

			g.pg->removeDetailLevels();

			auto b = g.pg->getBounds();
			auto newBounds = scaleBounds(b, scaleOffset);
			auto size = std::max({ newBounds.right - newBounds.left, newBounds.bottom - newBounds.top, 25.f });
			g.pg->setBounds(Forests::TBounds(newBounds.left, newBounds.top, newBounds.left + size, newBounds.top + size));
			g.pg->addDetailLevel<Forests::GrassPage>((float)len, (float)tr);

			if (g.bake.layer)
			{
				auto bounds = scaleBounds(g.bake.layer->mapBounds, scaleOffset);
				g.bake.layer->setMapBounds(bounds);
				g.bake.size = Ogre::Vector2(bounds.right - bounds.left, bounds.bottom - bounds.top);

				Global::gameMgr->geometryMgr->bakeLight(g.bake);

				if(g.density.grid.data)
					g.density.resize(g);
			}

			g.pg->reloadGeometry();

			auto yCenter = g.terrainQuery->offset_maxY - g.terrainQuery->offset_minY;
			g.terrainQuery->offset_maxY = yCenter + (g.terrainQuery->offset_maxY - yCenter)*scaleOffset.y;
			g.terrainQuery->offset_minY = yCenter - (yCenter - g.terrainQuery->offset_minY)*scaleOffset.y;
		}

	scaleOffset = Ogre::Vector3(1, 1, 1);
}

void EditorGrass::move(Ogre::Vector3& move)
{
	boundsNode->setVisible(true);

	moveOffset += move;
	updateNode();
}

void EditorGrass::rotate(Ogre::Vector3&, Ogre::Radian&)
{
}

void EditorGrass::addScale(Ogre::Vector3& scale)
{
	boundsNode->setVisible(true);

	scaleOffset += scale*0.1f;
	updateNode();
}

void EditorGrass::duplicate()
{
	/*std::vector<Ogre::Entity*> oldSelected = selected;

	deselect();

	for (auto& e : oldSelected)
	{
		auto ent = e->clone(getCloneName(e->getName()));
		auto newRoot = Global::sceneMgr->getRootSceneNode()->createChildSceneNode(e->getParentSceneNode()->getPosition(), e->getParentSceneNode()->getOrientation());
		newRoot->setScale(e->getParentSceneNode()->getScale());
		newRoot->attachObject(ent);

		add(ent);
	}*/
}

void EditorGrass::remove()
{
	/*for (auto& e : selected)
	{
		auto n = e->getParentSceneNode();
		e->detachFromParent();
		Global::sceneMgr->destroyEntity(e);
		Global::sceneMgr->destroySceneNode(n);
	}

	selected.clear();*/
}

void EditorGrass::add(GrassInfo& grass)
{
	selected.push_back(grass);

	if (!boundsNode)
	{
		auto e = GUtils::MakeEntity("basicBox.mesh", getPosition());
		e->setMaterialName("lowYellow");
		e->setRenderQueueGroup(RENDER_QUEUE_MAIN + 1);
		boundsNode = e->getParentSceneNode();
	}

	boundsNode->setVisible(false);
	updateNode();

	boundsNode->showBoundingBox(true);

	painter->setItem(this);
}

void EditorGrass::select(GrassInfo& grass)
{
	deselect();
	add(grass);
}

void EditorGrass::deselect()
{
	boundsNode->setVisible(false);
	boundsNode->showBoundingBox(false);
	selected.clear();
	painter->setItem(nullptr);
}

bool EditorGrass::filter(std::string& name)
{
	auto oldSelected = selected;

	deselect();

	for (auto& e : oldSelected)
	{
		if (e.name == name)
			add(e);
	}

	return !selected.empty();
}

void EditorGrass::sendUiInfoMessage(EditorUiHandler* handler)
{
	UiMessage msg;
	msg.id = UiMessageId::ShowSelectionInfo;
	SelectionInfo info;

	if (selected.size() == 1)
	{
		info.name = selected[0].name;
		info.pos = selected[0].pg->getSceneNode()->getPosition();
	}
	else
	{
		info.name = std::to_string(selected.size()) + L" objects";
		for (auto& e : selected)
			info.names.push_back(e.name);

		info.pos = getPosition();
	}

	info.scale = Ogre::Vector3(1, 1, 1);
	info.type = ItemType::Grass;
	info.usePaint = true;

	GrassSelectionInfo grassInfo;
	grassInfo.density = selected[0].bake.layer->density;
	grassInfo.minHSize = selected[0].bake.layer->minHeight;
	grassInfo.maxHSize = selected[0].bake.layer->maxHeight;
	grassInfo.minWSize = selected[0].bake.layer->minWidth;
	grassInfo.maxWSize = selected[0].bake.layer->maxWidth;
	grassInfo.preserveMask = false;
	info.typeData = &grassInfo;

	msg.data = &info;

	handler->sendMsg(msg);
}

void EditorGrass::handleSelectionMessage(SelectionInfoChange* change)
{
	if (change->change == SelectionInfoChange::Id::GrassDensity)
	{
		selected[0].bake.layer->setDensity(*(float*)change->data);
		selected[0].pg->reloadGeometry();
	}
	if (change->change == SelectionInfoChange::Id::GrassSize)
	{
		Ogre::Vector4& v = *(Ogre::Vector4*)change->data;
		selected[0].bake.layer->setMinimumSize(v.z, v.x);
		selected[0].bake.layer->setMaximumSize(v.w, v.y);
		selected[0].pg->reloadGeometry();
	}
	else if (change->change == SelectionInfoChange::Id::GrassPaintPreserve)
	{
		selected[0].density.preserveOriginal(*((float*)change->data) != 0);
		selected[0].density.apply(selected[0]);
	}
}

void EditorGrass::updateNode()
{
	auto b = getVisualBounds();
	boundsNode->setScale(b.getSize()*scaleOffset);
	boundsNode->setPosition(b.getCenter() + moveOffset);
}

Ogre::AxisAlignedBox EditorGrass::getVisualBounds()
{
	Ogre::AxisAlignedBox box;

	for (auto& g : selected)
	{
		auto b = g.bake.layer ? g.bake.layer->mapBounds : g.pg->getBounds();

		Ogre::AxisAlignedBox subbox(b.left, g.terrainQuery->offset_minY, b.top, b.right, g.terrainQuery->offset_maxY, b.bottom);

		box = subbox;// .merge(subbox);
	}

	return box;
}

void EditorGrass::setGrassPosition(GrassInfo& pg, Ogre::Vector3& pos)
{
	pg.pg->getSceneNode()->setPosition(pos);
}

void EditorGrass::fillPaint(float w)
{
	if (selected[0].density.empty())
	{
		selected[0].density.resize(selected[0]);
	}

	selected[0].density.fill(w);
	selected[0].density.apply(selected[0]);
}

OgreNewt::Body* EditorGrass::getPaintTarget()
{
	return selected[0].terrainQuery->bodyTarget;
}

float paintReloadTimer = 0;
const float repaintTimePeriod = 0.25f;

void EditorGrass::paint(Ogre::Vector3 pos, float w, float size)
{
	if (selected[0].density.empty())
	{
		selected[0].density.resize(selected[0]);
	}

	selected[0].density.paint(pos.x, pos.z, w, size);

	paintReloadTimer += Global::tslf;
	if (paintReloadTimer > repaintTimePeriod)
	{
		selected[0].density.apply(selected[0]);
		paintReloadTimer = 0;
	}
}

void EditorGrass::stoppedPainting()
{
	paintReloadTimer = 0;

	if (!selected.empty())
	{
		selected[0].density.apply(selected[0]);
	}
}
