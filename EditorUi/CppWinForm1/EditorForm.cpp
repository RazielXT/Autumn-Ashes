#include "EditorForm.h"
#include "..\..\Ballz\EditorComm.h"
#include <vector>
#include <string>

using namespace System;
using namespace System::Windows::Forms;


[STAThread]
void FormsMain(HWND* hwnd, HWND* parent)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	CppWinForm1::EditorForm form;

	if (hwnd)
		*hwnd = form.getRenderWindowHandle();

	if (parent)
		*parent = form.getTopWindowHandle();

	Application::Run(%form);
}

void FormsUpdate(UiMessage* msg)
{
	if (msg->id == UiMessageId::HideSelectionInfo)
		CppWinForm1::EditorForm::instance->hideItemInfo();
	if (msg->id == UiMessageId::ShowSelectionInfo)
		CppWinForm1::EditorForm::instance->showItemInfo((SelectionInfo*)msg->data);
	if (msg->id == UiMessageId::SelectionInfoChanged)
		CppWinForm1::EditorForm::instance->changeItemInfo((SelectionInfoChange*)msg->data);
	if (msg->id == UiMessageId::ShowMouse)
		::Cursor::Show();
	if (msg->id == UiMessageId::HideMouse)
		::Cursor::Hide();
	if (msg->id == UiMessageId::StartLoading)
		CppWinForm1::EditorForm::instance->setLoading(true);
	if (msg->id == UiMessageId::EndLoading)
		CppWinForm1::EditorForm::instance->setLoading(false);
	if (msg->id == UiMessageId::SetEditorProperties)
		CppWinForm1::EditorForm::instance->setProperties((EditorProperties*)msg->data);
}

#ifdef NO_LIB

int Main()
{
	FormsMain(0,0);

	return 0;
}

void SendMsg(UiMessageId id, void* vdata)
{
	if (id == UiMessageId::GetSceneSettings)
	{
		GetSceneSettingsData* data = (GetSceneSettingsData*)vdata;

		data->lutOptions.push_back(std::string("cool"));
		data->currentLutId = 0;

		data->skyboxOptions.push_back("TCENoonSkyBox");
		data->currentSkyboxId = 0;
	}
	else if(id == UiMessageId::SelectionInfoChanged)
	{
		SelectionInfoChange data;
		EditableParamsResponse resp;
		data.change = SelectionInfoChange::Id::SendParams;
		data.data = &resp;

		//if (data->change == SelectionInfoChange::Id::GetParams)
		{
			auto& paramsData = resp.params;

			EditableParams params;
			params.groupName = "Hello";

			for (size_t i = 0; i < 15; i++)
			{
				/*EditableParam param;
				param.size = 2;
				param.buffer[0] = 5;
				param.buffer[1] = 10;
				param.group = params.groupName;
				param.step = 0.1;
				param.name = "World" + std::to_string(i);
				params.params.push_back(param);*/
			}

			paramsData.push_back(params);
		}

		//CppWinForm1::EditorForm::instance->changeItemInfo(&data);
	}
	else if (id == UiMessageId::ShowSelectionInfo)
	{
		UiMessage msg;
		msg.id = UiMessageId::ShowSelectionInfo;
		SelectionInfo info;
		EntitySelectionInfo eInfo;

		info.name = "name";
		info.pos = {0,0,0};
		info.scale = { 1,1,1 };

		eInfo.animNames.push_back("anim");
		info.typeData = &eInfo;
		info.usePaint = false;
		info.hasParams = true;

		info.type = ItemType::Entity;
		msg.data = &info;
	
		FormsUpdate(&msg);
	}
}

#else

#pragma managed(push, off)

typedef void(*MSGFUNC)(UiMessage*);
extern "C"
{
	__declspec(dllexport) void __cdecl StartUi(HWND*, HWND*);

	__declspec(dllexport) void __cdecl SendMsg(UiMessage*);

	__declspec(dllexport) void __cdecl SetMsgFunc(MSGFUNC);
}

MSGFUNC sendMsg;

__declspec(dllexport) void __cdecl SetMsgFunc(MSGFUNC func)
{
	sendMsg = func;
}

__declspec(dllexport) void __cdecl SendMsg(UiMessage* msg)
{
	FormsUpdate(msg);
}

__declspec(dllexport) void __cdecl StartUi(HWND* hwnd, HWND* parent)
{
	FormsMain(hwnd, parent);
}

int __stdcall Main(HINSTANCE h, ULONG ulReason, PVOID pvReserved) {

	switch (ulReason)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
#pragma managed(pop)

void SendMsg(UiMessageId id, void* data)
{
	UiMessage msg{ id, data };
	sendMsg(&msg);
}

#endif // NO_LIB
