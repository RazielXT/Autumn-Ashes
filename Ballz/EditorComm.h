#pragma once
#include <string>
#include <vector>

enum class UiMessageId
{
	//to UI
	ShowEntityInfo,	 //EntityInfo

	//from UI
	GetWorldItems,	//GetWorldItemsData
	SelectWorldItem,	 //WorldItem
	SetSelect,
	SetMove,
	SetRotate,
	SetScale,
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

struct Quaternion
{
	float x;
	float y;
	float z;
	float w;
};
}
#endif


struct EntityInfo
{
	std::wstring name;
	Ogre::Vector3 pos;
	Ogre::Vector3 scale;
	Ogre::Quaternion rotation;
};

struct WorldItem
{
	std::wstring name;
	std::wstring type;
};

struct GetWorldItemsData
{
	std::vector<WorldItem> items;
};