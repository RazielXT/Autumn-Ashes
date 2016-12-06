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
extern bool continueExecution;

void onEnd()
{
	uiStarted = false;
	continueExecution = false;
}

bool EditorUiHandler::ensureUi()
{
	if (uiStarted)
		return false;

	if (lib.init(&sender, handleMsg, onEnd, &outputWindowHwnd, &topWindowHwnd))
	{
		uiStarted = true;
		return true;
	}

	return false;
}

void EditorUiHandler::sendMsg(UiMessage& msg)
{
	sender(&msg);
}

AppUiMessage EditorUiHandler::readUiMessage()
{
	std::unique_lock<std::mutex> lk(msgMutex);

	return AppUiMessage(storedMsg);
}

bool EditorUiHandler::isActiveUi()
{
	return uiStarted;
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
	release();
}

void AppUiMessage::release()
{
	if (!empty)
	{
		{
			std::unique_lock<std::mutex> lk(msgMutex);
			storedMsg = nullptr;
			empty = true;
		}

		cv.notify_all();
	}
}
