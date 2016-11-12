#include "EditorForm.h"
#include "..\..\Ballz\EditorComm.h"
#include <vector>
#include <string>

using namespace System;
using namespace System::Windows::Forms;


[STAThread]
void FormsMain()
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	CppWinForm1::EditorForm form;

	Application::Run(%form);

	/*
	System::IntPtr myWindowHandle = System::IntPtr(hwnd);
	System::Windows::Forms::IWin32Window ^w = System::Windows::Forms::Control::FromHandle(myWindowHandle);
	form.Show(w);	
	*/
}

void FormsUpdate(UiMessage* msg)
{
	//if (msg->id == UiMessageId::SetMove)
	//	EditorUi::EditorForm::instance->setStr((std::string*)msg->data);
}

#ifdef NO_LIB

int Main()
{
	FormsMain();

	return 0;
}

void SendMsg(UiMessageId, void*)
{
}

#else

typedef void(*MSGFUNC)(UiMessage*);
extern "C"
{
	__declspec(dllexport) void __cdecl StartUi(void);

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

__declspec(dllexport) void __cdecl StartUi(void)
{
	FormsMain();
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
#pragma managed(push, off)


