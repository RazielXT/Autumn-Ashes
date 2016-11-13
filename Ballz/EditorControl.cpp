#include "stdafx.h"
#include "EditorControl.h"
#include "win32\Win32Mouse.h"
#include "ObjectSelection.h"
#include "GUtils.h"
#include "PlayerCamera.h"

EditorControl::EditorControl(OIS::Mouse* mouse) : mMouse(mouse)
{
}

EditorControl::~EditorControl()
{
}

void EditorControl::displayEntityInfo(Ogre::Entity* ent)
{
	UiMessage msg;
	msg.id = UiMessageId::ShowEntityInfo;
	EntityInfo info;
	info.name = std::wstring(ent->getName().begin(), ent->getName().end());
	info.pos = ent->getParentSceneNode()->getPosition();
	info.scale = ent->getParentSceneNode()->getScale();
	msg.data = &info;
	uiHandler.sendMsg(&msg);
}

void EditorControl::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	if (active)
	{
		if (id == OIS::MB_Right || id == OIS::MB_Middle)
			setVievMode();

		if (id == OIS::MB_Middle)
			cam.mousePressed(arg, id);

		if (id == OIS::MB_Left && editMode)
		{
			if (mode == EditorMode::Select)
			{
				auto ent = ObjectSelection::pickEntity();

				if (ent)
				{
					displayEntityInfo(ent);
				}
			}
			else if (mode == EditorMode::AddItem)
			{
				auto cam = Global::camera->camera;
				auto mouseray = ObjectSelection::getMouseRay();
				GUtils::RayInfo rayInfo;
				if (GUtils::getRayInfo(cam->getDerivedPosition(), cam->getDerivedPosition() + mouseray.getDirection() * 100000, rayInfo))
				{
					auto e = GUtils::MakeEntity("aspenLeafs.mesh", rayInfo.pos);
					displayEntityInfo(e);
				}
			}
		}
	}
}

void EditorControl::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	if (active)
		if (id == OIS::MB_Right || id == OIS::MB_Middle)
			setEditMode();
}

bool EditorControl::update(float tslf)
{
	if (active)
	{
		auto msg = uiHandler.readUiMessage();

		if(!msg.empty)
			switch (msg.id)
			{
			case UiMessageId::SelectMode:
				mode = EditorMode::Select;
				break;
			case UiMessageId::MoveMode:
				mode = EditorMode::Move;
				break;
			case UiMessageId::ScaleMode:
				mode = EditorMode::Scale;
				break;
			case UiMessageId::RotateMode:
				mode = EditorMode::Rotate;
				break;
			case UiMessageId::AddItemMode:
				mode = EditorMode::AddItem;
				break;
			default:
				break;
			}
	}

	return active;
}

void EditorControl::setActive(bool active)
{
	if (this->active == active)
		return;

	this->active = active;

	if (active)
	{
		if(uiHandler.ensureUi())
			Global::mEventsMgr->addCachedTask(this);

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

