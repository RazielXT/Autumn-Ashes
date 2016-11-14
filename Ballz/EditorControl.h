#pragma once
#include <vector>
#include "EditorCamera.h"
#include "EditorUiHandler.h"
#include "ObjectSelection.h"
#include "SceneOptions.h"

class EditorControl : public InputListener, EventTask
{
public:

	EditorControl(OIS::Mouse* mMouse);
	~EditorControl();

	virtual void mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	virtual void mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

	virtual bool update(float tslf);

	bool connected = false;
	bool active = false;
	bool editMode = false;

	void setActive(bool active);

	void setVievMode();
	void setEditMode();

	bool connectEditorUi();

	virtual void movedMouse(const OIS::MouseEvent &e) override;

	EditorCamera cam;
	OIS::Mouse* mMouse;

	EditorUiHandler uiHandler;

	enum class EditorMode { None, AddItem, Select, Move, Rotate, Scale } mode = EditorMode::None;

	void displayEntityInfo(Ogre::Entity* ent);
	void getWorldItemsInfo(GetWorldItemsData& data);

	ObjectSelection selector;
	SceneOptions scene;
};