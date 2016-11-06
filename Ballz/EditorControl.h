#pragma once
#include <vector>
#include "EditorCamera.h"

class EditorControl
{
public:

	EditorControl(OIS::Mouse* mMouse);

	bool connected = false;
	bool started = false;
	bool editMode = false;

	void startEditor();

	void setVievMode();
	void setEditMode();

	bool connectEditorUi();

	void sendMsg(std::vector<char>& data);

	EditorCamera cam;
	OIS::Mouse* mMouse;
};