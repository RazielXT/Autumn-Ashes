#pragma once

#include "stdafx.h"

class CrowsPack;


class Crow
{
	friend class CrowFlight;
	friend class CrowLanding;

public:

	Crow();
	~Crow();

	bool update(Ogre::Real tslf);

	bool wantsToFly() const;
	bool wantsToLand() const;

	void flyTo();
	bool landTo();

protected:

	enum {	OnGround, Flying, Landing, Lifting	} state;
	
	Ogre::SceneNode* mNode;
	Ogre::Entity* mEntity;
};