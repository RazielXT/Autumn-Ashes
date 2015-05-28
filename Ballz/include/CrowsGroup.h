#pragma once

#include "stdafx.h"
#include "EventTask.h"
#include "Crow.h"

class CrowsGroup;

class CrowFlight
{
public:

	CrowFlight(int crows, float randomYaw, float flightMinTime, Ogre::SceneNode* node);
	~CrowFlight();

	bool update(Ogre::Real tslf);

	Ogre::Animation* getFlightAnim();
	void addCrow(Crow* crow);

private:

	float randomYawMax;
	float flightMinTime;

	std::vector<Ogre::Animation*> flightAnims;
	std::vector<Crow*> crows;
	CrowsGroup* group;
};

class CrowLanding
{
public:

	CrowLanding(int crows, float radius, Ogre::SceneNode* node);
	~CrowLanding();

	bool update(Ogre::Real tslf);

	bool acceptsLanding() const;
	void addCrow(Crow* crow);

private:

	float radius;
	Ogre::Vector3 pos;

	std::vector<Crow*> crows;
	CrowsGroup* group;
};

class CrowsGroup : public EventTask
{
public:

	CrowsGroup(int mID) : id(mID) {};
	~CrowsGroup();

	bool update(Ogre::Real tslf);

	CrowFlight* getPossibleFlight();
	CrowLanding* getPossibleLanding();

	const int id;

private:

	std::vector<CrowFlight*> flights;
	std::vector<CrowLanding*> landings;
	
};