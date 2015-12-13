#pragma once

#include "Gorilla.h"

class GuiMaterialDebug
{
public:

	GuiMaterialDebug();
	~GuiMaterialDebug()
	{
	}

	void initUi(Gorilla::Layer* layer);

private:

	Gorilla::Caption*       debugMaterialCaption[MAX_DEBUG_LINES];
	Gorilla::Caption*       debugMaterialVarCaption[DEBUG_VARIABLES_COUNT];

};
