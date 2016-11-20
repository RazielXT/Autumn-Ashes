#pragma once

#include "Ogre.h"

namespace SceneInteraction
{
Ogre::Ray getMouseRay();
bool TestEntityRay(Ogre::Ray& ray, Ogre::Entity* pentity, Ogre::Vector3& hitpoint, float* closest_distance = nullptr, bool bothSides = false);
Ogre::Entity* PickEntity(Ogre::Ray &ray, Ogre::Vector3* hitpoint = nullptr, Ogre::Entity* excludeobject = nullptr, Ogre::Real max_distance = 20000);
}
