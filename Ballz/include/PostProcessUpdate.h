#pragma once

#include "stdafx.h"
#include "EventTask.h"
#include "PostProcessSwimming.h"

class Player;
struct PostProcessVariables;

class PostProcessUpdate : public EventTask
{
	Ogre::Camera* cam;
	PostProcessVariables* vars;

	//motion blur
	Ogre::Matrix4 prevVP;
	Ogre::Vector3 prevPos;
	Ogre::Quaternion prevOr;
	Ogre::Real mPreviousFPS;

public:

	PostProcessSwimming swimming;

	PostProcessUpdate();

	void injectPostProcess(PostProcessVariables* vars);

	bool update(float tslf);
};