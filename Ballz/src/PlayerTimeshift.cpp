#include "stdafx.h"
#include "PlayerTimeshift.h"
#include "Player.h"
#include "PlayerEnergies.h"
#include "Energy.h"

const float timeshiftTimer = 5.0f;

PlayerTimeshift::PlayerTimeshift(Player* player, PlayerEnergies* energies)
{
    p = player;
    pEnergies = energies;
}

void PlayerTimeshift::setShiftTime()
{
    p->saveState(playerHistory);
}

void PlayerTimeshift::shiftBack()
{
    if (!chargedEnergy)
    {
        chargedEnergy = pEnergies->getAvailableEnergy();

        if (chargedEnergy)
        {
            chargedEnergy->startUsage(timeshiftTimer, Energy::Flashback);
            setShiftTime();
        }
    }
    else
    {
        auto e = pEnergies->getAvailableEnergy();

        if (e)
        {
            Global::player->loadState(playerHistory);
            Global::player->bodyVelocityL = 0;

            e->startUsage(energyRechargeTime, Energy::Recharging, true);
            chargedEnergy->startUsage(energyRechargeTime, Energy::Recharging, true);
        }
    }
}

void PlayerTimeshift::updateStateHistory(float tslf)
{
    if (chargedEnergy && !chargedEnergy->updateUsage(tslf))
    {
        chargedEnergy = nullptr;
    }
}