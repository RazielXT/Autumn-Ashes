#include "stdafx.h"
#include "PlayerAbilities.h"
#include "PlayerSliding.h"
#include "Player.h"

PlayerAbilities::PlayerAbilities(Player* player) : p(player)
{
}

PlayerAbilities::~PlayerAbilities()
{
}

void PlayerAbilities::setShiftTime()
{
    shiftTimeTarget = stateTimer;
}

void PlayerAbilities::shiftBack()
{
    if (playerHistory.empty())
        return;

    auto& targetState = playerHistory.front();

    for (auto& state : playerHistory)
    {
        if (state.timestamp < shiftTimeTarget)
            targetState = state;
        else
            break;
    }

    p->loadState(targetState);
    p->bodyVelocityL = 0;

    playerHistory.clear();
    lastTimestamp = 0;
}

void PlayerAbilities::portForward()
{
	if (p->pSliding->foundTarget())
		p->pSliding->portToTarget();
	else
	{
		//ray target

		//port to target
	}
}

void PlayerAbilities::update(float tslf)
{
	updateStateHistory(tslf);
	updatePortTarget();
}

void PlayerAbilities::updateStateHistory(float tslf)
{
	const float timeStateGaps = 0.1f;
	const float maxtime = 5;

	stateTimer += tslf;

	while (!playerHistory.empty() && (stateTimer - playerHistory.front().timestamp) > maxtime)
	{
		playerHistory.pop_front();
	}

	if ((stateTimer - lastTimestamp) > timeStateGaps)
	{
		PlayerStateInfo info;
		p->saveState(info);
		info.timestamp = lastTimestamp = stateTimer;
		playerHistory.push_back(info);
	}
}

void PlayerAbilities::updatePortTarget()
{
	if (shiftPressed)
	{
		if (!p->pSliding->showPossibleSlideTarget())
			showPortTarget();
	}
	else
	{
		p->pSliding->hideSlideTarget();
		hidePortTarget();
	}
}

void PlayerAbilities::pressedKey(const OIS::KeyEvent &arg)
{
    switch (arg.key)
    {
    case OIS::KC_LSHIFT:
        shiftPressed = true;
        break;

	case OIS::KC_F:
		portForward();
		break;

    case OIS::KC_R:
        shiftBack();
        break;

    case OIS::KC_T:
        setShiftTime();
        break;
    }
}

void PlayerAbilities::releasedKey(const OIS::KeyEvent &arg)
{
    switch (arg.key)
    {
    case OIS::KC_LSHIFT:
        shiftPressed = false;
        break;
    }
}

void PlayerAbilities::hidePortTarget()
{
	
}

void PlayerAbilities::showPortTarget()
{

}
