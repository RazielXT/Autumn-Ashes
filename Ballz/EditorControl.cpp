#include "stdafx.h"
#include "EditorControl.h"
#include "win32\Win32Mouse.h"

EditorControl::EditorControl(OIS::Mouse* mouse) : mMouse(mouse)
{
}

EditorControl::~EditorControl()
{
}

void EditorControl::setActive(bool active)
{
	if (this->active == active)
		return;

	this->active = active;

	if (active)
		setVievMode();
	else
	{
		cam.disable();
	}
}

void EditorControl::setVievMode()
{
	cam.enable();

	((OIS::Win32Mouse*)mMouse)->setForegroundMode(false);
	int i = ShowCursor(FALSE);
	editMode = false;
}

void EditorControl::setEditMode()
{
	cam.unregisterInputListening();

	((OIS::Win32Mouse*)mMouse)->setForegroundMode(true);
	while (ShowCursor(TRUE));
	editMode = true;
}

bool EditorControl::connectEditorUi()
{

}

void EditorControl::sendMsg(std::vector<char>& data)
{

}

