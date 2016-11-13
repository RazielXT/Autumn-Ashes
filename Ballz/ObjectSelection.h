#pragma once
#include "stdafx.h"

namespace ObjectSelection
{
Ogre::Ray getMouseRay();

Ogre::Entity* pickEntity();
};