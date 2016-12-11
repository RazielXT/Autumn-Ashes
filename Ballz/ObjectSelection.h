#pragma once
#include "stdafx.h"
#include "SelectionGizmo.h"
#include "EditorComm.h"
#include "EditorItem.h"
#include "EditorEntity.h"
#include "EditorGrass.h"

struct SelectionInfoChange;
class EditorControl;

class ObjectSelection
{
public:

	EditorControl* parent;

	void init(EditorControl* parent);

	void setMode(SelectionMode mode);

	void setSelectedEntity(Ogre::Entity* ent, bool forceDeselect = true);

	void setSelectedGrass(GrassInfo& grass, bool forceDeselect = true);

	void updateUiSelectedInfo();

	Ogre::Entity* pickMouseRayItem();

	void lootAtSelection();

	void rayPlaceSelection(float offset);

	void uiEditSelection(SelectionInfoChange* change);

	void uiSelectItem(SelectWorldItemData& data);
	void removeSelection();
	EditorItem* selected = nullptr;

	EditorEntity selectedEntities;
	EditorGrass selectedGrasses;

	SelectionGizmo gizmo;

	bool addMode = false;
};