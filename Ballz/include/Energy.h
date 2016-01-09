#pragma once

#include "stdafx.h"
#include "EventTask.h"

class Energy
{
public:

    Energy();
    void init();
    void deinit();

    void hover(Ogre::Vector3 pos);
    void absorb();
    void place();

    inline bool isAbsorbed()
    {
        return state == Absorbed;
    };

    inline bool isHovering()
    {
        return state == Hovering;
    };

    enum UsageState { None, Flashback, Charging, Recharging };
    void startUsage(float chargeTime, UsageState usage, bool autoupdate = false);
    bool updateUsage(float time);
    void stopUsage();

    inline UsageState getUsageState()
    {
        return usage.state;
    };

    inline float getUsageTimer()
    {
        return usage.timer;
    };

    Ogre::Vector3 position;

private:

    enum { Absorbed, Hovering, Placed } state;

    struct
    {
        float stateTime;
        float timer;
        UsageState state;
    } usage;

    Ogre::Entity* ent;
};

class EnergyRecharge : public EventTask
{
public:

    EnergyRecharge(Energy* energy) : e(energy) {};

    bool update(Ogre::Real tslf)
    {
        return e->updateUsage(tslf);
    }

private:

    Energy* e;
};

struct EnergyPlacement
{
    void addEnergy(Energy* e);
    Energy* removeEnergy();
    bool isFull();
    bool isRemovable();

    Ogre::Vector3 pos;
    Ogre::Vector3 dir;

    std::function<void()> addCallback;
    std::function<void()> removeCallback;
    Energy* storedEnergy;
};

namespace SceneEnergies
{
extern EnergyPlacement* createEnergyPlacement(Ogre::Vector3 pos, Ogre::Vector3 dir, std::function<void()> addCallback, std::function<void()> removeCallback);
extern Energy* createEnergy();
extern void reset();
extern Energy* getAvailableEnergyFromDirection(Ogre::Vector3 pos, Ogre::Vector3 dir, float rayLength);
extern EnergyPlacement* getEnergyPlacementFromDirection(Ogre::Vector3 pos, Ogre::Vector3 dir, float rayLength);
};