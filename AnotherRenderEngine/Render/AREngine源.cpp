#include"SystemClass.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	SystemClass* System=NULL;
	bool result;

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

