﻿#pragma once
#ifndef _SYSTEM_CLASS_H
#define _SYSTEM_CLASS_H
#define WIN32_LEAN_AND_MEAN  //这个宏可以减少Win32头文件的大小

#include"InputClass.h"
#include<Windows.h>  
#include"GraphicsClass.h"
#include "../Core/LayerManager.h"
#include <functional>
#include "../Core/Event/EventBase.h"
#include "../Core/GameObject.h"
//using namespace std;

class SystemClass
{
	
private:
	LPCWSTR mApplicationName;  //应用名字
	HINSTANCE mHinstance; //应用实例句柄
	HWND mHwnd; //应用窗口句柄
	static shared_ptr<SystemClass> m_pDirectxCore;
private:
	InputClass* m_Input; //输入类
	GraphicsClass* m_Graphics; //图形类
	LayerManager* m_LayerManager;
private:
	//帧函数
	bool Frame();

	//初始化窗口函数
	void InitializeWindow(int&, int&);

	//关闭窗口函数
	void ShutdownWindow();

public:
	static shared_ptr<SystemClass> Get();
	//using EventCallback = std::function<void(Event&)>;
	//EventCallback eventCallback;
	//void SetEventCallback(const EventCallback& callBack);
	void OnEvent(Event& event);
	bool OnChangeShader(Event& event);
	InputClass* Input();
	ModelClass* Model();
	vector<GameObject> models;
	GraphicsClass* Graphics();
	/*构造函数*/
	SystemClass();

	/*拷贝构造函数*/
	SystemClass(const SystemClass&);

	/*析构函数*/
	~SystemClass();

	/*初始化函数*/
	bool Initialize();

	/*关闭应用函数*/
	void Shutdown();

	/*运行函数*/
	void Run();

	/*消息操作函数,这里这个函数接受的消息很多传递给全局静态函数WndProc处理,这个回调函数有些特殊,请注意*/
	LRESULT CALLBACK MessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
};

/*静态回调函数*/
//static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

/*静态全局变量,这个主要用来进行回调函数的消息传递*/

#define GDirectxCore (SystemClass::Get())
#define g_Graphics (SystemClass::Get()->Graphics())
#define g_Device (SystemClass::Get()->Graphics()->D3D()->GetDevice())
#define g_DeviceContext (SystemClass::Get()->Graphics()->D3D()->GetDeviceContext())
#endif 
