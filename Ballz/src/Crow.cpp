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
	path.update(tslf, mNode);

	//update state timer
	stateChangeTimer -= tslf;
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
		path.setLiftingAnim(flightAnim, 0);
	}
	else
	{
		//force state
		path.setLiftingAnim(flightAnim, 0);
	}
}

void Crow::landTo(Ogre::Vector3 pos)
{
	if (path.state == OnGround)
	{
		//start landing
		path.setLandingAnim(pos);
	}
	else
	{
		//force state
		path.setLandingAnim(pos);
	}
}
