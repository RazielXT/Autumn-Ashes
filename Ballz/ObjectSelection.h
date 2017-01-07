#pragma once
#include "stdafx.h"
#include "SelectionGizmo.h"
#include "EditorComm.h"
#include "EditorEntity.h"
#include "EditorGrass.h"
#include "EditorParticle.h"

struct SelectionInfoChange;
class EditorControl;

class ObjectSelection
{
public:

	ObjectSelection(EditorControl* control);

	void init();

	void setMode(SelectionMode mode);

	void setSelectedEntity(Ogre::Entity* ent, bool forceDeselect = true);

	void setSelectedGrass(GrassInfo& grass, bool forceDeselect = true);

	void setSelectedParticle(Ogre::ParticleSystem* ps, bool forceDeselect = true);

	void updateUiSelectedInfo();

	Ogre::Entity* pickMouseRayItem();

	void lootAtSelection();

	void rayPlaceSelection(float offset);

	void uiEditSelection(SelectionInfoChange* change);

	void uiSelectItem(SelectWorldItemData& data);
	void uiGetWorldItemsInfo(GetWorldItemsData& data);

	void removeSelection();

	EditorItem* selected = nullptr;
	EditorGrass selectedGrasses;
	EditorEntity selectedEntities;
	EditorParticle selelectedParticles;

	SelectionGizmo gizmo;

	bool addMode = false;

	EditorControl* parent;
};