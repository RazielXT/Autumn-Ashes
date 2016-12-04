#include "stdafx.h"
#include "EditorControl.h"
#include "win32\Win32Mouse.h"
#include "ObjectSelection.h"
#include "GUtils.h"
#include "PlayerCamera.h"
#include "SceneInteraction.h"


std::string wtos(void* str)
{
	std::wstring* wstr = (std::wstring*)str;
	return std::string(wstr->begin(), wstr->end());
}

std::string wtos(std::wstring& wstr)
{
	return std::string(wstr.begin(), wstr.end());
}

std::wstring stow(std::string& str)
{
	return std::wstring(str.begin(), str.end());
}

EditorControl::EditorControl(EditorUiHandler& handler, OIS::Mouse* mouse) : mMouse(mouse), uiHandler(handler)
{
#ifdef EDITOR
	active = true;
	connected = true;

	mode = EditorMode::Select;
	selector.setMode(SelectionMode::Select);
#endif // EDITOR
}

EditorControl::~EditorControl()
{
}

void EditorControl::displayItemInfo(EditorItem* item)
{
	if (item)
	{
		item->sendUiInfoMessage(&uiHandler);
	}
	else
	{
		uiHandler.sendMsg(UiMessage{ UiMessageId::HideSelectionInfo });
	}
}

void EditorControl::getWorldItemsInfo(GetWorldItemsData& data)
{
	auto it = Global::sceneMgr->getMovableObjectIterator("Entity");

	WorldItemsGroup entityGroup;
	entityGroup.name = L"Entity";
	while (it.hasMoreElements())
	{
		Ogre::Entity* e = static_cast<Ogre::Entity*>(it.getNext());
		std::wstring name(e->getName().cbegin(), e->getName().cend());
		entityGroup.items.push_back({ name });
	}

	data.groups.push_back(entityGroup);
}

void EditorControl::pressedKey(const OIS::KeyEvent &arg)
{
	if (arg.key == OIS::KC_LCONTROL)
		selector.addMode = true;

	if (arg.key == OIS::KC_DELETE)
	{
		selector.removeSelection();
	}
}

void EditorControl::releasedKey(const OIS::KeyEvent &arg)
{
	if (arg.key == OIS::KC_LCONTROL)
		selector.addMode = false;
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
				selector.pickMouseRayItem();
			}
			else if (mode == EditorMode::AddItem)
			{
				auto cam = Global::camera->cam;
				auto mouseray = SceneInteraction::getMouseRay();
				GUtils::RayInfo rayInfo;
				if (GUtils::getRayInfo(cam->getDerivedPosition(), cam->getDerivedPosition() + mouseray.getDirection() * 100000, rayInfo))
				{
					auto e = GUtils::MakeEntity("aspenLeafs.mesh", rayInfo.pos);
					selector.setSelectedEntity(e);
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
				selector.setMode(SelectionMode::Select);
				break;
			case UiMessageId::MoveMode:
				mode = EditorMode::SelectEdit;
				selector.setMode(SelectionMode::Move);
				break;
			case UiMessageId::ScaleMode:
				mode = EditorMode::SelectEdit;
				selector.setMode(SelectionMode::Scale);
				break;
			case UiMessageId::RotateMode:
				mode = EditorMode::SelectEdit;
				selector.setMode(SelectionMode::Rotate);
				break;
			case UiMessageId::AddItemMode:
				mode = EditorMode::AddItem;
				break;
			case UiMessageId::GetWorldItems:
				getWorldItemsInfo(*(GetWorldItemsData*)msg.data);
				break;
			case UiMessageId::SelectWorldItem:
				selector.uiSelectItem(*(SelectWorldItemData*)msg.data);
				break;
			case UiMessageId::SelectionInfoChanged:
				selector.uiEditSelection((SelectionInfoChange*) msg.data);
				break;
			case UiMessageId::PlacementRayUtil:
				selector.rayPlaceSelection(*(float*)msg.data);
				break;
			case UiMessageId::GetSceneSettings:
				scene.getCurrentSceneInfo((GetSceneSettingsData*)msg.data);
				break;
			case UiMessageId::SceneSettingsChanged:
				scene.editScene((SceneSettingsChange*)msg.data);
				break;
			case UiMessageId::LookAtSelection:
				selector.lootAtSelection();
				break;
			default:
				break;
			}
	}

	selector.gizmo.update();

	return uiHandler.isActiveUi();
}

void EditorControl::afterLoadInit()
{
	if (active)
	{
		Global::eventsMgr->addCachedTask(this);
		selector.init(this);
		registerInputListening();
		cam.enable();
		setEditMode();

		((OIS::Win32Mouse*)mMouse)->setForegroundMode(true);
	}
}

void EditorControl::toggleActivePlay()
{
	if (connected)
	{
		active = !active;

		if (!active)
		{
			cam.returnToPlayer();
			unregisterInputListening();
			//((OIS::Win32Mouse*)mMouse)->setForegroundMode(false);

			uiHandler.sendMsg(UiMessage{ UiMessageId::HideMouse });
		}
		else
		{
			cam.enable();
			cam.disable();
			registerInputListening();
			//((OIS::Win32Mouse*)mMouse)->setForegroundMode(true);

			uiHandler.sendMsg(UiMessage{ UiMessageId::ShowMouse });
		}
	}
}

void EditorControl::setVievMode()
{
	if (!editMode)
		return;

	//while (ShowCursor(FALSE) > 0);
	//((OIS::Win32Mouse*)mMouse)->setForegroundMode(false);
	editMode = false;

	cam.enable();
}

void EditorControl::setEditMode()
{
	if (editMode)
		return;

	//while (ShowCursor(TRUE) <= 0);
	//((OIS::Win32Mouse*)mMouse)->setForegroundMode(true);
	editMode = true;

	SetFocus(uiHandler.outputWindowHwnd);
	cam.disable();
}

bool EditorControl::connectEditorUi()
{

}

void EditorControl::movedMouse(const OIS::MouseEvent &e)
{
}

