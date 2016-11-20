#pragma once

#include "EditorCommLib.h"

struct AppUiMessage;

struct EditorUiHandler
{
	bool ensureUi();

	void sendMsg(UiMessage& msg);

	AppUiMessage readUiMessage();

	bool isActiveUi();

protected:

	CommLib lib;

	UI_MSG_FUNC sender;
};

struct AppUiMessage
{
	AppUiMessage(UiMessage* msg);
	~AppUiMessage();

	bool empty;

	UiMessageId id;
	void* data;
};