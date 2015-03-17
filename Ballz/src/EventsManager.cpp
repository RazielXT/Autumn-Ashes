#include "stdafx.h"
#include "EventsManager.h"
#include "InputListener.h"

DelayedTask::DelayedTask(EventTask* r, float timer)
{
    this->r = r;
    this->timer = timer;
}

EventsManager::EventsManager()
{
};

void EventsManager::activatePlayerTrigger(bodyUserData* t)
{
    auto trigger = t->trigger;
    short a = trigger->playerAction;

    std::map<short, std::vector<EventTask*>>::iterator it = trigger->tasks.find(a);
    if (it != trigger->tasks.end())
    {
        for (unsigned int j = 0; j < (*it).second.size(); j++)
        {
            if ((*it).second.at(j)->taskDelay || (*it).second.at(j)->start())
            {
                Global::mEventsMgr->addCachedTask((*it).second.at(j));
            }
        }

        if (trigger->cooldown>0)
            Global::mEventsMgr->cooldownTrigger(t);
    }
}

void EventsManager::addCachedTask(EventTask* r)
{
    if(r->taskDelay)
    {
        DelayedTask dr(r,r->taskDelay);
        currentDelayedCachedTasks.push_back(dr);
        return;
    }

    currentCachedTasks.push_back(r);

    Ogre::LogManager::getSingleton().getLog("RuntimeEvents.log")->logMessage("Task added to event manager from "+Ogre::StringConverter::toString(GetCurrentThreadId())+", total: "+Ogre::StringConverter::toString(currentCachedTasks.size()),Ogre::LML_NORMAL);
}

void EventsManager::addTask(EventTask* r)
{
    auto& t = typeid(*r);
    Ogre::LogManager::getSingleton().getLog("RuntimeEvents.log")->logMessage("One time task of " + Ogre::String(t.name()) + " added to event manager, total: " + Ogre::StringConverter::toString(oneTimeTasks.size()), Ogre::LML_NORMAL);

    if(r->taskDelay)
    {
        DelayedTask dr(r,r->taskDelay);
        oneTimeDelayedTasks.push_back(dr);
    }
    else if (r->start())
        oneTimeTasks.push_back(r);
    else
        delete r;
}

void EventsManager::cooldownTrigger(bodyUserData* t)
{
    t->trigger->timer=0;
    t->enabledTrigger=false;
    unavailableTriggers.push_back(t);
}

void EventsManager::clear()
{
    oneTimeTasks.clear();
    currentCachedTasks.clear();
    oneTimeDelayedTasks.clear();
    currentDelayedCachedTasks.clear();
    unavailableTriggers.clear();
}

void EventsManager::update(Ogre::Real time)
{
    //update tasks
    std::list<EventTask*>::iterator iter = currentCachedTasks.begin();
    auto state = Global::gameMgr->gameState;

    while(iter != currentCachedTasks.end())
    {
        if (((*iter)->stateExecution != state && (*iter)->stateExecution != UNDEFINED) || (*iter)->update(time))
            iter++;
        else
            iter = currentCachedTasks.erase(iter);
    }

    iter = oneTimeTasks.begin();

    //update one time tasks
    while(iter != oneTimeTasks.end())
    {
        if (((*iter)->stateExecution != state && (*iter)->stateExecution != UNDEFINED) || (*iter)->update(time))
            ++iter;
        else
        {
            EventTask* r = (*iter);
            iter = oneTimeTasks.erase(iter);
            delete r;
        }
    }

    if (state != GAME)
        return;

    //used triggers cooldown
    std::list<bodyUserData*>::iterator iter2 = unavailableTriggers.begin();

    while(iter2 != unavailableTriggers.end())
    {
        (*iter2)->trigger->timer += time;

        if ((*iter2)->trigger->timer >= (*iter2)->trigger->cooldown)
        {
            (*iter2)->enabledTrigger = true;
            iter2 = unavailableTriggers.erase(iter2);
        }
        else
            ++iter2;
    }


    //update delay of tasks
    auto iter3 = currentDelayedCachedTasks.begin();

    while(iter3 != currentDelayedCachedTasks.end())
    {
        (*iter3).timer -= time;

        if ((*iter3).timer <= 0)
        {
            if ((*iter3).r->start())
                currentCachedTasks.push_back((*iter3).r);
            iter3 = currentDelayedCachedTasks.erase(iter3);
        }
        else
            iter3++;
    }

    iter3 = oneTimeDelayedTasks.begin();

    while(iter3 != oneTimeDelayedTasks.end())
    {
        (*iter3).timer-=time;

        if ((*iter3).timer<=0)
        {
            if((*iter3).r->start())
                oneTimeTasks.push_back((*iter3).r);
            iter3 = oneTimeDelayedTasks.erase(iter3);
        }
        else
            iter3++;
    }

}

void EventsManager::addInputListener(InputListener* l)
{
    keyListeners.push_back(l);
}

void EventsManager::removeInputListener(InputListener* l)
{
    auto d = std::find(keyListeners.begin(), keyListeners.end(), l);
    if (d != keyListeners.end())
        keyListeners.erase(d);
}

void EventsManager::listenersKeyReleased(const OIS::KeyEvent &arg)
{
    auto state = Global::gameMgr->gameState;

    for (auto l : keyListeners)
    {
        if (l->executionState == state || l->executionState == UNDEFINED)
            l->releasedKey(arg);
    }
}

void EventsManager::listenersKeyPressed(const OIS::KeyEvent &arg)
{
    auto state = Global::gameMgr->gameState;

    for (auto l : keyListeners)
    {
        if (l->executionState == state || l->executionState == UNDEFINED)
            l->pressedKey(arg);
    }
}

void EventsManager::listenersMouseMoved(const OIS::MouseEvent &e)
{
    auto state = Global::gameMgr->gameState;

    for (auto l : keyListeners)
    {
        if (l->executionState == state || l->executionState == UNDEFINED)
            l->movedMouse(e);
    }
}
