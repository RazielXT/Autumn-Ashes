#pragma once
#include <vector>
#include "EditorCamera.h"

class EditorControl : public InputListener
{
public:

	EditorControl(OIS::Mouse* mMouse);
	~EditorControl();

	virtual void mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	virtual void mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

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