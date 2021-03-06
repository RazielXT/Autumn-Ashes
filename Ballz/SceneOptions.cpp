#include "stdafx.h"
#include "SceneOptions.h"
#include "EditorComm.h"

void SceneOptions::getCurrentSceneInfo(GetSceneSettingsData* data)
{
	auto luts = Global::ppMgr->getColorGradingPresets();

	for (auto& lut : luts)
		data->lutOptions.push_back(lut);

	data->currentLutId = 0;
	for (size_t i = 0; i < luts.size(); i++)
		if (luts[i] == Global::ppMgr->getCurrentGradingPreset())
			data->currentLutId = (int) i;

	data->skyboxOptions.push_back("TCENoonSkyBox");
	data->currentSkyboxId = 0;
}

void SceneOptions::editScene(SceneSettingsChange* change)
{
	if(change->change == SceneSettingsChange::SceneChange::Lut)
		Global::ppMgr->setColorGradingPreset(*(std::string*)change->data);
	if (change->change == SceneSettingsChange::SceneChange::Skybox)
		Global::sceneMgr->setSkyBox(true, *(std::string*)change->data);
}

void SceneOptions::reloadShaders()
{
	auto it = Ogre::HighLevelGpuProgramManager::getSingleton().getResourceIterator();

	while (it.hasMoreElements())
	{
		Ogre::HighLevelGpuProgramPtr gpuProgram = it.getNext();
		gpuProgram->reload();
	}
}

