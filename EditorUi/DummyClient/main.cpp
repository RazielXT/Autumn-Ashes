#include <stdio.h>
#include <windows.h>
#include <future>
#include <string>
#include "CommLib.h"

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
}

void uiEnd()
{
	printf("Ui end\n");
}

int main()
{
	CommLib lib;

	UI_MSG_FUNC sender;
	lib.init(&sender, uiMsgCallback, uiEnd);

	lib.h.join();

	getchar();

	return 0;
}