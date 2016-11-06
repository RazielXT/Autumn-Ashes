#pragma once
#include <vector>
#include "EditorCamera.h"

class EditorControl
{
public:

	EditorControl(OIS::Mouse* mMouse);
	~EditorControl();

	bool connected = false;
	bool active = false;
	bool editMode = false;

	void setActive(bool active);

	void setVievMode();
	void setEditMode();

	bool connectEditorUi();

	void sendMsg(std::vector<char>& data);

	EditorCamera cam;
	OIS::Mouse* mMouse;
};