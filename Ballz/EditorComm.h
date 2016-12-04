#pragma once
#include <string>
#include <vector>
#include <map>

enum class UiMessageId
{
	//to UI
	HideSelectionInfo,
	ShowSelectionInfo,	 //SelectionInfo
	ShowMouse,
	HideMouse,
	StartLoading,
	EndLoading,

	//from UI
	GetWorldItems,	//GetWorldItemsData
	SelectWorldItem,	//SelectWorldItemData
	GetSceneSettings,	//GetSceneSettingsData
	AddItemMode,	 //AddItemModeInfo
	SelectMode,
	MoveMode,
	RotateMode,
	ScaleMode,
	SelectionInfoChanged,	 //SelectionInfoChange
	SceneSettingsChanged,	 //SceneSettingsChange
	LookAtSelection,
	PlacementRayUtil		//float
};

struct UiMessage
{
	UiMessageId id;
	void* data;
};

#ifndef OGRE_VERSION
namespace Ogre
{
struct Vector3
{
	float x;
	float y;
	float z;
};
}
#else
extern std::string wtos(std::wstring& str);
extern std::string wtos(void* str);
extern std::wstring stow(std::string& str);
#endif

struct AddItemModeInfo
{
	std::string itemType;
	std::string prefix;
};

struct SelectionInfo
{
	std::wstring name;
	std::vector<std::wstring> names;
	Ogre::Vector3 pos;
	Ogre::Vector3 scale;
};

struct WorldItem
{
	std::wstring name;
};

struct WorldItemsGroup
{
	std::wstring name;
	std::vector<WorldItem> items;
};

struct GetWorldItemsData
{
	std::vector<WorldItemsGroup> groups;
};

struct SelectWorldItemData
{
	std::string groupName;
	WorldItem item;
};

struct SelectionInfoChange
{
	enum class SelectionChange { Pos, Scale } change;
	void* data;
};

struct GetSceneSettingsData
{
	std::vector<std::wstring> skyboxOptions;
	int currentSkyboxId = 0;

	std::vector<std::wstring> lutOptions;
	int currentLutId = 0;
};

struct SceneSettingsChange
{
	enum class SceneChange { Skybox, Lut } change;
	void* data;
};