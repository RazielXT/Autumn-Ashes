#pragma once

class PlayerAnimation
{
public:

	PlayerAnimation() {};

	virtual void start() = 0;
	virtual void update(float time) = 0;

};