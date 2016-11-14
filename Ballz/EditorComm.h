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
	AddItemMode,	 //WorldItem
	SelectMode,
	MoveMode,
	RotateMode,
	ScaleMode,
	EntityInfoChanged,	 //EntityInfoChange
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
#endif


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