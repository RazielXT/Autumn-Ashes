#pragma once
#include <vector>
#include "EditorCamera.h"
#include "EditorUiHandler.h"
#include "ObjectSelection.h"
#include "SceneOptions.h"

struct EditorItem;

class EditorControl : public InputListener, EventTask
{
public:

	EditorControl(EditorUiHandler& handler, OIS::Mouse* mMouse);
	~EditorControl();

	virtual void mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	virtual void mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	virtual void movedMouse(const OIS::MouseEvent &e) override;

	virtual bool update(float tslf);

	bool connected = false;
	bool active = false;
	bool editMode = false;

	void afterLoadInit();
	void toggleActivePlay();

	void setVievMode();
	void setEditMode();

	bool connectEditorUi();

	EditorCamera cam;
	OIS::Mouse* mMouse;

	EditorUiHandler& uiHandler;

	enum class EditorMode { None, AddItem, Select, SelectEdit } mode = EditorMode::None;

	void displayItemInfo(EditorItem* item);
	void getWorldItemsInfo(GetWorldItemsData& data);

	virtual void pressedKey(const OIS::KeyEvent &arg) override;

	virtual void releasedKey(const OIS::KeyEvent &arg) override;

	ObjectSelection selector;
	SceneOptions scene;
};