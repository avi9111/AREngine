#include"GraphicsClass.h"
#include "TexClass.h"
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
#pragma region �м����dynamic ʵ�����



	//
	///////////////////// ��̬ʹ�÷�������Щ�� ///////////////////
	//https://docs.microsoft.com/zh-cn/windows/win32/direct3d9/performance-optimizations?redirectedfrom=MSDN#Using_Dynamic_Vertex_and_Index_Buffers
	//ID3D11Buffer* mVB1; // stores vertices of type Vertex
	//auto context = mD3D->GetDeviceContext();
	//mVB1->
	//UINT stride = sizeof(mModel->mVertexs);
	//UINT offset = 0;
	//context->IASetVertexBuffers(0, 1, &mVB1, &stride, &offset);//IASet ������������汾���ͻ���

	// USAGE STYLE 1
	// Discard the entire vertex buffer and refill with thousands of vertices.
	// Might contain multiple objects and/or require multiple DrawPrimitive 
	//   calls separated by state changes, etc.
	//�������������������������������������������������������������ܲ���Ҫ��̬�ı� vertexBuffer��ֻ��Ҫ��ͣ���ǰ�漴��
	//�������� ����Model->SetPosition()��ʱʵ��
	//UINT nNumberOfVertices = 4;
	//int m_nVertexStride = 1;//???�д�
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

	//��ӣ�ʹ������
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