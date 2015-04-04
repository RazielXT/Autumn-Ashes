#pragma once

#include "stdafx.h"

class Player;

class PlayerParkour
{
	friend class Player;

	Player* p;
	OgreNewt::Body* body;


public:

	PlayerParkour(Player* player);

	bool spacePressed();
};