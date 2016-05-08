#include "stdafx.h"
#include "PlayerAbilities.h"
#include "PlayerSliding.h"
#include "Player.h"
#include "MUtils.h"
#include "GameStateManager.h"
#include "GameUi.h"
#include "PlayerEnergies.h"
#include "Energy.h"

PlayerAbilities::PlayerAbilities(Player* player) : p(player), pEnergies(player), flash(player, &pEnergies), timeshift(player, &pEnergies)
{
}

PlayerAbilities::~PlayerAbilities()
{
}

void PlayerAbilities::update(float tslf)
{
	pEnergies.update(tslf);

	timeshift.updateStateHistory(tslf);
	flash.update(tslf);
}

bool PlayerAbilities::pressedKey(const OIS::KeyEvent &arg)
{
	if (pEnergies.pressedKey(arg))
		return true;

	switch (arg.key)
	{
	case OIS::KC_F:
		return flash.portForward();

	case OIS::KC_R:
		timeshift.shiftBack();
		break;

	default:
		return false;
	}

	return true;
}

bool PlayerAbilities::releasedKey(const OIS::KeyEvent &arg)
{
	if (pEnergies.releasedKey(arg))
		return true;

	return false;
}

bool PlayerAbilities::pressedMouse(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	return (pEnergies.pressedMouse(arg, id));
}

bool PlayerAbilities::releasedMouse(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	return (pEnergies.releasedMouse(arg, id));
}


bool PlayerFlash::portForward()
{
	auto target = pEnergies->facingEnergy();
	auto e = pEnergies->getAvailableEnergy();

	if (target && e)
	{
		target->startUsage(energyRechargeTime, Energy::Recharging, true);
		e->startUsage(energyRechargeTime, Energy::Recharging, true);

		portingTimer = 0;

		Global::camera->detachCamera(cameraPortNode);

		portStartPos = cameraPortNode->getPosition();
		portTargetPos = target->position;

		return true;
	}

	return false;
}

void PlayerFlash::update(float tslf)
{
	if (portingTimer >= 0)
	{
		const float portTime = 0.1f;
		portingTimer += tslf;

		if (portingTimer >= portTime)
		{
			Global::mPPMgr->vars.radialHorizBlurVignette.x = 0;
			Global::camera->attachCamera(true);
			p->bodyPosition = portTargetPos;
			p->body->setPositionOrientation(portTargetPos, Ogre::Quaternion::IDENTITY);
			p->body->setVelocity(p->facingDir * 10);
			portingTimer = -1;
		}
		else
		{
			Global::mPPMgr->vars.radialHorizBlurVignette.x = 1;
			cameraPortNode->setPosition(MUtils::lerp(portStartPos, portTargetPos, portingTimer / portTime));
		}
	}
}

PlayerFlash::PlayerFlash(Player* player, PlayerEnergies* energies)
{
	p = player;
	pEnergies = energies;
	cameraPortNode = Global::mSceneMgr->getRootSceneNode()->createChildSceneNode();
}
