#include "ComInput.h"
#include "../Render/SystemClass.h"
#include "Event/KeyCode.h"
#include "Event/TesterEvent.h"


//#include "Component.h"
//全局变量，参考：https://www.cnblogs.com/stones-dream/p/9553843.html
//extern SystemClass* D3DAPP;
ComInput::ComInput():Component("Input")
{
	
}
//重启的另一种方法参考：
// https://blog.csdn.net/JacksonLiang/article/details/1349223
// 
// #include <QString>
// #include <tlhelp32.h>
//void check_and_restart() {
//	PROCESSENTRY32 pe32;
//	pe32.dwSize = sizeof(pe32);
//
//	HANDLE snap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
//	if (snap == INVALID_HANDLE_VALUE) {
//		std::cout << "CreateToolhelp32Snapshot failed." << std::endl;
//		return;
//	}
//
//	BOOL more = ::Process32First(snap, &pe32);
//	bool exists = false;
//	QString name = fullname.mid(fullname.lastIndexOf('\\'));
//	while (more) {
//		if (name.compare(QString::fromWCharArray(pe32.szExeFile)) == 0) {
//			exists = true;
//			HWND hwnd = ::FindWindow(NULL, name.toStdWString().c_str());
//			if (hwnd != NULL) {
//				HINSTANCE hdll = ::LoadLibrary(L"user32.dll");
//				if (NULL != hdll) {
//					typedef BOOL(WINAPI* PROCISHUNGAPPWINDOW)(HWND);
//					PROCISHUNGAPPWINDOW IsHungAppWindow = (PROCISHUNGAPPWINDOW)GetProcAddress(hdll, "IsHungAppWindow");
//					if (IsHungAppWindow(hwnd)) {
//						//kill and restart
//						HANDLE proc = OpenProcess(PROCESS_ALL_ACCESS, TRUE, pe32.th32ProcessID);
//						TerminateProcess(proc, 0);
//						::CloseHandle(proc);
//						goto restart;
//					}
//				}
//			}
//			::CloseHandle(snap);
//			return;
//		}
//		more = ::Process32Next(snap, &pe32);
//	}
//	::CloseHandle(snap);
//
//	//restart
//	if (!exists) {
//		goto restart;
//	}
//restart:
//	STARTUPINFO si;
//	PROCESS_INFORMATION pi;
//	si.dwFlags = STARTF_USESHOWWINDOW;
//	si.wShowWindow = TRUE;
//	if (CreateProcess(fullname.toStdWString().c_str(), NULL, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
//		CloseHandle(pi.hThread);
//		CloseHandle(pi.hProcess);
//	}
//}
void resetKeyboards()
{
	auto input = GDirectxCore->Input();
	for (int i = 0; i < 200; i++)
	{
		input->KeyUp(i);
	}
}
void ComInput::OnImguiRender()
{
	auto input = GDirectxCore->Input();
	float moveStep = 0.001;
	if (GDirectxCore->Input()->IsKeyDown(119))
	{
		auto pos = GDirectxCore->Model()->GetPosition();
		pos.y += moveStep;
		GDirectxCore->Model()->SetPosition(pos);
	}
	else if (GDirectxCore->Input()->IsKeyDown(115))
	{
		auto pos = GDirectxCore->Model()->GetPosition();
		pos.y -= moveStep;
		GDirectxCore->Model()->SetPosition(pos);
	}
	else if (GDirectxCore->Input()->IsKeyDown(97)) {
		auto pos = GDirectxCore->Model()->GetPosition();
		pos.x -= moveStep;
		GDirectxCore->Model()->SetPosition(pos);
	}
	else if (GDirectxCore->Input()->IsKeyDown(100))
	{
		auto pos = GDirectxCore->Model()->GetPosition();
		pos.x += moveStep;
		GDirectxCore->Model()->SetPosition(pos);
	}
	else if (GDirectxCore->Input()->IsKeyDown(49))//键盘 1==49; 键盘 2==50
	{
		try
		{
			TesterEvent event;
			event.key = 49;
			/* 这个方法只是转发用
			EventDispatcher dispatcher(event);
			dispatcher.Dispath<TesterEvent>(BIND_EVENT(SystemClass::OnChangeShader, this));*/

			eventCallback(event);
		}
		catch (exception ex) {
			resetKeyboards();//因为是按下事件出错，可能弹出提示框，所以清空（保护）
		}
	}
	else if (GDirectxCore-> Input()->IsKeyDown(50))
	{
		try
		{
			TesterEvent event;
			event.key = 50;
			eventCallback(event);
		}
		catch (exception ex) {
			resetKeyboards();//因为是按下事件出错，可能弹出提示框，所以清空（保护）
		}
	}
	else if (input->IsKeyDown(116))//F5 ==116; F6 == 117, F7 == 118
	{
		//重启
	}
}

/// <summary>
/// 确实应该在onInput（发送者）
/// </summary>
/// <param name="callBack"></param>
void ComInput::SetEventCallback(const EventCallback& callBack)
{
	eventCallback = callBack;
}