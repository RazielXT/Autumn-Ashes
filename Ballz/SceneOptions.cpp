#include "stdafx.h"
#include "SceneOptions.h"
#include "EditorComm.h"

void SceneOptions::getCurrentSceneInfo(GetSceneSettingsData* data)
{
	auto luts = Global::mPPMgr->getColorGradingPresets();

	for (auto& lut : luts)
		data->lutOptions.push_back(std::wstring(lut.begin(), lut.end()));

	data->currentLutId = 0;
	for (size_t i = 0; i < luts.size(); i++)
		if (luts[i] == Global::mPPMgr->getCurrentGradingPreset())
			data->currentLutId = (int) i;

	data->skyboxOptions.push_back(L"TCENoonSkyBox");
	data->currentSkyboxId = 0;
}

void SceneOptions::editScene(SceneSettingsChange* change)
{
	if(change->change == SceneSettingsChange::SceneChange::Lut)
		Global::mPPMgr->setColorGradingPreset(wtos(change->data));
	if (change->change == SceneSettingsChange::SceneChange::Skybox)
		Global::mSceneMgr->setSkyBox(true, wtos(change->data));
}
