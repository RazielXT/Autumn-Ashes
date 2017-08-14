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
	SetEditorProperties,  //EditorProperties

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
	PlacementRayUtil,		//float
	LoadLevel,				//string
	CloseEditor,
	ReloadGeometry,
	ReloadShaders,
	SetRedrawSettings,			//int
	ActivatePlayMode,
	Ping
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
struct Vector4
{
	float x;
	float y;
	float z;
	float w;
};
}
#else
extern std::string wtos(std::wstring& str);
extern std::string wtos(void* str);
extern std::wstring stow(std::string& str);
#endif

enum class ItemType
{
	Entity, Grass, Particle, Light
};

struct AddItemModeInfo
{
	ItemType type = ItemType::Entity;
	std::string path;
};

struct SelectionInfo
{
	std::string name;
	std::vector<std::string> names;
	Ogre::Vector3 pos;
	Ogre::Vector3 scale;

	bool usePaint = false;
	bool hasParams = false;
	ItemType type = ItemType::Entity;
	void* typeData = nullptr;
};

struct EditableParam
{
	std::string name;
	std::string group;
	enum Type {Floats, Strings} type;

	struct
	{
		int size;
		float buffer[4];
		float step;
	}
	floats;

	std::vector<std::string> strings;
};

struct EditableParams
{
	std::string groupName;
	std::vector<EditableParam> params;
};

struct EditableParamsResponse
{
	std::vector<EditableParams> params;
};

struct EntitySelectionInfo
{
	std::vector<std::string> animNames;
};

struct GrassSelectionInfo
{
	float density;
	float minHSize;
	float maxHSize;
	float minWSize;
	float maxWSize;
	bool preserveMask;
};

struct WorldItem
{
	std::string name;
};

struct WorldItemsGroup
{
	ItemType type;
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
	enum class Id
	{
		Pos, Scale,
		GrassDensity, GrassSize, GrassPaintPreserve,
		PaintOff, PaintAdd, PaintRemove, PaintWChange, PaintSizeChange, PaintFill,
		ActiveAnimation,
		GetParams, SendParams, ParamChanged, SaveParams
	}
	change;

	void* data;
};

struct GetSceneSettingsData
{
	std::vector<std::string> skyboxOptions;
	int currentSkyboxId = 0;

	std::vector<std::string> lutOptions;
	int currentLutId = 0;
};

struct SceneSettingsChange
{
	enum class SceneChange { Skybox, Lut } change;
	void* data;
};

struct EditorProperties
{
	std::vector<std::string> levels;
};