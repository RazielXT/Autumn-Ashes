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

    void pressedKey(const OIS::KeyEvent &arg);

};