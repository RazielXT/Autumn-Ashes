#pragma once
#include <vector>
#include "EditorCamera.h"
#include "EditorUiHandler.h"
#include "EditorPainter.h"
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
	virtual void movedMouse(const OIS::MouseEvent &e);

	virtual bool update(float tslf);

	bool connected = false;
	bool active = false;
	bool editMode = false;

	void afterLoadInit();
	void toggleActivePlay();

	void setVievMode();
	void setEditMode();

	void initEditorProperties();

	OIS::Mouse* mMouse;
	EditorUiHandler& uiHandler;

	enum class EditorMode { None, AddItem, Paint, Select, SelectEdit } mode = EditorMode::None;

	void setMode(EditorMode mode);
	void setPaintMode(EditorPainter::PaintMode mode);
	void setSelectMode(SelectionMode mode);
	void setAddItemMode(AddItemModeInfo* info);
	AddItemModeInfo addItemModeInfo;

	void displayItemInfo(EditorItem* item);

	virtual void pressedKey(const OIS::KeyEvent &arg) override;

	virtual void releasedKey(const OIS::KeyEvent &arg) override;
	void addItemToScene();
	ObjectSelection selector;
	EditorPainter painter;
	SceneOptions scene;
	EditorCamera cam;
};