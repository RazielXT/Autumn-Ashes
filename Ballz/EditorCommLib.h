#pragma once
#include "EditorComm.h"
#include <winbase.h>
#include <future>

typedef void(*UI_MSG_FUNC)(UiMessage*);
typedef void(*VOID_FUNC)();
typedef void(*HWND_FUNC)(HWND*, HWND*);
typedef void(*UI_MSG_CALLBCK_FUNC)(UI_MSG_FUNC);

struct CommLib
{
	HMODULE lib = nullptr;
	std::thread h;

	~CommLib()
	{
		if (lib)
			FreeLibrary(lib);

		if (h.joinable())
			h.detach();
	}

	bool init(UI_MSG_FUNC* uiSender, UI_MSG_FUNC appReceiver, VOID_FUNC onEndCallback, HWND* targetHwnd, HWND* topHwnd)
	{
		if (!lib)
		{
			lib = LoadLibrary("libEditorUi.dll");

			if (!lib)
				return false;
		}

		auto StartUi = (HWND_FUNC)GetProcAddress(lib, "StartUi");
		auto SetUiFunc = (UI_MSG_CALLBCK_FUNC)GetProcAddress(lib, "SetMsgFunc");
		*uiSender = (UI_MSG_FUNC)GetProcAddress(lib, "SendMsg");

		if (!StartUi || !SetUiFunc || !*uiSender)
			return false;

		if (h.joinable())
			h.detach();

		h = std::thread([&]()
		{

			StartUi(targetHwnd, topHwnd);

			onEndCallback();

		});

		Sleep(1000);

		if (SetUiFunc)
			SetUiFunc(appReceiver);

		return true;
	}

};
