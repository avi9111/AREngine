#pragma once
#ifndef _SYSTEM_CLASS_H
#define _SYSTEM_CLASS_H
#define WIN32_LEAN_AND_MEAN  //�������Լ���Win32ͷ�ļ��Ĵ�С

#include"InputClass.h"
#include<Windows.h>  
#include"GraphicsClass.h"

class SystemClass
{

private:
	LPCWSTR mApplicationName;  //Ӧ������
	HINSTANCE mHinstance; //Ӧ��ʵ�����
	HWND mHwnd; //Ӧ�ô��ھ��

private:
	InputClass* m_Input; //������
	GraphicsClass* m_Graphics; //ͼ����

private:
	//֡����
	bool Frame();

	//��ʼ�����ں���
	void InitializeWindow(int&, int&);

	//�رմ��ں���
	void ShutdownWindow();

public:

	/*���캯��*/
	SystemClass();

	/*�������캯��*/
	SystemClass(const SystemClass&);

	/*��������*/
	~SystemClass();

	/*��ʼ������*/
	bool Initialize();

	/*�ر�Ӧ�ú���*/
	void Shutdown();

	/*���к���*/
	void Run();

	/*��Ϣ��������,��������������ܵ���Ϣ�ܶഫ�ݸ�ȫ�־�̬����WndProc����,����ص�������Щ����,��ע��*/
	LRESULT CALLBACK MessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
};

/*��̬�ص�����*/
//static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

/*��̬ȫ�ֱ���,�����Ҫ�������лص���������Ϣ����*/


#endif 
