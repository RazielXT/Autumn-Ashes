#pragma once
#include <string>
#include <vector>
#include <map>

enum class UiMessageId
{
	//to UI
	ShowEntityInfo,	 //EntityInfo

	//from UI
	GetWorldItems,	//GetWorldItemsData
	GetSceneSettings,	//GetSceneSettingsData
	AddItemMode,	 //AddItemModeInfo
	SelectMode,
	MoveMode,
	RotateMode,
	ScaleMode,
	EntityInfoChanged,	 //EntityInfoChange
	SceneSettingsChanged,	 //SceneSettingsChange
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
std::string wtos(void* str)
{
	std::wstring* wstr = (std::wstring*)str;
	return std::string(wstr->begin(), wstr->end());
}
std::wstring stow(std::string& str)
{
	return std::wstring(str.begin(), str.end());
}
#endif

struct AddItemModeInfo
{
	std::string itemType;
	std::string prefix;
};

struct EntityInfo
{
	std::wstring name;
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

struct EntityInfoChange
{
	enum class EntityChange { Pos, Scale } change;
	void* data;
};

struct GetSceneSettingsData
{
	std::vector<std::wstring> skyboxOptions;
	int currentSkyboxId;

	std::vector<std::wstring> lutOptions;
	int currentLutId;
};

struct SceneSettingsChange
{
	enum class SceneChange { Skybox, Lut } change;
	void* data;
};