//����RenderEngine��ʱ��2022�꣩����һ��Direct3X�Ĺٷ���Ĵ��������������ӣ������ɲο���
//
#include"SystemClass.h"
//#include "../Include/app.h";
//#include <stdio.h>;
//#include "../Include/log/output_stream.h";
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	//nya_system::app* app = new nya_system::app();
	//app->start_windowed(100, 100, 800, 600, 0);
	///������һ�����棬.lib .dll�����ɹ�������

	SystemClass* System=NULL;
	bool result;

	//WindowRegisterClass(hInstance);

	//����SystemClass��
	System = new SystemClass;

	if (!System)
	{
		return 0;
	}

	//��ʼ��������ϵͳ����
	result = System->Initialize();
	if (result)
	{
		System->Run();
	}

	//�ر�systemclass����
	System->Shutdown();
	delete System;
	System = NULL;
	return 0;
}
//����ע�ắ��������SystemClassʵ��
////
////  ����: MyRegisterClass()
////
////  Ŀ��: ע�ᴰ���ࡣ
////
//ATOM WindowRegisterClass(HINSTANCE hInstance)
//{
//	WNDCLASSEXW wcex;
//
//	wcex.cbSize = sizeof(WNDCLASSEX);
//
//	wcex.style = CS_HREDRAW | CS_VREDRAW;
//	//  wcex.lpfnWndProc    = WndProc;
//	wcex.cbClsExtra = 0;
//	wcex.cbWndExtra = 0;
//	wcex.hInstance = hInstance;
//	//wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT1));//WinUsr.h
//	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
//	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
////	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WINDOWSPROJECT1);
////	wcex.lpszClassName = szWindowClass;
////	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
//
//	return RegisterClassExW(&wcex);
//}




