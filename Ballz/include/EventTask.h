#pragma once

#include <vector>

class EventTask
{
public:

	EventTask() {};
	virtual ~EventTask() {};

	virtual void setUserData(void* data);
	virtual bool start();
	virtual bool update(float tslf);

	GameState stateExecution = GAME;
};

class DelayedEventTask
{
public:

	DelayedEventTask() {};
	virtual ~DelayedEventTask() {};

protected:

	void startDelay();
	bool checkDelay(float tslf);
	float delay = 0;

private:

	float timer = 0;
};
