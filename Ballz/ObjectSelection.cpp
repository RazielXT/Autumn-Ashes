#include "stdafx.h"
#include "ObjectSelection.h"
#include "SceneInteraction.h"
#include "Player.h"

extern int mouseX;
extern int mouseY;

namespace ObjectSelection
{
Ogre::Ray getMouseRay()
{
	auto cam = Global::camera->camera;
	return cam->getCameraToViewportRay(mouseX / (float)Global::mWindow->getWidth(), mouseY / (float)Global::mWindow->getHeight());
}

Ogre::Entity* pickEntity()
{
	return SceneInteraction::PickEntity(getMouseRay());
}
}

