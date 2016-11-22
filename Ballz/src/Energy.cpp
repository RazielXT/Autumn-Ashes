#include "stdafx.h"
#include "Energy.h"
#include "GUtils.h"
#include "MUtils.h"

Energy::Energy()
{
}

void Energy::deinit()
{
	ent = nullptr;
}

void Energy::init()
{
	ent = GUtils::MakeEntity("basicSphere.mesh");
	ent->setMaterialName("concreteMat");

	usage.stateTime = 0;
	usage.timer = 0;
	usage.state = None;
}

void Energy::hover(Ogre::Vector3 pos)
{
	position = pos;
	state = Hovering;

	ent->setVisible(true);
	ent->getParentSceneNode()->setPosition(pos);
}

void Energy::place()
{
	state = Placed;
	ent->setVisible(false);
}

void Energy::absorb()
{
	state = Absorbed;
	ent->setVisible(false);
}

bool Energy::updateUsage(float time)
{
	usage.timer += time;

	if (usage.timer > usage.stateTime)
	{
		stopUsage();
		return false;
	}

	return true;
}

void Energy::startUsage(float chargeTime, UsageState state, bool autoupdate)
{
	usage.timer = 0;
	usage.stateTime = chargeTime;
	usage.state = state;

	if (autoupdate)
		Global::eventsMgr->addTask(new EnergyRecharge(this));
}

void Energy::stopUsage()
{
	usage.timer = usage.stateTime = 0;
	usage.state = None;
}

namespace SceneEnergies
{
static EnergyPlacement placements[20];
static int placementsCount;

static Energy energies[20];
static int count = 0;

Energy* createEnergy()
{
	count++;
	energies[count - 1].init();

	return &energies[count - 1];
}

EnergyPlacement* createEnergyPlacement(Ogre::Vector3 pos, Ogre::Vector3 dir, std::function<void()> addCallback, std::function<void()> removeCallback)
{
	placements[placementsCount].pos = pos;
	placements[placementsCount].dir = dir;
	placements[placementsCount].addCallback = addCallback;
	placements[placementsCount].removeCallback = removeCallback;
	placements[placementsCount].storedEnergy = 0;

	placementsCount++;

	return &placements[placementsCount - 1];
}

void reset()
{
	for (int i = 0; i < count; i++)
	{
		energies[i].deinit();
	}

	count = 0;
	placementsCount = 0;
}

Energy* getAvailableEnergyFromDirection(Ogre::Vector3 pos, Ogre::Vector3 dir, float rayLength)
{
	float closest = rayLength*rayLength + 5;
	Energy* found = nullptr;

	for (int i = 0; i < count; i++)
	{
		float pDist = energies[i].position.squaredDistance(pos);

		if (pDist < closest && energies[i].isHovering() && energies[i].getUsageState() == Energy::None)
		{
			auto p = MUtils::getProjectedPointOnLine(energies[i].position, pos, pos + dir*rayLength);

			if (p.sqMinDistance < 5)
			{
				closest = pDist;
				found = &energies[i];
			}
		}
	}

	return found;
}

EnergyPlacement* getEnergyPlacementFromDirection(Ogre::Vector3 pos, Ogre::Vector3 dir, float rayLength)
{
	float closest = rayLength*rayLength + 5;
	EnergyPlacement* found = nullptr;

	for (int i = 0; i < count; i++)
	{
		float pDist = placements[i].pos.squaredDistance(pos);

		if (pDist < closest && placements[i].dir.dotProduct(dir) <= 0)
		{
			auto p = MUtils::getProjectedPointOnLine(placements[i].pos, pos, pos + dir*rayLength);

			if (p.sqMinDistance < 5)
			{
				closest = pDist;
				found = &placements[i];
			}
		}
	}

	return found;
}

}


void EnergyPlacement::addEnergy(Energy* e)
{
	storedEnergy = e;

	addCallback();
}

Energy* EnergyPlacement::removeEnergy()
{
	Energy* e = storedEnergy;

	storedEnergy = nullptr;

	removeCallback();

	return e;
}

bool EnergyPlacement::isFull()
{
	return storedEnergy != nullptr;
}

bool EnergyPlacement::isRemovable()
{
	return storedEnergy != nullptr && storedEnergy->getUsageState() != Energy::Recharging;
}

