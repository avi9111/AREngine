#include"GraphicsClass.h"

GraphicsClass::GraphicsClass()
{
	//��ʼ���ĸ����ָ��
	mD3D = NULL;
	mCamera = NULL;
	mColorShader = NULL;
	mModel = NULL;
}

GraphicsClass::~GraphicsClass()
{

}

GraphicsClass::GraphicsClass(const GraphicsClass&other)
{

}

bool GraphicsClass::Initialize(int ScreenWidth, int ScreenHeight, HWND hwnd)
{
	bool result;
	
	//��һ,����D3DClass�ಢ�ҳ�ʼ��,D3DClassӦ���ǵ�һ�����������ҳ�ʼ������,��Ϊ����ĵ�ColroShaderClass,ModelClass����Ҫd3dDevice��d3dDeviceContext
	mD3D = new D3DClass();
	if (!mD3D)
	{
		return false;
	}
	result = mD3D->Initialize(ScreenWidth, ScreenHeight, VSYNC_ENABLE, hwnd, FULL_SCREEN, SCREEN_FAR, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"D3DClass Initialize failure", NULL, MB_OK);
		return false;
	}


	//�ڶ�,����CameraClass��
	mCamera = new CameraClass();
	if (!mCamera)
	{
		return false;
	}
	//��ʼ�������λ��
	mCamera->SetPostion(0.0f, 0.0f, -5.0f); 

	//����,����ModelClass���ҳ�ʼ��
	mModel = new ModelClass();
	if (!mModel)
	{
		return false;
	}
	result = mModel->Initialize(mD3D->GetDevice());
	if (!result)
	{
		MessageBox(hwnd, L"ModelClass Initialize failure", NULL, MB_OK);
		return false;
	}

	//����,����ColorShaderClass,���ҽ��г�ʼ��
	mColorShader = new ColorShaderClass();
	if (!mColorShader)
	{
		return false;
	}
	result = mColorShader->Initialize(mD3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"mColorClass Initialize failure", NULL, MB_OK);
		return false;
	}

	return true;
}


void GraphicsClass::Shutdown()
{
	//�ͷ�mD3D
	if (mD3D)
	{
		mD3D->Shutdown();
		delete mD3D;
		mD3D = NULL;
	}

	//�ͷ�mCamera
	if (mCamera)
	{
		delete mCamera;
		mCamera = NULL;
	}

	//�ͷ�mModel
	if (mModel)
	{
		mModel->Shutdown();
		delete mModel;
		mModel = NULL;
	}

	//�ͷ�mColorShader
	if (mColorShader)
	{
		mColorShader->Shutdown();
		delete mColorShader;
		mColorShader = NULL;
	}
}



bool GraphicsClass::Frame()
{
	bool result;
	result = Render();
	if (!result)
		return false;
	return true;
}

bool GraphicsClass::Render()
{
	//�����任����
	XMMATRIX WorldMatrix, ViewMatrix, ProjMatrix;
	bool result;

	//��һ,������濪ʼ���Ƴ���
	mD3D->BeginScene(0.3f, 0.2f, 0.f, 1.0f);

    //�ڶ�,����ViewMatrix(����CameraClass��mPostion��mRotation�����ɵ�)
	mCamera->Render();

	//����,��ȡ�����任����(WorldMatrix��ProjMatrix����mD3D��,ViewMatrix����CameraClass)

	WorldMatrix = mD3D->GetWorldMatrix();

	ProjMatrix = mD3D->GetProjMatrix();

	ViewMatrix = mCamera->GetViewMatrix();

	//����,���ö��㻺���������������˽ṹ��
	mModel->Render(mD3D->GetDeviceContext());

	//����,����VertexShader��PixelShader��InputLayout�Լ����������ֵ,�����ս��л���
	result = mColorShader->Render(mD3D->GetDeviceContext(),mModel->GetIndexCount(), WorldMatrix, ViewMatrix, ProjMatrix);
	if (!result)
	{
		MessageBox(NULL, L"ColorShader Render failure", NULL, MB_OK);
		return false;
	}

	//����Ⱦ�ĳ������׸���Ļ
	mD3D->EndScene();
	return true;
}