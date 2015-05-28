#include "stdafx.h"
#include "Crow.h"

using namespace Ogre; 

Crow::Crow()
{
	//init entity+node

	state = None;
	stateTimer = 0;
}

Crow::~Crow()
{

}

bool Crow::update(Ogre::Real tslf)
{
	//update model anim
	//update node pos
	//update state timer
}

bool Crow::readyToFly() const
{
	return (state == OnGround && stateTimer <= 0);
}

bool Crow::readyToLand() const
{
	return (state == Flying && stateTimer <= 0);
}

void Crow::flyTo(Ogre::Animation* flightAnim)
{
	if (state == OnGround)
	{
		//start lifting
	}
	else
	{
		//force state
	}
}

void Crow::landTo(Ogre::Vector3 pos)
{
	if (state == OnGround)
	{
		//start landing
	}
	else
	{
		//force state
	}
}

void Crow::createLandingAnim()
{
}

void Crow::createLiftingAnim()
{

}