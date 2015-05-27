#pragma once

#include "stdafx.h"
#include "EventTask.h"
#include "Crow.h"

class CrowsGroup;

class CrowFlight
{
public:

	CrowFlight(int crows, float randomYaw, float flightMinTime);
	~CrowFlight();

	bool update(Ogre::Real tslf);

	void createPossibleFlight();

private:

	std::vector<Crow*> crows;
	CrowsGroup* pack;
	Ogre::Entity* mEntity;
};

class CrowLanding
{
public:

	CrowLanding(int crows, float radius);
	~CrowLanding();

	bool update(Ogre::Real tslf);

	bool acceptsLanding() const;

private:

	std::vector<Crow*> crows;
	CrowsGroup* pack;
	Ogre::Entity* mEntity;
};

class CrowsGroup : public EventTask
{
public:

	CrowsGroup(int mID) : id(mID) {};
	~CrowsGroup();

	bool update(Ogre::Real tslf);
	void getPossibleFlight();
	void getPossibleLanding();

	const int id;

private:

	std::vector<CrowFlight*> flights;
	std::vector<CrowLanding*> landings;
	
};