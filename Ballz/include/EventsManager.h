#ifndef EVENTMGR_H
#define EVENTMGR_H

#include "stdafx.h"
#include <vector>
#include "EventTask.h"
#include "BodyUserData.h"

struct DelayedTask
{
    DelayedTask(EventTask* r, float timer);

    EventTask* r;
    float timer;
};

class EventsManager
{
private:

    std::list<EventTask*> currentCachedTasks;
    std::list<EventTask*> oneTimeTasks;
    std::list<DelayedTask> currentDelayedCachedTasks;
    std::list<DelayedTask> oneTimeDelayedTasks;
    std::list<bodyUserData*> unavailableTriggers;

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
};

#endif