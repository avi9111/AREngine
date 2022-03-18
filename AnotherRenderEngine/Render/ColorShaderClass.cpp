#include"ColorShaderClass.h"

ColorShaderClass::ColorShaderClass()
{
	 md3dVertexShader=NULL;
	 md3dPixelShader=NULL;
	 md3dInputLayout=NULL;
	 mCBMatrixBuffer=NULL;
}


ColorShaderClass::ColorShaderClass(const ColorShaderClass&)
{

}

ColorShaderClass::~ColorShaderClass()
{

}


bool ColorShaderClass::Initialize(ID3D11Device* d3dDevice, HWND hwnd)
{
	bool result;
	//result = InitializeShader(d3dDevice, hwnd, L"MyShader.fx", L"MyShader.fx");
	result = InitializeShader(d3dDevice, hwnd, L"MyShader.fx", L"MyShader.fx");
	if (!result)
		return false;

	return true;
}

void ColorShaderClass::Shutdown()
{
	ShutdownShader();
}
/// <summary>
/// 
/// </summary>
/// <param name="d3dDeviceContext"></param>
/// <param name="indexCount">object index(索引）</param>
/// <param name="worldMatrix"></param>
/// <param name="viewMatrix"></param>
/// <param name="ProjMatrix"></param>
/// <returns></returns>
bool ColorShaderClass::Render(ID3D11DeviceContext* d3dDeviceContext, int indexCount, CXMMATRIX worldMatrix, CXMMATRIX viewMatrix, CXMMATRIX ProjMatrix)
{
	bool result;

	//设置用来渲染的Shader属性
	result = SetShaderParameter(d3dDeviceContext, worldMatrix, viewMatrix, ProjMatrix);
	if (!result)
		return false;

	//渲染Shader
	RenderShader(d3dDeviceContext, indexCount);
	
	return true;
}

bool ColorShaderClass::InitializeShader(ID3D11Device* d3dDevice, HWND hwnd, WCHAR const * VSFileName, WCHAR const * PSFileName)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* VertexShaderBuffer;  
	ID3D10Blob* PixelShaderBuffer;
	
	//第一,初始化参数
	errorMessage = NULL;
    VertexShaderBuffer=NULL;
	PixelShaderBuffer=NULL;

	//第二,编译VertexShader代码,并创建VertexShader
	result = D3DX11CompileFromFile(VSFileName, NULL, NULL, "VS", "vs_4_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, NULL, &VertexShaderBuffer, &errorMessage, NULL);
	if (FAILED(result))
	{
		//存在错误信息
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, VSFileName);
		}
		//不存在错误信息,也就是没有找到Shader文件
		else
		{
			MessageBox(hwnd, L"can not find VS file", L"error", MB_OK);
		}
	}

	HR2(d3dDevice->CreateVertexShader(VertexShaderBuffer->GetBufferPointer(),VertexShaderBuffer->GetBufferSize(),NULL,&md3dVertexShader));


	//第三,编译PixelShader,并创建PixelShader
	result = D3DX11CompileFromFile(PSFileName, NULL, NULL, "PS", "ps_4_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, NULL, &PixelShaderBuffer, &errorMessage, NULL);
	if (FAILED(result))
	{
		//存在错误信息
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, PSFileName);
		}
		//不存在错误信息,也就是没有找到Shader文件
		else
		{
			MessageBox(hwnd, L"can not find PS file", L"error", MB_OK);
		}
	}

	HR2(d3dDevice->CreatePixelShader(PixelShaderBuffer->GetBufferPointer(), PixelShaderBuffer->GetBufferSize(), NULL, &md3dPixelShader));

	//第四,填充输入布局形容结构体,创建输入布局
	D3D11_INPUT_ELEMENT_DESC VertexInputLayout[] =
	{
		{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 }, // 96位即12个字节
		{ "COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};

	unsigned int numElements = sizeof(VertexInputLayout) / sizeof(VertexInputLayout[0]);         //布局数量
	
	HR2(d3dDevice->CreateInputLayout(VertexInputLayout, numElements, VertexShaderBuffer->GetBufferPointer(), VertexShaderBuffer->GetBufferSize(), &md3dInputLayout));

	//第五,释放VertexShaderBuffer和PixelShaderBuffer
	VertexShaderBuffer->Release();
	VertexShaderBuffer = NULL;
	PixelShaderBuffer->Release();
	PixelShaderBuffer = NULL;

	//第六,设置(常量)缓存形容结构体,并创建常量缓存
	D3D11_BUFFER_DESC matrixBufferDesc;
	ZeroMemory(&matrixBufferDesc, sizeof(matrixBufferDesc));
	matrixBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	matrixBufferDesc.ByteWidth = sizeof(CBMatrix);   //结构体大小,必须为16字节倍数
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = 0; 

	HR2(d3dDevice->CreateBuffer(&matrixBufferDesc, NULL, &mCBMatrixBuffer));

    return true;
}

bool ColorShaderClass::ShutdownShader()
{
	HR2(mCBMatrixBuffer);
	HR2(md3dInputLayout);
	HR2(md3dPixelShader);
	HR2(md3dVertexShader);
	return true;
}

void ColorShaderClass::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR const * shaderFilename)
{
	char* compileErrors;
	unsigned long bufferSize, i;
	ofstream fout;


	// 获取指向错误信息文本的指针
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// 获取错误信息文本的长度
	bufferSize = errorMessage->GetBufferSize();

	// 创建一个txt,用于写入错误信息
	fout.open("shader-error.txt");

	//想txt文件写入错误信息
	for (i = 0; i<bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	// 关闭文件
	fout.close();

	// Release the error message.
	errorMessage->Release();
	errorMessage = 0;

    //弹出提醒的小窗口
	MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);

}


bool ColorShaderClass::SetShaderParameter(ID3D11DeviceContext* d3dDeviceContext, CXMMATRIX worldMatrix, CXMMATRIX viewMatrix, CXMMATRIX ProjMatrix)
{
	//D3D11_MAPPED_SUBRESOURCE mappedResource;
	

	//CBMatrix* cbPtr;
	unsigned int bufferNum;

	//将矩阵转置,在传入常量缓存前进行转置,因为GPU对矩阵数据会自动进行一次转置
	CBMatrix cb;
	XMMATRIX worldMa = XMMatrixTranspose(worldMatrix);
	XMMATRIX viewMa = XMMatrixTranspose(viewMatrix);
	XMMATRIX ProjMa = XMMatrixTranspose(ProjMatrix);
	cb.mWorldMatrix = worldMa;
	cb.mViewMatrix = viewMa;
	cb.mProjMatrix = ProjMa;
	d3dDeviceContext->UpdateSubresource(mCBMatrixBuffer, 0, NULL, &cb, 0, 0);
	/**/
	//锁定常量缓存,这时候常量缓存和子资源关联在一起
	//HR2(d3dDeviceContext->Map(mCBMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));

	//获取指向常量缓存数据的指针
	//cbPtr = (CBMatrix*)mappedResource.pData;

	//赋予常量缓存数据
	//cbPtr->mProjMatrix = worldMa;
	//cbPtr->mViewMatrix = viewMa;
	//cbPtr->mProjMatrix = ProjMa;

	//解锁常量缓存
	//d3dDeviceContext->Unmap(mCBMatrixBuffer, 0);
	
	//设置在顶点缓存中常量缓存的位置,注册号
	bufferNum = 0;

	//设置在VertexShader的常量缓存的值(带着更新的值)
	d3dDeviceContext->VSSetConstantBuffers(bufferNum, 1, &mCBMatrixBuffer); 

	return true;
}
/// <summary>
/// 
/// </summary>
/// <param name="deviceContext"></param>
/// <param name="indexCount">第几个object(object索引）</param>
void ColorShaderClass::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
	//设置顶点输入布局
	deviceContext->IASetInputLayout(md3dInputLayout);

	//设置VertexShader和PixelShader
	deviceContext->VSSetShader(md3dVertexShader, NULL, 0);
	deviceContext->PSSetShader(md3dPixelShader, NULL, 0);

	//渲染三角形
	deviceContext->DrawIndexed(indexCount, 0, 0);
}