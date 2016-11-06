#include "stdafx.h"
#include "EditorControl.h"
#include "win32\Win32Mouse.h"

EditorControl::EditorControl(OIS::Mouse* mouse) : mMouse(mouse)
{
}

void EditorControl::startEditor()
{
	started = true;
	setVievMode();
}

void EditorControl::setVievMode()
{
	cam.enable();

	((OIS::Win32Mouse*)mMouse)->setForegroundMode(false);
	ShowCursor(FALSE);
	editMode = false;
}

void EditorControl::setEditMode()
{
	cam.unregisterInputListening();

	((OIS::Win32Mouse*)mMouse)->setForegroundMode(true);
	ShowCursor(TRUE);
	editMode = true;
}

bool EditorControl::connectEditorUi()
{

}

void EditorControl::sendMsg(std::vector<char>& data)
{

}

