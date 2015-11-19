#pragma once

#include "stdafx.h"
#include "CameraShaker.h"
#include "PlayerStateInfo.h"

class CameraJump
{
public:
	CameraJump();

	void saveState(PlayerStateInfo& info);
	void loadState(PlayerStateInfo& info);

	bool update(float tslf);

private:

	Player* p;
};

