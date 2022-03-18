#include"GraphicsClass.h"

GraphicsClass::GraphicsClass()
{
	//初始化四个类的指针
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
	
	//第一,创建D3DClass类并且初始化,D3DClass应该是第一个被创建并且初始化的类,因为后面的的ColroShaderClass,ModelClass都需要d3dDevice和d3dDeviceContext
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


	//第二,创建CameraClass类
	mCamera = new CameraClass();
	if (!mCamera)
	{
		return false;
	}
	//初始化相机的位置
	mCamera->SetPostion(0.0f, 0.0f, -5.0f); 

	//第三,创建ModelClass并且初始化
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

	//第四,创建ColorShaderClass,并且进行初始化
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
	//释放mD3D
	if (mD3D)
	{
		mD3D->Shutdown();
		delete mD3D;
		mD3D = NULL;
	}

	//释放mCamera
	if (mCamera)
	{
		delete mCamera;
		mCamera = NULL;
	}

	//释放mModel
	if (mModel)
	{
		mModel->Shutdown();
		delete mModel;
		mModel = NULL;
	}

	//释放mColorShader
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
	//三个变换矩阵
	XMMATRIX WorldMatrix, ViewMatrix, ProjMatrix;
	bool result;

	//第一,清除缓存开始绘制场景
	mD3D->BeginScene(0.3f, 0.2f, 0.f, 1.0f);

    //第二,生成ViewMatrix(根据CameraClass的mPostion和mRotation来生成的)
	mCamera->Render();

	//第三,获取三个变换矩阵(WorldMatrix和ProjMatrix来自mD3D类,ViewMatrix来自CameraClass)

	WorldMatrix = mD3D->GetWorldMatrix();

	ProjMatrix = mD3D->GetProjMatrix();

	ViewMatrix = mCamera->GetViewMatrix();

	//第四,设置顶点缓存和索引缓存和拓扑结构体
	mModel->Render(mD3D->GetDeviceContext());

	//第五,设置VertexShader和PixelShader，InputLayout以及常量缓存的值,并最终进行绘制
	result = mColorShader->Render(mD3D->GetDeviceContext(),mModel->GetIndexCount(), WorldMatrix, ViewMatrix, ProjMatrix);
	if (!result)
	{
		MessageBox(NULL, L"ColorShader Render failure", NULL, MB_OK);
		return false;
	}

	//把渲染的场景呈献给屏幕
	mD3D->EndScene();
	return true;
}