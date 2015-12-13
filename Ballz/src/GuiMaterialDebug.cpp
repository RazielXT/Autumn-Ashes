#include "stdafx.h"
#include "GuiMaterialDebug.h"

void GuiMaterialDebug::initUi(Gorilla::Layer* layer)
{
	for (size_t i = 0; i < MAX_DEBUG_LINES; i++)
	{
		debugMaterialCaption[i] = layer->createCaption(48, 50, 700 + 5 + 20 * (float)i, "");
		debugMaterialCaption[i]->size(1500, 50);
		debugMaterialCaption[i]->setScale(0.3f);
		debugMaterialCaption[i]->align(Gorilla::TextAlign_Left);
	}

	for (size_t i = 0; i < DEBUG_VARIABLES_COUNT; i++)
	{
		debugMaterialCaption[i] = layer->createCaption(48, 1450, 700 + 150 + 20 * (float)i, "");
		debugMaterialCaption[i]->size(500, 50);
		debugMaterialCaption[i]->setScale(0.3f);
		debugMaterialCaption[i]->align(Gorilla::TextAlign_Left);
	}
}