#pragma once

#include "stdafx.h"

class Player;
struct PostProcessVariables;

class PlayerPostProcess
{
	friend class Player;

	Player* p;
	PostProcessVariables* vars;

	//motion blur
	Ogre::Matrix4 prevVP;
	Ogre::Vector3 prevPos;
	Ogre::Quaternion prevOr;
	Ogre::Real mPreviousFPS;

public:

	PlayerPostProcess(Player* player);

	void injectPostProcess(PostProcessVariables* vars);

	void update(float tslf);
};