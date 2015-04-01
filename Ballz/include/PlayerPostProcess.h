#pragma once

#include "stdafx.h"

class Player;

class PlayerPostProcess
{
	friend class Player;

	//motion blur
	Ogre::Real* ppFall, *ppMotionBlur;
	Ogre::Matrix4* iVP, *pVP;
	Ogre::Matrix4 prevVP;
	Ogre::Vector3 prevPos;
	Ogre::Quaternion prevOr;
	Ogre::Real mPreviousFPS;

	Player* p;

public:

	PlayerPostProcess(Player* player);

	void injectPostProcess(Ogre::Real* postPFall, Ogre::Matrix4* invVP, Ogre::Matrix4* prevVP, Ogre::Real* mbAm)
	{
		ppFall = postPFall;
		iVP = invVP;
		pVP = prevVP;
		ppMotionBlur = mbAm;
	}

	void update(float tslf);
};