#include"SystemClass.h"
#include "../Core/ComInput.h"//TODO:������ã�Ҫ��
#include "../Core/Event/TesterEvent.h"
//ȫ�ֱ���
//static SystemClass* D3DAPP = NULL;
// ��������ͷ�ļ���
extern SystemClass* D3DAPP = NULL;

//����SystemClass������ȫ�ֻص�����
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);  //����WM_QUIT��Ϣ����Ϣ���� ���յ�WM_QUIT������Ϣѭ��
		return 0;
	}

	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}

	//��������Ϣ���͵�D3DAPPҲ����SystemClass�����MessageHandler����
	default:
		return D3DAPP->MessageHandler(hwnd, message, wParam, lParam);

	}
}

/*
/// <summary>
/// ��һֱʹ�õ�EventHandler��ͬ�����c++�Զ���BaseEvent��ʹ�ã�Ӧ����onInput
/// </summary>
/// <param name="callBack"></param>
void SystemClass::SetEventCallback(const EventCallback& callBack)
{
	eventCallback = callBack;
}*/

InputClass* SystemClass::Input()
{
	return m_Input;
}
/// <summary>
/// TODO:��ʱ����
/// </summary>
/// <returns></returns>
ModelClass* SystemClass::Model()
{
	return m_Graphics->Model();
}

SystemClass::SystemClass()
{
	m_Graphics = NULL;
	m_Input = NULL;
	m_LayerManager = NULL;

}

SystemClass::SystemClass(const SystemClass& sys)
{

}

/*�������������л����ڴ��һ��ԭ���ǽ����Դ��ڴ�Ļ�������*/
SystemClass::~SystemClass()
{

}

void SystemClass::OnEvent(Event& event)
{
	//û��ȫ�ù��Ĵ��룬handler����ɲο����룺
	//������룬����1
	//EventDispatcher dispatcher(event);
	//dispatcher.Dispath<WindowCloseEvent>(BIND_EVENT(Game::OnClose, this));

	//������룬����2
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
	case 49:
		
		shader->ShaderIndex = 1;
		initResult = shader->Initialize(m_Graphics->D3D()->GetDevice(), mHwnd);
		break;
	case 50:
		shader->ShaderIndex = 2;
		initResult = shader->Initialize(m_Graphics->D3D()->GetDevice(), mHwnd);
		break;
	case 51 :
		break;
	default:
		break;
	}
	if (initResult == false) {
		throw exception("shader compile error");
	}
	return true;
}

/*ϵͳ���ʼ������*/
bool SystemClass::Initialize()
{

	//SetEventCallback(BIND_EVENT(SystemClass::OnEvent, this));
	int ScreenWidth, ScreenHeight;
	bool result;

	/*��ʼ����Ļ��Ⱥ͸߶�*/
	ScreenWidth = 0;
	ScreenHeight = 0;

	/*��ʼ��ϵͳ��Ĵ���*/
	InitializeWindow(ScreenWidth, ScreenHeight);

	/*�������������*/
	m_Input = new InputClass();
	if (!m_Input)
	{
		return false;
	}

	m_LayerManager = new LayerManager();
	auto component = new ComInput();
	component->SetEventCallback(BIND_EVENT(SystemClass::OnEvent, this));
	m_LayerManager->PushLayer(shared_ptr<ComInput>( component));


	/*��ʼ���������*/
	m_Input->Initialize();

	/*����ͼ�������*/
	m_Graphics = new GraphicsClass();
	if (!m_Graphics)
	{
		return false;
	}
		
	/*��ʼ��ͼ�ζ���,�����ڴ��ڳ�ʼ��֮��,��Ϊ���ڳ�ʼ��֮��,���ھ���ű�����*/
	result=m_Graphics->Initialize(ScreenWidth, ScreenHeight, mHwnd);
	if (!result)
	{
		return false;
	}
	
	return true;
}

void SystemClass::Shutdown()
{
	/*�ͷ�ͼ�������*/
	if (m_Graphics)
	{
		m_Graphics->Shutdown();
		delete m_Graphics;
		m_Graphics = NULL;
	}

	/*�ͷ����������*/
	if (m_Input)
	{
		delete m_Input;
		m_Input = NULL;
	}

	/*�رմ���*/
	this->ShutdownWindow();
}

void SystemClass::Run()
{
	MSG msg = { 0 };
	bool done, result;

	/*ѭ��ֱ���յ����Դ��ڵĻ���ʹ���ߵ�quit��Ϣ*/
	done = false;
	while (!done)
	{
		//����������Ϣ
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				done = true;
			}	
			TranslateMessage(&msg);
			DispatchMessage(&msg);       //����Ϣ���͵�WindProc()��
		}
		else
		{
			for (auto it = m_LayerManager->Begin(); it != m_LayerManager->End(); it++)
			{
				(*it)->OnImguiRender();
			}
			result = Frame();  //Frame���еĺ������������Ϸ�˳�
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

	
	//�㰴�����˳�����,���˳����Ӧ��(�����ĺ�պ����Ӧ)
	if (m_Input->IsKeyDown(VK_ESCAPE))
	{
		return false;
	}
	
	//����ͼ�ζ����֡����
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
		//��Ϣ����ɲο�sdEngine����
		// GameWindow.cpp
		//WM_CHAR �� WM_keydown������۲�
		//https://bbs.csdn.net/topics/60387619
	   //ȷ��һ�����Ѿ��ڼ����ϰ���
	   case WM_KEYDOWN:
	  {
		   if (wParam == VK_ESCAPE)
		   {
		   }
		  //���һ����������,�����������Ϣ�͵�input object,Ϊ�˿��Լ�¼���״̬
		  m_Input->KeyDown((unsigned int)wParam);//�����ַ����� ��229,�޽⡣��������������������
		  return 0;
	  }

	   //ȷ��һ�������ͷ�
	   case WM_KEYUP:
	   {
		   //���һ�������ͷ�,�����������Ϣ�͵�input object,Ϊ�˿���ȡ�����״̬
		   m_Input->KeyUp((unsigned int)wParam);
		   //��֪�� WM_KEYUP��WM_CHAR�Ĺ�ϵ
		   //�е�ɵɵ�㲻���
		   //��ʱ��ǿ�ư����а�������ԭ
		   //��WM_KEYUP ʱ��wParam�ֲ��̶���Ҳ��wm_char����ͬ����������������
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
	   //��������Ϣ���͵�Ĭ�ϴ�����Ϣ����
	   default:
	   {
		   return DefWindowProc(hwnd, message, wParam, lParam); //Ϊȫ�־�̬����
	   }
	}
}



void SystemClass::InitializeWindow(int& ScrrenWidth, int &ScrrenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScrrenSettings;
	int posX, posY;

	//��ȡһ�������ָ����������ָ��
	D3DAPP = this;   

	//��ȡӦ��ʵ�����
	mHinstance = GetModuleHandle(NULL);

	//����Ӧ��һ������
	mApplicationName = L"Engine";

	//�趨Ҫ�������������	
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

	//ע�������
	RegisterClassEx(&wc);

	//��ȡ��Ļ�ֱ��ʵĿ�Ⱥ͸߶�
	ScrrenWidth = GetSystemMetrics(SM_CXSCREEN);
	ScrrenHeight= GetSystemMetrics(SM_CYSCREEN);

	//ȡ�����Ƿ�Ϊȫ��Ļ
	if (FULL_SCREEN)
	{
		//���Ϊȫ��Ļ,���趨��ĻΪ�û��������󻯲���Ϊ32bit
		memset(&dmScrrenSettings, 0, sizeof(dmScrrenSettings));
		dmScrrenSettings.dmSize = sizeof(dmScrrenSettings);
		dmScrrenSettings.dmPelsWidth = (unsigned long)ScrrenWidth;
		dmScrrenSettings.dmPelsHeight = (unsigned long)ScrrenHeight;
		dmScrrenSettings.dmBitsPerPel = 32;
		dmScrrenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		//�ı���ʾ�趨,����Ϊȫ��Ļ
		ChangeDisplaySettings(&dmScrrenSettings, CDS_FULLSCREEN);

		//�趨�������Ͻǵ�λ��
		posX = posY = 0;
	}
	else
	{
		//������ڻ�,�趨Ϊ800*600�ֱ���
		ScrrenWidth = 800;
		ScrrenHeight = 600;

		//����λ����Ļ��΢�����Ϸ�
		posX = (GetSystemMetrics(SM_CXSCREEN) - ScrrenWidth) / 2-200;
		posY = (GetSystemMetrics(SM_CYSCREEN) - ScrrenHeight) / 2-100;

	}

	//��������,���һ�ȡ���ڵľ��
	mHwnd = CreateWindowEx(WS_EX_APPWINDOW, mApplicationName, mApplicationName,
		WS_OVERLAPPEDWINDOW,
		posX, posY, ScrrenWidth, ScrrenHeight, NULL, NULL, mHinstance, NULL);

	//��������ʾ����Ļ֮��,���趨�ô���Ϊ��Ҫ���е�
	ShowWindow(mHwnd, SW_SHOW);
	SetForegroundWindow(mHwnd);
	SetFocus(mHwnd);

	//���������
	//ShowCursor(false);
}

void SystemClass::ShutdownWindow()
{
	//��ʾ�����
	//ShowCursor(true);

	//����뿪ȫ��Ļģʽ,�ָ���ʾ�趨
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	//�Ƴ�(�ƻ�)����
	DestroyWindow(mHwnd);
	mHwnd = NULL;

	//�Ƴ�����ʵ��
	UnregisterClass(mApplicationName, mHinstance);
	mHinstance = NULL;

	//�ÿ�Ӧ�������
	D3DAPP = NULL;

}

