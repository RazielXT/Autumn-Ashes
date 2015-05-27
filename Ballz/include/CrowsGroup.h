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

	void createPossibleFlight();

private:

	float randomYawMax;
	float flightMinTime;

	std::vector<Ogre::Animation*> flightNames;
	std::vector<Crow*> crows;
	CrowsGroup* pack;
};

class CrowLanding
{
public:

	CrowLanding(int crows, float radius);
	~CrowLanding();

	bool update(Ogre::Real tslf);

	bool acceptsLanding() const;

private:

	float radius;

	std::vector<Crow*> crows;
	CrowsGroup* pack;
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