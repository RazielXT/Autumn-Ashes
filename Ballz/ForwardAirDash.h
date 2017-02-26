#pragma once

#include "stdafx.h"
#include "EffectsTasks.h"

class ForwardAirDash : public EventTask
{
public:

	ForwardAirDash();

	bool start();
	bool update(Ogre::Real tslf);

private:

	bool dashed;
	float timer;
	FovPeakEffect fovEffect;
};