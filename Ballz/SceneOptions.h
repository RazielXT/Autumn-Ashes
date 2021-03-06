#pragma once
#include "stdafx.h"

struct GetSceneSettingsData;
struct SceneSettingsChange;

class SceneOptions
{
public:

	void getCurrentSceneInfo(GetSceneSettingsData* data);

	void editScene(SceneSettingsChange* change);

	void reloadShaders();

	float fpsCap = 30;
};