#pragma once

#include "stdafx.h"
#include "EditorItem.h"

class ObjectSelection;

class SelectionGizmo : public InputListener
{
public:

	SelectionMode currentMode = SelectionMode::Select;
	EditorItem* item;
	ObjectSelection* parent;

	Ogre::Entity* axesPlanes[3];

	void init(ObjectSelection* parent);

	void setRoot(EditorItem* item);

	void setMode(SelectionMode mode);

	bool update();

protected:

	bool copyMode = false;
	void deactivate();

	virtual void pressedKey(const OIS::KeyEvent &arg);
	virtual void releasedKey(const OIS::KeyEvent &arg);

	virtual void mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	virtual void mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	virtual void movedMouse(const OIS::MouseEvent &e) override;

	struct GizmoWidget
	{
		static const int MAX_AXES = 4;
		Ogre::Entity* axes[MAX_AXES];

		std::string selectedMaterial;

		int aboveAxis = -1;
		int activeAxis = -1;

		virtual void deinit();
		virtual void mousePressed(OIS::MouseButtonID id, EditorItem* root);
		virtual void mouseReleased(OIS::MouseButtonID id, EditorItem* root, SelectionMode mode);
		virtual void movedMouse(int x, int y, EditorItem* root, SelectionGizmo* parent);

		virtual void init(SelectionGizmo* parent) = 0;
		virtual void movedActiveMouse(int x, int y, EditorItem* root) = 0;
		virtual void setDefaultMats() = 0;
	};

	struct MoveGizmoWidget : GizmoWidget
	{
		virtual void init(SelectionGizmo* parent);
		virtual void movedActiveMouse(int x, int y, EditorItem* root);
		virtual void setDefaultMats();
	};
	MoveGizmoWidget moveWidget;

	struct ScaleGizmoWidget : GizmoWidget
	{
		virtual void init(SelectionGizmo* parent);
		virtual void movedActiveMouse(int x, int y, EditorItem* root);
		virtual void setDefaultMats();
	};
	ScaleGizmoWidget scaleWidget;

	struct RotateGizmoWidget : GizmoWidget
	{
		virtual void init(SelectionGizmo* parent);
		virtual void movedActiveMouse(int x, int y, EditorItem* root);
		virtual void setDefaultMats();
	};
	RotateGizmoWidget rotateWidget;

	GizmoWidget* activeWidget = nullptr;
};