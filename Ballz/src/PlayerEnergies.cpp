#include "stdafx.h"
#include "PlayerAbilities.h"
#include "PlayerEnergies.h"
#include "Player.h"
#include "GameStateManager.h"
#include "GameUi.h"
#include "Energy.h"
#include "GUtils.h"

static const float energyManipulationRange = 20;
static const float chargeTime = 0.5f;
static const float chargeStaleTime = 1.0f;

PlayerEnergies::PlayerEnergies(Player* player)
{
	p = player;

	ui = Global::gameMgr->myMenu->gameUi;
	ui->clear();

	releaseEnergy = nullptr;

	releasedEnt = GUtils::MakeEntity("basicSphere.mesh");
	releasedEnt->setMaterialName("concreteMat");
	releasedEnt->getParentSceneNode()->setScale(Ogre::Vector3(0.2f));
	releasedEnt->setVisible(false);
}

PlayerEnergies::~PlayerEnergies()
{
	ui->clear();
}

void PlayerEnergies::update(float tslf)
{
	if (releaseEnergy)
	{
		if (!releaseEnergy->updateUsage(tslf))
		{
			releaseEnergy = nullptr;
			releasedEnt->setVisible(false);
		}
		else
		{
			releasedEnt->getParentSceneNode()->setPosition(getChargedEnergyPosition(releaseEnergy->getUsageTimer()));
		}
	}
	else
	{
		auto e = facingEnergy();
		auto p = facingEnergyPlacement();

		if (e)
			ui->showUseGui(e->getUsageState() == Energy::None ? Ui_GrabEnergy : Ui_GrabEnergyUnavailable);
		else if (p)
			ui->showUseGui(p->isFull() ? (p->isRemovable() ? Ui_GrabEnergy : Ui_GrabEnergyUnavailable) : (getAvailableEnergy() ? Ui_UseEnergy : Ui_UseEnergyUnavailable));
	}

	ui->showEnergyGui(currentEnergies);
}

Energy* PlayerEnergies::facingEnergy()
{
	return SceneEnergies::getAvailableEnergyFromDirection(p->getCameraPosition(), p->getFacingDirection(), std::min(p->facingWallDistance,energyManipulationRange));
}

EnergyPlacement* PlayerEnergies::facingEnergyPlacement()
{
	return SceneEnergies::getEnergyPlacementFromDirection(p->getCameraPosition(), p->getFacingDirection(), std::min(p->facingWallDistance, energyManipulationRange));
}

bool PlayerEnergies::grabEnergy()
{
	auto e = facingEnergy();

	if (e && e->getUsageState() == Energy::None)
	{
		e->absorb();
		currentEnergies.push_back(e);
	}
	else
	{
		auto p = facingEnergyPlacement();

		if (p && p->isRemovable())
		{
			e = p->removeEnergy();
			e->absorb();
			currentEnergies.push_back(e);
		}
	}

	return e;
}

bool PlayerEnergies::pressedKey(const OIS::KeyEvent &arg)
{
	switch (arg.key)
	{
	case OIS::KC_E:
	{
		releaseEnergy = getAvailableEnergy();

		if (releaseEnergy)
		{
			releaseEnergy->startUsage(chargeTime + chargeStaleTime, Energy::Charging);
			releasedEnt->setVisible(true);
		}

		return releaseEnergy;
	}
	}

	return false;
}

Ogre::Vector3 PlayerEnergies::getChargedEnergyPosition(float time)
{
	return p->getCameraPosition() + p->getFacingDirection() * std::min(chargeTime, time) / chargeTime * std::min(p->facingWallDistance, energyManipulationRange);
}

bool PlayerEnergies::releasedKey(const OIS::KeyEvent &arg)
{
	if (arg.key == OIS::KC_E)
	{
		if (releaseEnergy)
		{
			releaseEnergy->hover(getChargedEnergyPosition(releaseEnergy->getUsageTimer()));
			releaseEnergy->startUsage(energyRechargeTime, Energy::Recharging, true);

			removeEnergy(releaseEnergy);
			releaseEnergy = nullptr;
			releasedEnt->setVisible(false);

			return true;
		}
	}

	return false;
}

Energy* PlayerEnergies::getAvailableEnergy()
{
	for (size_t i = 0; i < currentEnergies.size(); i++)
	{
		if (currentEnergies[i]->getUsageState() == Energy::None)
			return currentEnergies[i];
	}

	return nullptr;
}

void PlayerEnergies::removeEnergy(Energy* e)
{
	for (size_t i = 0; i < currentEnergies.size(); i++)
		if (currentEnergies[i] == e)
			currentEnergies.erase(currentEnergies.begin() + i);
}

bool PlayerEnergies::pressedMouse(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	if (id == OIS::MB_Right)
	{
		return grabEnergy();
	}
	if (id == OIS::MB_Left)
	{
		auto e = getAvailableEnergy();

		if (e)
		{
			auto p = facingEnergyPlacement();

			if (p && !p->isFull())
			{
				p->addEnergy(e);
				e->place();
				e->startUsage(2.0f, Energy::Recharging, true);
				removeEnergy(e);

				return true;
			}
		}
	}

	return false;
}

bool PlayerEnergies::releasedMouse(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	return false;
}
