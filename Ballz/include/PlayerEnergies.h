#pragma once

#include "stdafx.h"

class Player;
class GameUi;
class Energy;
struct EnergyPlacement;

const float energyRechargeTime = 1.0f;

class PlayerEnergies
{
    Player* p;
    GameUi* ui;
    std::vector<Energy*> currentEnergies;

public:

    PlayerEnergies(Player* player);
    ~PlayerEnergies();

    bool pressedMouse(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
    bool releasedMouse(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

    bool pressedKey(const OIS::KeyEvent &arg);
    bool releasedKey(const OIS::KeyEvent &arg);

    Energy* getAvailableEnergy();
    void update(float tslf);

    Energy* facingEnergy();
    EnergyPlacement* facingEnergyPlacement();
    bool grabEnergy();

private:

    Ogre::Vector3 getChargedEnergyPosition(float time);
    void removeEnergy(Energy* e);

    Energy* releaseEnergy = nullptr;

    Ogre::Entity* releasedEnt;
};