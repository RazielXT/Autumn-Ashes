#include "stdafx.h"
#include "EditorControl.h"
#include "win32\Win32Mouse.h"
#include "ObjectSelection.h"
#include "GUtils.h"
#include "PlayerCamera.h"
#include "SceneInteraction.h"
#include "GameStateManager.h"


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

EditorControl::EditorControl(EditorUiHandler& handler, OIS::Mouse* mouse) : mMouse(mouse), uiHandler(handler), painter(this), selector(this)
{
#ifdef EDITOR
	active = true;
	connected = true;

	setMode(EditorMode::Select);
	selector.setMode(SelectionMode::Select);
#endif // EDITOR
}

EditorControl::~EditorControl()
{
}

void EditorControl::initEditorProperties()
{
	EditorProperties editor;
	auto levels = Global::gameMgr->getLevels();
	for (auto l : levels)
	{
		editor.levels.push_back(l.name);
	}

	uiHandler.sendMsg(UiMessage{UiMessageId::SetEditorProperties, &editor});
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

void EditorControl::addItemToScene()
{
	if (addItemModeInfo.type == ItemType::Entity)
	{
		if (addItemModeInfo.path.empty())
			return;

		auto pathEnd = addItemModeInfo.path.find_last_of('\\') + 1;
		auto location = addItemModeInfo.path.substr(0, pathEnd);
		auto name = addItemModeInfo.path.substr(pathEnd);

		auto cam = Global::camera->cam;
		auto mouseray = SceneInteraction::getMouseRay();
		GUtils::RayInfo rayInfo;
		if (GUtils::getRayInfo(cam->getDerivedPosition(), cam->getDerivedPosition() + mouseray.getDirection() * 100000, rayInfo))
		{
			try
			{
				if (ResourceGroupManager::getSingleton().findResourceLocation("Level", location)->empty())
					ResourceGroupManager::getSingleton().addResourceLocation(location, "FileSystem", "Level");

				auto e = GUtils::MakeEntity(name, rayInfo.pos, Ogre::Vector3(1, 1, 1)*0.25f);
				e->setMaterialName("flame_body");
				//auto e = GUtils::MakeEntity("aspenLeafs.mesh", rayInfo.pos);
				selector.setSelectedEntity(e);
			}
			catch (Ogre::Exception& e)
			{
				MessageBoxA(NULL, e.getDescription().c_str(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
			}
		}
	}
}

void EditorControl::setMode(EditorMode m)
{
	mode = m;

	if (mode != EditorMode::Paint)
		painter.setMode(EditorPainter::None);

	if(mode != EditorMode::SelectEdit)
		selector.setMode(SelectionMode::Select);
}

void EditorControl::setPaintMode(EditorPainter::PaintMode m)
{
	if(m == EditorPainter::None)
		setSelectMode(SelectionMode::Select);
	else
	{
		setMode(EditorMode::Paint);
		painter.setMode(m);
	}
}

void EditorControl::setSelectMode(SelectionMode m)
{
	if(m == SelectionMode::Select)
		setMode(EditorMode::Select);
	else
		setMode(EditorMode::SelectEdit);

	selector.setMode(m);
}

void EditorControl::setAddItemMode(AddItemModeInfo* info)
{
	addItemModeInfo = *info;

	setMode(EditorMode::AddItem);
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
				addItemToScene();
			}
			else if (mode == EditorMode::Paint)
			{
				painter.mousePressed();
			}
		}
	}
}

void EditorControl::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	if (active)
		if (id == OIS::MB_Right || id == OIS::MB_Middle)
			setEditMode();
		else if (editMode && id == OIS::MB_Left && mode == EditorMode::Paint)
		{
			painter.mouseReleased();
		}
}

bool EditorControl::update(float tslf)
{
	if (active)
	{
		auto msg = uiHandler.readUiMessage();

		if (!msg.empty)
			GUtils::DebugPrint("UiMsg " + std::to_string((int)msg.id));

		if(!msg.empty)
			switch (msg.id)
			{
			case UiMessageId::SelectMode:
				setSelectMode(SelectionMode::Select);
				break;
			case UiMessageId::MoveMode:
				setSelectMode(SelectionMode::Move);
				break;
			case UiMessageId::ScaleMode:
				setSelectMode(SelectionMode::Scale);
				break;
			case UiMessageId::RotateMode:
				setSelectMode(SelectionMode::Rotate);
				break;
			case UiMessageId::AddItemMode:
				setAddItemMode((AddItemModeInfo*)msg.data);
				break;
			case UiMessageId::GetWorldItems:
				selector.uiGetWorldItemsInfo(*(GetWorldItemsData*)msg.data);
				break;
			case UiMessageId::SelectWorldItem:
				selector.uiSelectItem(*(SelectWorldItemData*)msg.data);
				break;
			case UiMessageId::SelectionInfoChanged:
				selector.uiEditSelection((SelectionInfoChange*) msg.data);
				painter.handleChangeMessage((SelectionInfoChange*)msg.data);
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
			case UiMessageId::LoadLevel:
			{
				auto levelName = *(std::string*)msg.data;
				msg.release();
				Global::gameMgr->switchToLevel(levelName);
				break;
			}
			case UiMessageId::CloseEditor:
				exit(0);
				break;
			case UiMessageId::ReloadGeometry:
				Global::gameMgr->reloadMeshes();
				break;
			case UiMessageId::ReloadShaders:
				scene.reloadShaders();
				break;
			case UiMessageId::SetRedrawSettings:
			{
				scene.fpsCap = (float) *(int*)msg.data;
				Global::window->setAutoUpdated(scene.fpsCap != 0);
				break;
			}
			case UiMessageId::ActivatePlayMode:
				toggleActivePlay();
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
		selector.init();
		painter.init();
		registerInputListening();
		cam.camNode = nullptr;
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

void EditorControl::movedMouse(const OIS::MouseEvent&)
{
	if (active && editMode && mode == EditorMode::Paint)
	{
		painter.mouseMoved();
	}
}

