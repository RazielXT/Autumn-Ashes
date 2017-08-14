#include <stdio.h>
#include <windows.h>
#include <future>
#include <string>
#include "EditorCommLib.h"

void uiMsgCallback(UiMessage* msg)
{
	printf("Received type %d\n", (int)msg->id);

	if (msg->id == UiMessageId::SelectWorldItem)
	{
		auto msgs = (std::vector<std::wstring>*)msg->data;

		for (auto s : *msgs)
		{
			wprintf(L"Received name %s\n", s.data());
		}	
	}

	if (msg->id == UiMessageId::SelectWorldItem)
	{
		GetSceneSettingsData* data = (GetSceneSettingsData*)msg->data;

		data->lutOptions.push_back("cool");
		data->currentLutId = 0;

		data->skyboxOptions.push_back("TCENoonSkyBox");
		data->currentSkyboxId = 0;
	}
}

void uiEnd()
{
	printf("Ui end\n");
}

int main()
{
	CommLib lib;

	UI_MSG_FUNC sender;
	lib.init(&sender, uiMsgCallback, 0, 0);

	lib.h.join();

	getchar();

	return 0;
}