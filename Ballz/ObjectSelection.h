#pragma once
#include "stdafx.h"
#include "SelectionGizmo.h"
#include "EditorComm.h"
#include "EditorItem.h"
#include "EditorEntity.h"

struct SelectionInfoChange;
class EditorControl;

class ObjectSelection
{
public:

	EditorControl* parent;

	void init(EditorControl* parent);

	void setMode(SelectionMode mode);

	void setSelectedEntity(Ogre::Entity* ent, bool forceDeselect = true);

	void updateUiSelectedInfo();

	Ogre::Entity* pickMouseRayItem();

	void lootAtSelection();

	void uiEditSelection(SelectionInfoChange* change);

	void uiSelectItem(SelectWorldItemData& data);
	void removeSelection();
	EditorItem* selected = nullptr;

	EditorEntity selectedEntities;

	SelectionGizmo gizmo;

	bool addMode = false;
};