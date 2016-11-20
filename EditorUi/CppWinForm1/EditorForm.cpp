#include "EditorForm.h"
#include "..\..\Ballz\EditorComm.h"
#include <vector>
#include <string>

using namespace System;
using namespace System::Windows::Forms;


[STAThread]
void FormsMain(HWND hwnd)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	CppWinForm1::EditorForm form;

	if(!hwnd)
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


		//System::IntPtr myWindowHandle = System::IntPtr(hwnd);
		//System::Windows::Forms::IWin32Window ^w = System::Windows::Forms::Control::FromHandle(myWindowHandle);
		try
		{
			form.Show(nativeWindow);
			//form.SetDesktopLocation(0, 0);
			Application::Run(%form);
		}
		catch (const std::exception&)
		{
			MessageBox::Show("Show failure");
			return;
		}

		//MessageBox::Show("No failure");
	}		
}

void FormsUpdate(UiMessage* msg)
{
	if (msg->id == UiMessageId::HideSelectionInfo)
		CppWinForm1::EditorForm::instance->hideItemInfo();
	if (msg->id == UiMessageId::ShowSelectionInfo)
		CppWinForm1::EditorForm::instance->showItemInfo((SelectionInfo*)msg->data);
}

#ifdef NO_LIB

int Main()
{
	FormsMain(NULL);

	return 0;
}

void SendMsg(UiMessageId, void*)
{
}

#else

#pragma managed(push, off)

typedef void(*MSGFUNC)(UiMessage*);
extern "C"
{
	__declspec(dllexport) void __cdecl StartUi(HWND);

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

__declspec(dllexport) void __cdecl StartUi(HWND hwnd)
{
	FormsMain(hwnd);
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
