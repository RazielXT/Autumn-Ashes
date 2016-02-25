#pragma once

#include "stdafx.h"
#include "PlayerStateInfo.h"

class Player;
class PlayerEnergies;
class Energy;

class PlayerTimeshift
{
	Player* p;
	PlayerEnergies* pEnergies;
	Energy* chargedEnergy = nullptr;

	PlayerStateInfo playerHistory;

	void setShiftTime();

public:

	PlayerTimeshift(Player* player, PlayerEnergies* energies);

	void shiftBack();
	void updateStateHistory(float tslf);
};