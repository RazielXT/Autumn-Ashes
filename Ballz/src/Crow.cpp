#include "stdafx.h"
#include "Crow.h"

using namespace Ogre; 

Crow::Crow()
{
	//init entity+node

	stateChangeTimer = 0;
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
	return (path.state == OnGround && stateChangeTimer <= 0);
}

bool Crow::readyToLand() const
{
	return (path.state == Flying && stateChangeTimer <= 0);
}

void Crow::flyTo(Ogre::Animation* flightAnim)
{
	if (path.state == OnGround)
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
	if (path.state == OnGround)
	{
		//start landing
	}
	else
	{
		//force state
	}
}
