#include "stdafx.h"
#include "EditorUiHandler.h"

std::mutex msgMutex;
std::condition_variable cv;
UiMessage* storedMsg;

void handleMsg(UiMessage* msg)
{
	std::unique_lock<std::mutex> lk(msgMutex);
	storedMsg = msg;

	if (!cv.wait_for(lk, std::chrono::seconds(1), [] {return storedMsg == nullptr; }))
		storedMsg = nullptr;
}

bool uiStarted = false;

void onEnd()
{
	uiStarted = false;
}

extern size_t hwnd;

bool EditorUiHandler::ensureUi()
{
	if (uiStarted)
		return false;

	if (lib.init(&sender, handleMsg, onEnd, (HWND)hwnd))
	{
		uiStarted = true;
		return true;
	}

	return false;
}

void EditorUiHandler::sendMsg(UiMessage* msg)
{
	sender(msg);
}

AppUiMessage EditorUiHandler::readUiMessage()
{
	std::unique_lock<std::mutex> lk(msgMutex);

	return AppUiMessage(storedMsg);
}

AppUiMessage::AppUiMessage(UiMessage* msg)
{
	empty = msg == nullptr;

	if (msg)
	{
		id = msg->id;
		data = msg->data;
	}
}

AppUiMessage::~AppUiMessage()
{
	if (!empty)
	{
		{
			std::unique_lock<std::mutex> lk(msgMutex);
			storedMsg = nullptr;
		}

		cv.notify_all();
	}
}
