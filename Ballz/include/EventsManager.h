#ifndef EVENTMGR_H
#define EVENTMGR_H

#include <vector>
#include "EventTask.h"
#include "BodyUserData.h"

class InputListener;
struct bodyUserData;

class EventsManager
{
private:

	std::vector<EventTask*> updatingCachedTasks;
	std::vector<EventTask*> currentCachedTasks;
	std::vector<EventTask*> oneTimeTasks;
	std::vector<bodyUserData*> unavailableTriggers;

	std::vector<InputListener*> keyListeners;

public:

	EventsManager();

	void activatePlayerTrigger(bodyUserData* trigger);

	void addCachedTask(EventTask* r);

	void addTask(EventTask* r);

	void cooldownTrigger(bodyUserData* t);

	void clear();

	void update(Ogre::Real time);

	void addInputListener(InputListener* l);

	void removeInputListener(InputListener* l);

	void listenersKeyPressed(const OIS::KeyEvent &arg);

	void listenersKeyReleased(const OIS::KeyEvent &arg);

	void listenersMouseMoved(const OIS::MouseEvent &e);

	void listenersMousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

	void listenersMouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
};

#endif