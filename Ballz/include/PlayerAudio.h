#pragma once

#include "stdafx.h"

class PlayerAudio
{
	friend class Player;

	Player* p;

public:

	PlayerAudio(Player* player);
	~PlayerAudio();

	void update(float tslf);

};