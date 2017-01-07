#include "stdafx.h"
#include "EventsManager.h"
#include "InputListener.h"
#include "EventTask.h"
#include "GameStateManager.h"

EventsManager::EventsManager()
{
}

void EventsManager::activatePlayerTrigger(bodyUserData* t)
{
	auto trigger = t->trigger;
	short a = trigger->playerAction;

	std::map<short, std::vector<EventTask*>>::iterator it = trigger->tasks.find(a);
	if (it != trigger->tasks.end())
	{
		for (unsigned int j = 0; j < (*it).second.size(); j++)
		{
			if ((*it).second.at(j)->start())
			{
				Global::eventsMgr->addCachedTask((*it).second.at(j));
			}
		}

		if (trigger->cooldown>0)
			Global::eventsMgr->cooldownTrigger(t);
	}
}

void EventsManager::addCachedTask(EventTask* r)
{
	currentCachedTasks.push_back(r);

	Ogre::LogManager::getSingleton().getLog("RuntimeEvents.log")->logMessage("Task added to event manager from "+Ogre::StringConverter::toString(GetCurrentThreadId())+", total: "+Ogre::StringConverter::toString(currentCachedTasks.size()),Ogre::LML_NORMAL);
}

void EventsManager::addTask(EventTask* r)
{
	auto& t = typeid(*r);
	Ogre::LogManager::getSingleton().getLog("RuntimeEvents.log")->logMessage("One time task of " + Ogre::String(t.name()) + " added to event manager, total: " + Ogre::StringConverter::toString(oneTimeTasks.size()), Ogre::LML_NORMAL);

	if (r->start())
		oneTimeTasks.push_back(r);
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
	unavailableTriggers.clear();
}

void EventsManager::update(Ogre::Real time)
{
	//update tasks
	updatingCachedTasks = currentCachedTasks;
	currentCachedTasks.clear();

	auto it = updatingCachedTasks.begin();
	auto state = Global::gameMgr->gameState;

	while(it != updatingCachedTasks.end())
	{
		if (((*it)->stateExecution != state && (*it)->stateExecution != UNDEFINED) || (*it)->update(time))
			it++;
		else
			it = updatingCachedTasks.erase(it);
	}
	currentCachedTasks.insert(currentCachedTasks.begin(), updatingCachedTasks.begin(), updatingCachedTasks.end());

	auto it2 = oneTimeTasks.begin();

	//update one time tasks
	while(it2 != oneTimeTasks.end())
	{
		if (((*it2)->stateExecution != state && (*it2)->stateExecution != UNDEFINED) || (*it2)->update(time))
			++it2;
		else
		{
			auto t = *it2;
			it2 = oneTimeTasks.erase(it2);
			delete t;
		}

	}

	if (state != GAME)
		return;

	//used triggers cooldown
	auto it3 = unavailableTriggers.begin();

	while(it3 != unavailableTriggers.end())
	{
		(*it3)->trigger->timer += time;

		if ((*it3)->trigger->timer >= (*it3)->trigger->cooldown)
		{
			(*it3)->enabledTrigger = true;
			it3 = unavailableTriggers.erase(it3);
		}
		else
			++it3;
	}
}

void EventsManager::addInputListener(InputListener* l)
{
	auto d = std::find(keyListeners.begin(), keyListeners.end(), l);
	if (d == keyListeners.end())
		keyListeners.push_back(l);
	else
		printf("");
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
		if (l && l->executionState == state || l->executionState == UNDEFINED)
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

void EventsManager::listenersMousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	auto state = Global::gameMgr->gameState;

	for (auto l : keyListeners)
	{
		if (l->executionState == state || l->executionState == UNDEFINED)
			l->mousePressed(arg, id);
	}
}

void EventsManager::listenersMouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	auto state = Global::gameMgr->gameState;

	for (auto l : keyListeners)
	{
		if (l->executionState == state || l->executionState == UNDEFINED)
			l->mouseReleased(arg, id);
	}
}