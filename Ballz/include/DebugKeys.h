#pragma once

#include "stdafx.h"
#include "InputListener.h"
#include "PostProcessMgr.h"
#include "EffectsTasks.h"


class DebugKeys : public InputListener
{
public:
    DebugKeys()
    {
        executionState = UNDEFINED;
    }

	void reloadVariables();

    void pressedKey(const OIS::KeyEvent &arg);

	struct DebugVar
	{
		std::string name;
		float* target;
		float step;

		DebugVar(std::string n, float* tar, float st) : name(n), target(tar), step(st)
		{}
	};

	std::vector<DebugVar> debugVars;
	int debugVarsLine = 0;
};