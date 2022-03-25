#include"GraphicsClass.h"
#include "TexClass.h"
#include "../Core/GameObject.h"
#include "SystemClass.h"
shared_ptr<GameObject> mGameObject;
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

GraphicsClass::GraphicsClass():mBoxCB(0),mFrameBoxCB(0)
{
	//初始化四个类的指针
	mD3D = NULL;
	mCamera = NULL;
	mColorShader = NULL;
	mModel = NULL;

	// 直射光
	mDirLight.Ambient = XMFLOAT4(1,0,0, 1.0f);
	mDirLight.Diffuse = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	//mDirLight.Diffuse = XMFLOAT4(0.0f, 0, 0.9f, 1.0f);
	mDirLight.Specular = XMFLOAT4(0, 1, 0, 1.0f);
	//mDirLight.Direction = XMFLOAT3(0.57735f, -0.57735f, 0.57735f);
	mDirLight.Direction = XMFLOAT3(0.0f, 0.7f, 0.7f);
}

GraphicsClass::~GraphicsClass()
{
	ReleaseCOM(mBoxCB);
	ReleaseCOM(mFrameBoxCB);
}

ID3D11ShaderResourceView* GraphicsClass::LoadTexture(const WCHAR* path)
{
	TexClass* tex = new TexClass();
	//tex->Initilize(g_Device, L"TexturesAndMat/Gun_Texture.png");
	tex->Initilize(g_Device, path);
	ID3D11ShaderResourceView* texture = tex->GetTexture();

	//mShader2->Apply();

	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	ID3D11SamplerState* textureState;//暂时不知道有什么用
	auto hr = g_Device->CreateSamplerState(&sampDesc, &textureState);
	//return textureState;
	return texture;
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

	//创建GameObject
	shared_ptr<GameObject> go = shared_ptr<GameObject>(new GameObject());
	//shared_ptr<MeshComponent> mesh = shared_ptr<MeshComponent>(new MeshComponent("TObjects/zuoqi.FBX"));
	shared_ptr<MeshComponent> mesh = shared_ptr<MeshComponent>(new MeshComponent("TObjects/Tormentor.FBX"));
	
	//mesh->SetFirstMesh();//测试：：构建最基础的一个四边形 顶点 + index
	mGameObject = go;
	GDirectxCore->models.push_back(*go);
	go->SetMesh(mesh);
	go->RenderMesh();
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

	//第四-0，使用新的ShaderLib
	mShader2 = make_shared<VertexPixelShader>("MyShader.fx");
	

	////第四,创建ColorShaderClass,并且进行初始化
	//// 暂时只支持
	//// {
	////	pos;
	////	uv;
	//// }
	//mColorShader = new ColorShaderClass();
	//if (!mColorShader)
	//{
	//	return false;
	//}
	//result = mColorShader->Initialize(mD3D->GetDevice(), hwnd);
	//if (!result)
	//{
	//	MessageBox(hwnd, L"mColorClass Initialize failure", NULL, MB_OK);
	//	return false;
	//}

	//添加，使用纹理
	TexClass* tex = new TexClass();
	tex->Initilize(mD3D->GetDevice(), L"TexturesAndMat/Tormentor_Texturing.png");
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
	if (mShader2)
	{
		//delete mShader2;

	}
}



bool GraphicsClass::Frame()
{
	//这段代码都写的挺烂的说。。。。。。
	bool result;
	result = Render();
	if (!result)
		return false;
	return true;
}
bool isLogIndex;
bool isResetBufferByShader;
bool isLogUseOldShader;
bool isLogNewShader;
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

	/*
	* 第四~第五会绘制一个方格，只支持这种Input
	*	{
		XMFLOAT3 pos;
		XMFLOAT2 uv;
		}
		同时也屏蔽掉了colorClass的初始化，在GraphicsClass::Initialize（）
	*/
	//第四,设置顶点缓存和索引缓存和拓扑结构体
	//mModel->Render(mD3D->GetDeviceContext());
	//
	//
	////第五,设置VertexShader和PixelShader，InputLayout 以及常量缓存的值,并最终进行绘制
	//if(isLogIndex==false )
	//{
	//	printf("shader indexCount=%d" + mModel->GetIndexCount());
	//	isLogIndex = true;
	//}
	//result = mColorShader->Render(mD3D->GetDeviceContext(),mModel->GetIndexCount(), WorldMatrix, ViewMatrix, ProjMatrix);
	//if (!result)
	//{
	//	MessageBox(NULL, L"ColorShader Render failure", NULL, MB_OK);
	//	return false;
	//}

	if (mShader2->mRefInputLayout.size() == 0)//这里加个保护，否则后面会溢出，GPU溢出，整个win系统崩溃
	{
		if (isLogNewShader == false)
		{
			isLogNewShader = true;
			printf("mShader2初始化有问题，inputLayout == 0 ，请检查\n");
		}
		return true;
		//return false;//外面没有做保护，返回false会直接退出
	}
	auto worldMatrix = mGameObject->GetWorldMatrix();
	auto worldInvMatrix = mGameObject->GetInvenseTranspose(worldMatrix);
	auto inputLayoutLen = mShader2->mReflectLayoutSize;

	//根据shader改变buffer..和顶点（剔除没用参数，或者补足）
	if (isResetBufferByShader==false)
	{
		mGameObject->m_pMesh->ResetBuffer(mShader2->mRefInputLayout);
		isResetBufferByShader = true;
	}

	//注意会不停输出， Log模块没做好。。。。。。。。。。
	//printf("len=%d\n", inputLayoutLen);
	//printf("lenStride = %d \n", sizeof(VertexPCNTT));
	UINT ic = mGameObject->RenderTest(inputLayoutLen);

	//DX11 7.1
// 设置常量缓存区
	D3D11_BUFFER_DESC cbd;
	ZeroMemory(&cbd, sizeof(cbd));
	cbd.Usage = D3D11_USAGE_DEFAULT;
	cbd.ByteWidth = sizeof(ConstantBuffer);
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.CPUAccessFlags = 0;
	HR(g_Device->CreateBuffer(&cbd, nullptr, &mBoxCB));
	g_DeviceContext->VSSetConstantBuffers(0, 1, &mBoxCB);	// VS常量缓冲区对应b0

	D3D11_BUFFER_DESC cbdFrame;
	ZeroMemory(&cbdFrame, sizeof(cbdFrame));
	cbdFrame.Usage = D3D11_USAGE_DEFAULT;
	cbdFrame.ByteWidth = sizeof(FrameConstantBuffer);
	cbdFrame.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbdFrame.CPUAccessFlags = 0;
	HR(g_Device->CreateBuffer(&cbdFrame, nullptr, &mFrameBoxCB));
	g_DeviceContext->PSSetConstantBuffers(1, 1, &mFrameBoxCB);	// PS常量缓冲区对应b1


	bool useOld = false;
	if(useOld)
	{ 
		if (mColorShader)
		{
			//旧的shader->render
			result = mColorShader->Render(mD3D->GetDeviceContext(), ic, worldMatrix, ViewMatrix, ProjMatrix);
			//(无用) mColorShader->SetShaderParameterOutside(mD3D->GetDeviceContext(), worldMatrix, ViewMatrix, ProjMatrix);
		}
		else
		{
			if (isLogUseOldShader == false)
			{
				printf("用旧的Shader REnder有问题，没修复 \n");
				isLogUseOldShader = true;
			}
		}
	}
	else
	{
		//world = DirectX::XMLoadFloat4x4(&mWavesWorld);	// Waves位置
	//view = DirectX::XMLoadFloat4x4(&mView);
	//proj = DirectX::XMLoadFloat4x4(&mProj);
	//mCBuffer.gWorld = world;
		//g_DeviceContext->UpdateSubresource(mBoxCB, 0, nullptr, &mCBuffer, 0, 0);

		// 更新片元常量(新的shader无法用？？？）
		//mFrameCBuffer.gMaterial = mHillsMat;	// 更新Hills的material
		mFrameCBuffer.gDirLight = mDirLight;
		g_DeviceContext->UpdateSubresource(mFrameBoxCB, 0, nullptr, &mFrameCBuffer, 0, 0);

		//新的shader
		//这个方法Apply()，缺少matrix,需要先调用？？？ :SetShaderParameterOutside()
		mShader2->SetMatrix("World", worldMatrix);
		mShader2->SetMatrix("View", ViewMatrix);
		mShader2->SetMatrix("Proj", ProjMatrix);
		mShader2->SetMatrix("WorldInvTranpose", worldInvMatrix);
//		mShader2->SetRWStructBufferInData("gDirLight", &mDirLight,4);
		XMFLOAT4 dir = XMFLOAT4();
		dir.x = mDirLight.Direction.x;
		dir.y = mDirLight.Direction.y;
		dir.z = mDirLight.Direction.z;
		dir.w = 0;
		mShader2->SetFloat4("gLiteColor", dir);


		XMMATRIX ss;
		memcpy(&ss, &mDirLight, sizeof(XMMATRIX));
		mShader2->SetMatrixArrayElement("gDirLight", ss, 0);
		mShader2->Apply();//IASetInputLayout();->VSSetShader();->PSSetShader()

		////一步步，释出不行原因（找到inputLayout的问题，并解决了还是不知道根本原因）
		//mColorShader->SetShaderParameterOutside(g_DeviceContext, worldMatrix, ViewMatrix, ProjMatrix);
		//////不行1：
		//////mShader2->RenderTradition(worldMatrix, ViewMatrix, ProjMatrix);
		//////不行2
		////mShader2->ApplyDirectXOnly();
		//mShader2->ApplyDirectXOnly(mColorShader->md3dVertexShader,
		//	mColorShader->md3dPixelShader,
		//	mColorShader->md3dInputLayout);
		// 

	

		//渲染三角形（shader render必须）
		g_DeviceContext->DrawIndexed(ic, 0, 0);
	}

	//把渲染的场景呈献给屏幕
	mD3D->EndScene();
	return true;
}