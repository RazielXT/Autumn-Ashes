#include "stdafx.h"
#include "ObjectSelection.h"
#include "SceneInteraction.h"
#include "Player.h"
#include "EditorComm.h"

extern int mouseX;
extern int mouseY;

Ogre::Ray ObjectSelection::getMouseRay()
{
	auto cam = Global::camera->camera;
	return cam->getCameraToViewportRay(mouseX / (float)Global::mWindow->getWidth(), mouseY / (float)Global::mWindow->getHeight());
}

Ogre::Entity* ObjectSelection::pickEntity()
{
	pickedEntity = SceneInteraction::PickEntity(getMouseRay());
	return pickedEntity;
}

void ObjectSelection::editEntity(EntityInfoChange* change)
{
	if (pickedEntity)
	{
		Ogre::Vector3* vec = (Ogre::Vector3*)change->data;
		switch (change->change)
		{
		case EntityInfoChange::EntityChange::Pos:
			pickedEntity->getParentSceneNode()->setPosition(Ogre::Vector3(5,0,0));
			break;
		case EntityInfoChange::EntityChange::Scale:
			pickedEntity->getParentSceneNode()->setScale(*vec);
			break;
		default:
			break;
		}
	}
}

