#ifndef MEVENTR_H
#define MEVENTR_H

#include "stdafx.h"
#include "TestTask.h"
#include "HandFromGroundTask.h"
#include "HandGripTask.h"
#include "EntityMovementTask.h"
#include "CommonTasks.h"
#include "ChainsSoundTask.h"
#include "EffectsTasks.h"
#include "lvl1Tasks.h"

float getTaskDelay(Ogre::String& fname);
EventTask* makeTask(Ogre::String fname, float delay = 0);


#endif //MEVENTR_H