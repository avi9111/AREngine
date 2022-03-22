//整个RenderEngine暂时（2022年）基于一个Direct3X的官方书的代码练手入门例子，出处可参考：
//

#include"SystemClass.h"
#include "../Core/ImportFbx2.h"

//#include <SDEngine/Common/DirectxCore.h>//估计此哥们用了directX12（居然可以写include <>）
//#include "Engine.h"//太多引用，不能用
//#include "../Include/app.h";
//#include <stdio.h>;
//#include "../Include/log/output_stream.h";
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	string strFbxFileName = "TObjects/zuoqi.fbx";
	//auto m_pFBXModel = shared_ptr<FBXModelData>(new FBXModelData());
	GImportFBX->ImportFbxFileTest(strFbxFileName);


	//GImportFBX->m_mapFBXModel[strFbxFileName] = m_pFBXModel;
	

	//GDirectxCore->End();//???directX12??
	// 
	//nya_system::app* app = new nya_system::app();
	//app->start_windowed(100, 100, 800, 600, 0);
	///想引入一个引擎，.lib .dll都不成功，算了

	SystemClass* System=NULL;
	bool result;

	//WindowRegisterClass(hInstance);

	//创建SystemClass类
	//System = new SystemClass;

	//if (!System)
	//{
	//	return 0;
	//}


	//初始化和运行系统对象
	//result = System->Initialize();
	result = GDirectxCore->Initialize();
	if (result)
	{
		//System->Run();
		GDirectxCore->Run();
	}

	//关闭systemclass对象
	//System->Shutdown();
	GDirectxCore->Shutdown();

	//delete System;
	//System = NULL;
	return 0;
}
//窗口注册函数，已在SystemClass实现
////
////  函数: MyRegisterClass()
////
////  目标: 注册窗口类。
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




