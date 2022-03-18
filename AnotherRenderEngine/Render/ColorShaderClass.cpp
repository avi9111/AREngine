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
/// <param name="indexCount">object index(������</param>
/// <param name="worldMatrix"></param>
/// <param name="viewMatrix"></param>
/// <param name="ProjMatrix"></param>
/// <returns></returns>
bool ColorShaderClass::Render(ID3D11DeviceContext* d3dDeviceContext, int indexCount, CXMMATRIX worldMatrix, CXMMATRIX viewMatrix, CXMMATRIX ProjMatrix)
{
	bool result;

	//����������Ⱦ��Shader����
	result = SetShaderParameter(d3dDeviceContext, worldMatrix, viewMatrix, ProjMatrix);
	if (!result)
		return false;

	//��ȾShader
	RenderShader(d3dDeviceContext, indexCount);
	
	return true;
}

bool ColorShaderClass::InitializeShader(ID3D11Device* d3dDevice, HWND hwnd, WCHAR const * VSFileName, WCHAR const * PSFileName)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* VertexShaderBuffer;  
	ID3D10Blob* PixelShaderBuffer;
	
	//��һ,��ʼ������
	errorMessage = NULL;
    VertexShaderBuffer=NULL;
	PixelShaderBuffer=NULL;

	//�ڶ�,����VertexShader����,������VertexShader
	result = D3DX11CompileFromFile(VSFileName, NULL, NULL, "VS", "vs_4_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, NULL, &VertexShaderBuffer, &errorMessage, NULL);
	if (FAILED(result))
	{
		//���ڴ�����Ϣ
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, VSFileName);
		}
		//�����ڴ�����Ϣ,Ҳ����û���ҵ�Shader�ļ�
		else
		{
			MessageBox(hwnd, L"can not find VS file", L"error", MB_OK);
		}
	}

	HR2(d3dDevice->CreateVertexShader(VertexShaderBuffer->GetBufferPointer(),VertexShaderBuffer->GetBufferSize(),NULL,&md3dVertexShader));


	//����,����PixelShader,������PixelShader
	result = D3DX11CompileFromFile(PSFileName, NULL, NULL, "PS", "ps_4_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, NULL, &PixelShaderBuffer, &errorMessage, NULL);
	if (FAILED(result))
	{
		//���ڴ�����Ϣ
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, PSFileName);
		}
		//�����ڴ�����Ϣ,Ҳ����û���ҵ�Shader�ļ�
		else
		{
			MessageBox(hwnd, L"can not find PS file", L"error", MB_OK);
		}
	}

	HR2(d3dDevice->CreatePixelShader(PixelShaderBuffer->GetBufferPointer(), PixelShaderBuffer->GetBufferSize(), NULL, &md3dPixelShader));

	//����,������벼�����ݽṹ��,�������벼��
	D3D11_INPUT_ELEMENT_DESC VertexInputLayout[] =
	{
		{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 }, // 96λ��12���ֽ�
		{ "COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};

	unsigned int numElements = sizeof(VertexInputLayout) / sizeof(VertexInputLayout[0]);         //��������
	
	HR2(d3dDevice->CreateInputLayout(VertexInputLayout, numElements, VertexShaderBuffer->GetBufferPointer(), VertexShaderBuffer->GetBufferSize(), &md3dInputLayout));

	//����,�ͷ�VertexShaderBuffer��PixelShaderBuffer
	VertexShaderBuffer->Release();
	VertexShaderBuffer = NULL;
	PixelShaderBuffer->Release();
	PixelShaderBuffer = NULL;

	//����,����(����)�������ݽṹ��,��������������
	D3D11_BUFFER_DESC matrixBufferDesc;
	ZeroMemory(&matrixBufferDesc, sizeof(matrixBufferDesc));
	matrixBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	matrixBufferDesc.ByteWidth = sizeof(CBMatrix);   //�ṹ���С,����Ϊ16�ֽڱ���
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


	// ��ȡָ�������Ϣ�ı���ָ��
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// ��ȡ������Ϣ�ı��ĳ���
	bufferSize = errorMessage->GetBufferSize();

	// ����һ��txt,����д�������Ϣ
	fout.open("shader-error.txt");

	//��txt�ļ�д�������Ϣ
	for (i = 0; i<bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	// �ر��ļ�
	fout.close();

	// Release the error message.
	errorMessage->Release();
	errorMessage = 0;

    //�������ѵ�С����
	MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);

}


bool ColorShaderClass::SetShaderParameter(ID3D11DeviceContext* d3dDeviceContext, CXMMATRIX worldMatrix, CXMMATRIX viewMatrix, CXMMATRIX ProjMatrix)
{
	//D3D11_MAPPED_SUBRESOURCE mappedResource;
	

	//CBMatrix* cbPtr;
	unsigned int bufferNum;

	//������ת��,�ڴ��볣������ǰ����ת��,��ΪGPU�Ծ������ݻ��Զ�����һ��ת��
	CBMatrix cb;
	XMMATRIX worldMa = XMMatrixTranspose(worldMatrix);
	XMMATRIX viewMa = XMMatrixTranspose(viewMatrix);
	XMMATRIX ProjMa = XMMatrixTranspose(ProjMatrix);
	cb.mWorldMatrix = worldMa;
	cb.mViewMatrix = viewMa;
	cb.mProjMatrix = ProjMa;
	d3dDeviceContext->UpdateSubresource(mCBMatrixBuffer, 0, NULL, &cb, 0, 0);
	/**/
	//������������,��ʱ�������������Դ������һ��
	//HR2(d3dDeviceContext->Map(mCBMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));

	//��ȡָ�����������ݵ�ָ��
	//cbPtr = (CBMatrix*)mappedResource.pData;

	//���賣����������
	//cbPtr->mProjMatrix = worldMa;
	//cbPtr->mViewMatrix = viewMa;
	//cbPtr->mProjMatrix = ProjMa;

	//������������
	//d3dDeviceContext->Unmap(mCBMatrixBuffer, 0);
	
	//�����ڶ��㻺���г��������λ��,ע���
	bufferNum = 0;

	//������VertexShader�ĳ��������ֵ(���Ÿ��µ�ֵ)
	d3dDeviceContext->VSSetConstantBuffers(bufferNum, 1, &mCBMatrixBuffer); 

	return true;
}
/// <summary>
/// 
/// </summary>
/// <param name="deviceContext"></param>
/// <param name="indexCount">�ڼ���object(object������</param>
void ColorShaderClass::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
	//���ö������벼��
	deviceContext->IASetInputLayout(md3dInputLayout);

	//����VertexShader��PixelShader
	deviceContext->VSSetShader(md3dVertexShader, NULL, 0);
	deviceContext->PSSetShader(md3dPixelShader, NULL, 0);

	//��Ⱦ������
	deviceContext->DrawIndexed(indexCount, 0, 0);
}