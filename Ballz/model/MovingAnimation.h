#pragma once

#include "PlayerAnimation.h"

class MovingAnimation : public PlayerAnimation
{
public:

	MovingAnimation();

	void start() override;

	void update(float time) override;

};