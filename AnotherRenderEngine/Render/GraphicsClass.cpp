#include"GraphicsClass.h"
#include "TexClass.h"
#include "../Core/GameObject.h"
D3DClass* GraphicsClass::D3D()
{
	return mD3D;
}
ModelClass* GraphicsClass::Model()
{
	return mModel;
}

ColorShaderClass* GraphicsClass::Shader()
{
	
	return mColorShader;
}

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

	shared_ptr<GameObject> go = shared_ptr<GameObject>(new GameObject());
	shared_ptr<MeshComponent> mesh = shared_ptr<MeshComponent>(new MeshComponent("TObjects/zuoqi.FBX"));
	
	go->SetMesh(mesh);
	//go->RenderMesh();
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
#pragma region 中间插入dynamic 实验代码



	//
	///////////////////// 动态使用方法，有些长 ///////////////////
	//https://docs.microsoft.com/zh-cn/windows/win32/direct3d9/performance-optimizations?redirectedfrom=MSDN#Using_Dynamic_Vertex_and_Index_Buffers
	//ID3D11Buffer* mVB1; // stores vertices of type Vertex
	//auto context = mD3D->GetDeviceContext();
	//mVB1->
	//UINT stride = sizeof(mModel->mVertexs);
	//UINT offset = 0;
	//context->IASetVertexBuffers(0, 1, &mVB1, &stride, &offset);//IASet 这个方法，后面本来就会用

	// USAGE STYLE 1
	// Discard the entire vertex buffer and refill with thousands of vertices.
	// Might contain multiple objects and/or require multiple DrawPrimitive 
	//   calls separated by state changes, etc.
	//。。。。。。。。。。。。。。。。。。。。。。。。。。。。（可能不需要动态改变 vertexBuffer，只需要不停变更前面即可
	//。。。。 已在Model->SetPosition()临时实现
	//UINT nNumberOfVertices = 4;
	//int m_nVertexStride = 1;//???有错
	//// Determine the size of data to be moved into the vertex buffer.
	//UINT nSizeOfData = nNumberOfVertices * m_nVertexStride;
	//// Discard and refill the used portion of the vertex buffer.
	//CONST DWORD dwLockFlags = D3DLOCK_DISCARD;
	//// Lock the vertex buffer.
	//BYTE* pBytes;
	//if (FAILED(m_pVertexBuffer->Lock(0, 0, &pBytes, dwLockFlags)))
	//	return false;
	//// Copy the vertices into the vertex buffer.
	//memcpy(pBytes, pVertices, nSizeOfData);
	//m_pVertexBuffer->Unlock();
	//// Render the primitives.
	//m_pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, nNumberOfVertices / 3)

#pragma endregion

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

	//添加，使用纹理
	TexClass* tex = new TexClass();
	tex->Initilize(mD3D->GetDevice(), L"TexturesAndMat/Gun_Texture.png");
	ID3D11ShaderResourceView* texture = tex->GetTexture();
	auto d3dDeviceContext = mD3D->GetDeviceContext();

	// Describe the Sample State
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
	auto hr = mD3D->GetDevice()->CreateSamplerState(&sampDesc, &textureState);
	d3dDeviceContext->PSSetShaderResources(0, 1, &texture);
	//d3dDeviceContext->PSSetSamplers(0, 1, &textureState);
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

	//第五,设置VertexShader和PixelShader，InputLayout 以及常量缓存的值,并最终进行绘制
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