#include "stdafx.h"
#include "EditorControl.h"
#include "win32\Win32Mouse.h"

EditorControl::EditorControl(OIS::Mouse* mouse) : mMouse(mouse)
{
}

EditorControl::~EditorControl()
{
}

void EditorControl::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	if (active)
	{
		if (id == OIS::MB_Right || id == OIS::MB_Middle)
			setVievMode();

		if (id == OIS::MB_Middle)
			cam.mousePressed(arg, id);
	}
}

void EditorControl::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	if (active)
		if (id == OIS::MB_Right || id == OIS::MB_Middle)
			setEditMode();
}

void EditorControl::setActive(bool active)
{
	if (this->active == active)
		return;

	this->active = active;

	if (active)
	{
		cam.enable();
		setEditMode();
		registerInputListening();
	}
	else
	{
		setVievMode();
		cam.returnToPlayer();
		unregisterInputListening();
	}
}

void EditorControl::setVievMode()
{
	if (!editMode)
		return;

	while (ShowCursor(FALSE));
	((OIS::Win32Mouse*)mMouse)->setForegroundMode(false);
	editMode = false;

	cam.enable();
}

void EditorControl::setEditMode()
{
	if (editMode)
		return;

	while (ShowCursor(TRUE));
	((OIS::Win32Mouse*)mMouse)->setForegroundMode(true);
	editMode = true;

	cam.disable();
}

bool EditorControl::connectEditorUi()
{

}

void EditorControl::sendMsg(std::vector<char>& data)
{

}

void EditorControl::movedMouse(const OIS::MouseEvent &e)
{
}

