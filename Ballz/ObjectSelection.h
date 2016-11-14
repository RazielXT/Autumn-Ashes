#pragma once
#include "stdafx.h"

struct EntityInfoChange;

class ObjectSelection
{
public:

	Ogre::Entity* pickEntity();

	void editEntity(EntityInfoChange* change);

	Ogre::Ray getMouseRay();

	Ogre::Entity* pickedEntity = nullptr;
};