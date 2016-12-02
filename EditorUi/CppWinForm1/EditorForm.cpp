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

	/*if(!hwnd)
		Application::Run(%form);
	else
	{
		
		System::Windows::Forms::NativeWindow^ nativeWindow;

		try
		{
			System::IntPtr handle = System::IntPtr(hwnd);
			nativeWindow = gcnew System::Windows::Forms::NativeWindow();
			nativeWindow->AssignHandle(handle);
		}
		catch (const std::exception&)
		{
			MessageBox::Show("Init failure");
			return;
		}

		try
		{
			form.Show(nativeWindow);
			Application::Run(%form);
		}
		catch (const std::exception&)
		{
			MessageBox::Show("Show failure");
			return;
		}
	}	*/	
}

void FormsUpdate(UiMessage* msg)
{
	if (msg->id == UiMessageId::HideSelectionInfo)
		CppWinForm1::EditorForm::instance->hideItemInfo();
	if (msg->id == UiMessageId::ShowSelectionInfo)
		CppWinForm1::EditorForm::instance->showItemInfo((SelectionInfo*)msg->data);
	if (msg->id == UiMessageId::ShowMouse)
		::Cursor::Show();
	if (msg->id == UiMessageId::HideMouse)
		::Cursor::Hide();
}

#ifdef NO_LIB

int Main()
{
	HWND h;
	FormsMain(0,0);

	return 0;
}

void SendMsg(UiMessageId id, void* vdata)
{
	if (id == UiMessageId::GetSceneSettings)
	{
		GetSceneSettingsData* data = (GetSceneSettingsData*)vdata;

		data->lutOptions.push_back(std::wstring(L"cool"));
		data->currentLutId = 0;

		data->skyboxOptions.push_back(L"TCENoonSkyBox");
		data->currentSkyboxId = 0;
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
