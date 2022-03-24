﻿#include"SystemClass.h"
#include "../Core/ComInput.h"//TODO:相对引用，要改
#include "../Core/Event/TesterEvent.h"
#include "TexClass.h"
//全局变量
//static SystemClass* D3DAPP = NULL;
// ？？放在头文件？
//extern SystemClass* D3DAPP = NULL;

//接受SystemClass类对象的全局回调函数
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);  //发送WM_QUIT消息到消息队列 接收到WM_QUIT跳出消息循环
		return 0;
	}

	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}

	//将其它消息传送到D3DAPP也就是SystemClass对象的MessageHandler函数
	default:
		//return D3DAPP->MessageHandler(hwnd, message, wParam, lParam);
		return GDirectxCore->MessageHandler(hwnd, message, wParam, lParam);

	}
}

/*
/// <summary>
/// 可一直使用的EventHandler不同，这个c++自定义BaseEvent的使用：应该在onInput
/// </summary>
/// <param name="callBack"></param>
void SystemClass::SetEventCallback(const EventCallback& callBack)
{
	eventCallback = callBack;
}*/

InputClass* SystemClass::Input(){return m_Input;}
/// <summary>
/// TODO:临时方法
/// </summary>
/// <returns></returns>
ModelClass* SystemClass::Model(){return m_Graphics->Model();}
GraphicsClass* SystemClass::Graphics() { return m_Graphics; }
SystemClass::SystemClass()
{
	m_Graphics = NULL;
	m_Input = NULL;
	m_LayerManager = NULL;

}

SystemClass::SystemClass(const SystemClass& sys)
{

}

/*不在析构函数中回收内存的一点原因是谨慎对待内存的回收问题*/
SystemClass::~SystemClass()
{

}

void SystemClass::OnEvent(Event& event)
{
	//没完全用过的代码，handler代码可参考代码：
	//处理代码，例子1
	//EventDispatcher dispatcher(event);
	//dispatcher.Dispath<WindowCloseEvent>(BIND_EVENT(Game::OnClose, this));

	//处理代码，例子2
/*	if (!event.bHandled)
	{
	}*/
	EventDispatcher dispatcher(event);
	dispatcher.Dispath<TesterEvent>(BIND_EVENT(SystemClass::OnChangeShader, this));
}
bool SystemClass::OnChangeShader(Event& event)
{
	auto shader = m_Graphics->Shader();
	TesterEvent* eve = dynamic_cast<TesterEvent*>(&event);
	bool initResult = true;
	switch (eve->key)
	{
	case 49://num 1
		if (shader)
		{
			shader->ShaderIndex = 1;
			initResult = shader->Initialize(m_Graphics->D3D()->GetDevice(), mHwnd);
		}
		m_Graphics->mShader2->Init("MyShader.fx");
		m_Graphics->mShader2->Apply();
		break;
	case 51://num 3
		m_Graphics->mShader2->Init("Shader/s3.fx");
		m_Graphics->mShader2->Apply();
		break;
	case 50://num 2
		if (shader)
		{
			shader->ShaderIndex = 2;
			initResult = shader->Initialize(m_Graphics->D3D()->GetDevice(), mHwnd);
		}
		m_Graphics->mShader2->Init("Shader/s2.fx");
		TexClass* tex = new TexClass();
		tex->Initilize(g_Device, L"TexturesAndMat/Gun_Texture.png");
		ID3D11ShaderResourceView* texture = tex->GetTexture();
	
		m_Graphics->mShader2->Apply();

		D3D11_SAMPLER_DESC sampDesc;
		ZeroMemory(&sampDesc, sizeof(sampDesc));
		sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		sampDesc.MinLOD = 0;
		sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
		ID3D11SamplerState* textureState;
		auto hr = g_Device->CreateSamplerState(&sampDesc, &textureState);
		bool isset = m_Graphics->mShader2->SetTexture("ShaderTexture", texture);
		printf("设置shader texture %d \n", isset);
		break;

	//default:
	//	break;
	}
	if (initResult == false) {
		throw exception("shader compile error");
	}
	return true;
}

/*系统类初始化函数*/
bool SystemClass::Initialize()
{

	//SetEventCallback(BIND_EVENT(SystemClass::OnEvent, this));
	int ScreenWidth, ScreenHeight;
	bool result;

	/*初始化屏幕宽度和高度*/
	ScreenWidth = 0;
	ScreenHeight = 0;

	/*初始化系统类的窗口*/
	InitializeWindow(ScreenWidth, ScreenHeight);

	/*创建输入类对象*/
	m_Input = new InputClass();
	if (!m_Input)
	{
		return false;
	}

	m_LayerManager = new LayerManager();
	auto component = new ComInput();
	component->SetEventCallback(BIND_EVENT(SystemClass::OnEvent, this));
	m_LayerManager->PushLayer(shared_ptr<ComInput>( component));


	/*初始化输入对象*/
	m_Input->Initialize();

	/*创建图形类对象*/
	m_Graphics = new GraphicsClass();
	if (!m_Graphics)
	{
		return false;
	}
		
	/*初始化图形对象,发生在窗口初始化之后,因为窗口初始化之后,窗口句柄才被建立*/
	result=m_Graphics->Initialize(ScreenWidth, ScreenHeight, mHwnd);
	if (!result)
	{
		return false;
	}
	
	return true;
}

void SystemClass::Shutdown()
{
	/*释放图形类对象*/
	if (m_Graphics)
	{
		m_Graphics->Shutdown();
		delete m_Graphics;
		m_Graphics = NULL;
	}

	/*释放输入类对象*/
	if (m_Input)
	{
		delete m_Input;
		m_Input = NULL;
	}

	/*关闭窗口*/
	this->ShutdownWindow();
}

void SystemClass::Run()
{
	MSG msg = { 0 };
	bool done, result;

	/*循环直到收到来自窗口的或者使用者的quit消息*/
	done = false;
	while (!done)
	{
		//操作窗口消息
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				done = true;
			}	
			TranslateMessage(&msg);
			DispatchMessage(&msg);       //把消息发送到WindProc()中
		}
		else
		{
			for (auto it = m_LayerManager->Begin(); it != m_LayerManager->End(); it++)
			{
				(*it)->OnImguiRender();
			}
			result = Frame();  //Frame运行的函数可能造成游戏退出
			if (!result)
			{
				done = true;
			}
		}

	}
}

bool SystemClass::Frame()
{
	bool result;

	
	//你按下了退出函数,则退出这个应用(按键的宏刚好相对应)
	if (m_Input->IsKeyDown(VK_ESCAPE))
	{
		return false;
	}
	
	//进行图形对象的帧函数
	result = m_Graphics->Frame();
	if (!result)
	{
		return false;
	}
	return true;
}


LRESULT CALLBACK SystemClass::MessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		//消息处理可参考sdEngine最新
		// GameWindow.cpp
		//WM_CHAR 和 WM_keydown的问题观察
		//https://bbs.csdn.net/topics/60387619
	   //确认一个键已经在键盘上按下
	   case WM_KEYDOWN:
	  {
		   if (wParam == VK_ESCAPE)
		   {
		   }
		  //如果一个键被按下,则将这个键的信息送到input object,为了可以记录这个状态
		  m_Input->KeyDown((unsigned int)wParam);//所有字符都是 ，229,无解。。。。。。。。。。。
		  return 0;
	  }

	   //确认一个键被释放
	   case WM_KEYUP:
	   {
		   //如果一个键被释放,则将这个键的信息送到input object,为了可以取消这个状态
		   m_Input->KeyUp((unsigned int)wParam);
		   //不知道 WM_KEYUP，WM_CHAR的关系
		   //有点傻傻搞不清楚
		   //暂时，强制把所有按键都还原
		   //（WM_KEYUP 时，wParam又不固定，也差wm_char不相同。。。。。。。）
		  // if (wParam == 229) {
			   for (int i = 0; i < 200; i++)
			   {
				   m_Input->KeyUp(i);
			   }
		   //}
		   return 0;
	   }
	   case WM_CHAR:
	   {
		   m_Input->KeyDown((unsigned int)wParam);
		   return 0;
	   }
	   //其它的消息被送到默认处理消息函数
	   default:
	   {
		   return DefWindowProc(hwnd, message, wParam, lParam); //为全局静态函数
	   }
	}
}



void SystemClass::InitializeWindow(int& ScrrenWidth, int &ScrrenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScrrenSettings;
	int posX, posY;

	//获取一个额外的指向这个对象的指针
	//D3DAPP = this;   

	//获取应用实例句柄
	mHinstance = GetModuleHandle(NULL);

	//给予应用一个名字
	mApplicationName = L"Engine";

	//设定要创建的类的属性	
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = mHinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = mApplicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	//注册这个类
	RegisterClassEx(&wc);

	//获取屏幕分辨率的宽度和高度
	ScrrenWidth = GetSystemMetrics(SM_CXSCREEN);
	ScrrenHeight= GetSystemMetrics(SM_CYSCREEN);

	//取决于是否为全屏幕
	if (FULL_SCREEN)
	{
		//如果为全屏幕,则设定屏幕为用户桌面的最大化并且为32bit
		memset(&dmScrrenSettings, 0, sizeof(dmScrrenSettings));
		dmScrrenSettings.dmSize = sizeof(dmScrrenSettings);
		dmScrrenSettings.dmPelsWidth = (unsigned long)ScrrenWidth;
		dmScrrenSettings.dmPelsHeight = (unsigned long)ScrrenHeight;
		dmScrrenSettings.dmBitsPerPel = 32;
		dmScrrenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		//改变显示设定,设置为全屏幕
		ChangeDisplaySettings(&dmScrrenSettings, CDS_FULLSCREEN);

		//设定窗口左上角的位置
		posX = posY = 0;
	}
	else
	{
		//如果窗口化,设定为800*600分辨率
		ScrrenWidth = 800;
		ScrrenHeight = 600;

		//窗口位于屏幕稍微的左上方
		posX = (GetSystemMetrics(SM_CXSCREEN) - ScrrenWidth) / 2-200;
		posY = (GetSystemMetrics(SM_CYSCREEN) - ScrrenHeight) / 2-100;

	}

	//创建窗口,并且获取窗口的句柄
	mHwnd = CreateWindowEx(WS_EX_APPWINDOW, mApplicationName, mApplicationName,
		WS_OVERLAPPEDWINDOW,
		posX, posY, ScrrenWidth, ScrrenHeight, NULL, NULL, mHinstance, NULL);

	//将窗口显示于屏幕之上,并设定该窗口为主要集中点
	ShowWindow(mHwnd, SW_SHOW);
	SetForegroundWindow(mHwnd);
	SetFocus(mHwnd);

	//隐藏鼠标光标
	//ShowCursor(false);
}

void SystemClass::ShutdownWindow()
{
	//显示鼠标光标
	//ShowCursor(true);

	//如果离开全屏幕模式,恢复显示设定
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	//移除(破坏)窗口
	DestroyWindow(mHwnd);
	mHwnd = NULL;

	//移除程序实例
	UnregisterClass(mApplicationName, mHinstance);
	mHinstance = NULL;

	//置空应用类对象
	//D3DAPP = NULL;

}

shared_ptr<SystemClass> SystemClass::Get()
{
	if (nullptr == m_pDirectxCore)
	{
		m_pDirectxCore = shared_ptr<SystemClass>(new SystemClass());
	}

	return m_pDirectxCore;
	//return shared_ptr<SystemClass>();
	//return D3DAPP;
}
shared_ptr<SystemClass> SystemClass::m_pDirectxCore = nullptr;

