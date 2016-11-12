#pragma once

#include "Ogre.h"

namespace SceneInteraction
{
Ogre::Entity* PickEntity(Ogre::Ray &ray, Ogre::Vector3* hitpoint = nullptr, Ogre::Entity* excludeobject = nullptr, Ogre::Real max_distance = 20000);
}
