#ifndef EFFR_H
#define EFFR_H

#include "stdafx.h"
#include "EventTask.h"
#include "PostProcessMgr.h"

class ExtendedSpline : public Ogre::SimpleSpline
{
public:

	Ogre::Vector3 interpolateX(float t) const;
};

class FovPeakEffect : public EventTask
{
public:

	FovPeakEffect();

	bool start(float time, float peakTime, float peak, float target = 0);
	bool start(float time, float peakTime, float peak, float peakTime2, float peak2, float target = 0);

	bool update(Ogre::Real tslf);

private:

	ExtendedSpline spline;

	float timer;
	float duration;
};

class QuickScaryBlink : public EventTask
{
public:

	QuickScaryBlink();

	bool start();
	bool update(Ogre::Real tslf);

private:

	float timer;
	float totalTime, effectStr;
	bool doneBlink;
	PostProcessMgr* mgr;
	short id;
};


class SwitchColorSchemeFx : public EventTask
{
public:

	/*
	P,P,P,P
	r,g,b,time
	float,float,float,float
	*/
	SwitchColorSchemeFx(Ogre::String info);

	bool start();
	bool update(float tslf);

private:

	OgreNewt::Body* mBody;
	float bloomStrBase;
	float fovBase;
	Ogre::Vector3 colorTarget;
	Ogre::Vector3 colorBase;
	float timeW;
	float timer;

	static int lastID;
	static int idCounter;
	int myID;
};

#endif