#pragma once

#include "EditorCommLib.h"

struct AppUiMessage;

struct EditorUiHandler
{
	bool ensureUi();

	void sendMsg(UiMessage& msg);

	AppUiMessage readUiMessage();

	bool isActiveUi();

	HWND outputWindowHwnd = 0;
	HWND topWindowHwnd = 0;

protected:

	CommLib lib;

	UI_MSG_FUNC sender;
};

struct AppUiMessage
{
	AppUiMessage(UiMessage* msg);
	~AppUiMessage();

	void release();

	bool empty;

	UiMessageId id;
	void* data;
};