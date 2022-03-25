﻿#include"SystemClass.h"
#include "../Core/ComInput.h"//TODO:相对引用，要改
#include "../Core/Event/TesterEvent.h"
#include "TexClass.h"
#include "../Core/Helper/MathHelper.h"
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
SystemClass::SystemClass():mMainWndCaption(L"AR Engine")	// L表明是Unicode 两个字节的字符
//, mTheta(1.5f * MathHelper::Pi), mPhi(0.25f * MathHelper::Pi), mRadius(50.0f)
,mTheta(0),mPhi(0),mRadius(50),mThetaY(0)
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
	{
		if (shader)
		{
			shader->ShaderIndex = 1;
			initResult = shader->Initialize(m_Graphics->D3D()->GetDevice(), mHwnd);
		}
		m_Graphics->mShader2->Init("MyShader.fx");
		m_Graphics->mShader2->Apply();
		break;
	}
	case 51://num 3
	{
		m_Graphics->mShader2->Init("Shader/s3.fx");
		m_Graphics->mShader2->Apply();
		break;
	}
	case 52://num 4
	{
		m_Graphics->mShader2->Init("Shader/s4Bumped.fx");
		m_Graphics->mShader2->Apply();
		ID3D11ShaderResourceView* texBumped = m_Graphics->LoadTexture(L"TexturesAndMat/Tormentor_N.png");
		m_Graphics->mShader2->SetTexture("_BumpMap", texBumped);
		ID3D11ShaderResourceView* texColor = m_Graphics->LoadTexture(L"TexturesAndMat/Tormentor_Texturing.png");
		g_Graphics->mShader2->SetTexture("ShaderTexture", texColor);
		break;
	}
	case 50://num 2
	{
		//旧的Shader??
		if (shader)
		{
			shader->ShaderIndex = 2;
			initResult = shader->Initialize(m_Graphics->D3D()->GetDevice(), mHwnd);
		}

		//新的
		m_Graphics->mShader2->Init("Shader/s6NormalMap.fx");
		ID3D11ShaderResourceView* texBumped = m_Graphics->LoadTexture(L"TexturesAndMat/Tormentor_N.png");
		m_Graphics->mShader2->SetTexture("_BumpMap", texBumped);
		ID3D11ShaderResourceView* texColor = m_Graphics->LoadTexture(L"TexturesAndMat/Tormentor_Texturing.png");
		g_Graphics->mShader2->SetTexture("ShaderTexture", texColor);

		break;
	}
	default:
	{
		break;
	}
	}
	if (initResult == false) {
		throw exception("shader compile error");
	}
	return true;
}
/// <summary>
/// TODO:继承一个统一窗口，就可以在不同效果间切换了（虽然现在靠shader切换，键盘,1,2,3,4)
/// </summary>
void SystemClass::CalculateFrameStats()
{
	// 将fps信息显示到窗口bar上
	static int frameCnt = 0;
	static float timeElapsed = 0.0f;

	frameCnt++;

	if ((gameTimer.TotalTime() - timeElapsed) >= 1.0f)
	{
		float fps = (float)frameCnt;
		float mspf = 1000.0f / fps;

		std::wostringstream outs;
		outs.precision(6);
		outs << mMainWndCaption << L"   "
			<< L"FPS:" << fps << L"   "
			<< L"Frame Time:" << mspf << L"(ms)";
		SetWindowText(mHwnd, outs.str().c_str());
		frameCnt = 0;
		timeElapsed += 1.0f;
	}
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
	gameTimer.Reset();	// 时间类重置
	/*循环直到收到来自窗口的或者使用者的quit消息*/
	done = false;
	while (!done)
	{
		//操作窗口消息,处理	
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
			// 否则运行游戏逻辑
			gameTimer.Tick();
			for (auto it = m_LayerManager->Begin(); it != m_LayerManager->End(); it++)
			{
				(*it)->OnImguiRender();
			}
			result = Frame();  //Frame运行的函数可能造成游戏退出
			//////////////////////////////////////////////
			//(写的相当烂，整个结构可以说没有结构，和DirectX官方例子完整性差太多）
			//大神，刚毕业的时候写的吧
			//一个明显不会c++，不会windows 应用的人写的。。。
			//哎，不要在意这些细节，咱们继续肝引擎
			/////////////////////////////////////////////
			CalculateFrameStats();//统计fps

			////鼠标控制镜头(探照灯？？）
			//float x = mRadius * sinf(mPhi) * cosf(mTheta);
			//float z = mRadius * sinf(mPhi) * sinf(mTheta);
			//float y = mRadius * cosf(mPhi);
			////m_Graphics->mCamera->SetRotation(x, y, z);
			////mEyePosW = XMFLOAT4(x, y, z, 0.0f);

			//旋转人
			float x = 0;
			float dy = mLastMousePos.x - mDownMousePos.x;
			float z = 0;
			//XMFLOAT3 pos = GDirectxCore->models[0].m_pTransform->localRotation;
			//rot x;//上，下
			//rot y;//左，右
			XMFLOAT3 currPos =  XMFLOAT3(0,mOriginPos.y + dy,0);
			//GDirectxCore->models[0].m_pTransform->localRotation = currPos;
			models[0].m_pTransform->localRotation = XMFLOAT3(mThetaY*100, -mTheta*100, 0);

			if (!result)
			{
				done = true;
			}
			
			//TODO:// 暂停功能的实现
		}



	}
}

bool SystemClass::Frame()
{
	//还是很烂的代码（大牛也有新手的时候啊）
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
		// 处理鼠标事件
		case WM_LBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_RBUTTONDOWN:
			OnMouseDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			return 0;
		case WM_LBUTTONUP:
		case WM_MBUTTONUP:
		case WM_RBUTTONUP:
			OnMouseUp(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			return 0;
		case WM_MOUSEMOVE:
			OnMouseMove(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			return 0;

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
	//mHwnd = CreateWindowEx(WS_EX_APPWINDOW, mApplicationName, mApplicationName,
	mHwnd = CreateWindowEx(WS_EX_APPWINDOW,mApplicationName, mMainWndCaption.c_str(),
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

void SystemClass::OnMouseDown(WPARAM btnState, int x, int y)
{
	mOriginPos = models[0].m_pTransform->localPosition;
	//mDownMousePos.x = x;
	//mDownMousePos.y = y;
	mLastMousePos.x = x;
	mLastMousePos.y = y;
	SetCapture(mHwnd);	// 捕获鼠标
}

void SystemClass::OnMouseUp(WPARAM btnState, int x, int y)
{
	ReleaseCapture();
}

void SystemClass::OnMouseMove(WPARAM btnState, int x, int y)
{
	if ((btnState & MK_LBUTTON) != 0)
	{
		// 计算鼠标移动的距离
		float dx = XMConvertToRadians(0.25f * static_cast<float>(x - mLastMousePos.x));
		float dy = XMConvertToRadians(0.25f * static_cast<float>(y - mLastMousePos.y));
		

		// 更新视角信息
		mTheta += dx;
		mThetaY += dy;
		mPhi += dy;

		// 裁剪mPhi角度
		mPhi = MathHelper::Clamp(mPhi, 0.1f, MathHelper::Pi - 0.1f);
	}
	else if ((btnState & MK_RBUTTON) != 0)
	{
		// 设定每个像素点为0.005的距离
		float dx = 0.005f * static_cast<float>(x - mLastMousePos.x);
		float dy = 0.005f * static_cast<float>(y - mLastMousePos.y);

		// 更新相机与物体的距离
		mRadius += dx - dy;

		// 裁剪距离
		mRadius = MathHelper::Clamp(mRadius, 3.0f, 50.0f);
	}

	mLastMousePos.x = x;
	mLastMousePos.y = y;
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

