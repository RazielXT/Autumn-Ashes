#ifndef LVL1REAC_H
#define LVL1REAC_H

#include "stdafx.h"
#include "EventTask.h"
#include "EventTask.h"
#include "PostProcessMgr.h"

class ParkToDreamReaction : public EventTask
{
public:

	ParkToDreamReaction();

	void setUserData(void* data);
	bool start();
	bool update(Ogre::Real tslf);

private:

	float timer;
};

class NearParkEndReaction : public EventTask
{
public:

	NearParkEndReaction();

	void setUserData(void* data);
	bool start();
	bool update(Ogre::Real tslf);

private:

	irrklang::ISound* noiseSound;
	PostProcessMgr* mgr;
	float timer;
	bool doneEffect, doneEffect0;
};

class ParkEndReaction : public EventTask
{
public:

	ParkEndReaction();

	void setUserData(void* data);
	bool start();
	bool update(Ogre::Real tslf);

private:

	irrklang::ISound* sound;
	PostProcessMgr* mgr;
	float timer;
	bool doneEffect1 , doneEffect2, doneEffect3;
	Ogre::AnimationState *mAnim1,*mAnim2,*mAnim3,*mAnim4;
};

class OutOfCave : public EventTask
{
public:

	OutOfCave();

	void setUserData(void* data);
	bool start();
};

#endif