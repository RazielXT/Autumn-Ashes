#pragma once

#include "stdafx.h"
#include "TestTask.h"
#include "EntityMovementTask.h"
#include "CommonTasks.h"
#include "ChainsSoundTask.h"
#include "EffectsTasks.h"
#include "lvl1Tasks.h"

float getTaskDelay(Ogre::String& fname);
EventTask* makeTask(Ogre::String fname, float delay = 0);
