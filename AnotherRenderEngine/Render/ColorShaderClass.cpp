#include"ColorShaderClass.h"

#include <vector> 
#include <string> 
#include <fstream> 
#include <iostream> 

using namespace std;


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
	//result = InitializeShader(d3dDevice, hwnd, L"Shader/s1.fx", L"Shader/s1.fx");
	if (ShaderIndex == 2)
	{
		result = InitializeShader(d3dDevice, hwnd, L"Shader/s2.fx", L"Shader/s2.fx");
	}
	else
	{
		result = InitializeShader(d3dDevice, hwnd, L"MyShader.fx", L"MyShader.fx");
	}
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
std::string bufferToString(char* buffer, int bufflen)
{
	std::string ret(buffer, bufflen);

	return ret;
}
HRESULT WriteReadShader(ID3DBlob** ppSBlob, const wchar_t* const sTargetFileName)
{
	HRESULT hr;

	FILE* pFile;

//#ifdef AUTSHADERS_SAVE_TO_FILE
	/*_wfopen_s(&pFile, sTargetFileName, L"wb");
	fwrite((*ppSBlob)->GetBufferPointer(), (*ppSBlob)->GetBufferSize(), 1, pFile);
	fclose(pFile);*/
//#endif

	int nSize = 0;
	_wfopen_s(&pFile, sTargetFileName, L"rb");

	fseek(pFile, 0, SEEK_END);
	nSize = ftell(pFile);
	fseek(pFile, 0, SEEK_SET);

	*ppSBlob = NULL;
	//V_RETURN(D3DCreateBlob(nSize, ppSBlob));
	D3DCreateBlob(nSize, ppSBlob);

	fread_s((*ppSBlob)->GetBufferPointer(), nSize, nSize, 1, pFile);

	fclose(pFile);

	return S_OK;
}
void WriteShaderBytecode() {
	char const* input_file = "fileXPixel.binary";
	FILE* fp = fopen(input_file, "rb");
	fseek(fp, 0, SEEK_END);
	long bytecode_size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	char* bytecode = new char[bytecode_size];
	fread(bytecode, sizeof(char), bytecode_size, fp);
	fclose(fp);

	ID3DBlob* code = NULL;
	//int flags = atoi(argv[3]);
	// flags �ο���
	// https://docs.microsoft.com/en-us/windows/win32/api/d3dcompiler/ne-d3dcompiler-d3dcompiler_strip_flags
	//int hr = d3d_compiler.D3DStripShader(bytecode, bytecode_size, flags, &code);
	int hr = D3DStripShader(bytecode, bytecode_size, 0, &code);
	if (FAILED(hr))
	{
		printf("Strip error: 0x%x\n", hr);
	}

	char const* output_file = "filePixcelDecode.txt";
	fp = fopen(output_file, "wb");
	fwrite(code->GetBufferPointer(), sizeof(char), code->GetBufferSize(), fp);
	fclose(fp);

	delete[] bytecode;
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
		return false;
	}
	
	//WriteReadShader(&VertexShaderBuffer, L"fileX.binary");//�����ʱ�ļ�����Pos,color��ʽ(��һ�棩��û��uv

#pragma region ����������ļ�������Ĳ��� һ��



	///////////////////////// ����һ�������ļ�����  /////////////////////////////////////
	//// #include <fstream>//STD::д������������Աд�������ܴ�����ļ�
	//const unsigned long long size = 64ULL * 1024ULL * 1024ULL;
	//unsigned long long a[size];
	//int main()
	//{
	//	std::fstream myfile;
	//	myfile = std::fstream("file.binary", std::ios::out | std::ios::binary);
	//	//Here would be some error handling
	//	for (int i = 0; i < 32; ++i) {
	//		//Some calculations to fill a[]
	//		myfile.write((char*)&a, size * sizeof(unsigned long long));
	//	}
	//	myfile.close();
	//}

	//const unsigned long long size = 8ULL * 1024ULL * 1024ULL;//80G���ļ��������á�������
	//unsigned long long a[size];
	//FILE* pFile;
	//pFile = fopen("file.binary", "wb");
	//for (unsigned long long j = 0; j < 1024; ++j) {
	//	//Some calculations to fill a[]
	//	fwrite(a, 1, size * sizeof(unsigned long long), pFile);
	//}
	//fclose(pFile);
	
	//////////////// Read 1 �д� //////////////////////
	//////TODO:��Ҫ���Ԥ�����壺_CRT_SECURE_NO_WARNINGS�����Կ�Դ�ϴ�ʱ��ȥ����δ���
	////long size = sizeof(ID3D10Blob);
	//////int n = 555;
	//////int n = 15;
	//////size = sizeof(int);//�����ã�ע�ⷢ��ʱ��Ҫ��Ҫ��Blob*ָ��
	//FILE* pFile;
	//pFile = fopen("file.binary", "wb");
	//////int buffLength = VertexShaderBuffer->GetBufferSize();
	////fwrite(&VertexShaderBuffer, size, sizeof(VertexShaderBuffer), pFile);
	////fclose(pFile);
	//	// //////////// Read 2 //////////////////////
	////ID3D10Blob* asm_blob;
	////D3DDisassemble(VertexShaderBuffer->GetBufferPointer(), VertexShaderBuffer->GetBufferSize(),
	////	D3D_DISASM_ENABLE_INSTRUCTION_NUMBERING, "", &asm_blob);
	////if (!asm_blob)
	////	return false;

	////fwrite(asm_blob->GetBufferPointer(), 1, asm_blob->GetBufferSize(), stdout);
	////fclose(pFile);
	////asm_blob->Release();
	/////////////// read 3///////////////
	//fwrite(VertexShaderBuffer->GetBufferPointer(), 1, VertexShaderBuffer->GetBufferSize(), pFile);
	//fclose(pFile);

	//pFile = fopen("file.binary", "rb");
	//if (pFile == NULL) { fputs("File error", stderr); exit(1); }
	//// obtain file size:
	//fseek(pFile, 0, SEEK_END);
	//long lSize = ftell(pFile);
	//rewind(pFile);
	//// allocate memory to contain the whole file:
	//ID3D10Blob* bufferReplace = (ID3D10Blob*)malloc(sizeof(ID3D10Blob) * lSize);
	//if (bufferReplace == NULL) { fputs("Memory error", stderr); exit(2); }
	//// copy the file into the buffer:
	//result = fread(bufferReplace, 1, lSize, pFile);
	//if (result != lSize) { fputs("Reading error", stderr); exit(3); }
	//// terminate
	////fclose(pFile);
	////free(bufferReplace);
	/////////////////// IO �� ///////////////////////////
	//////auto s = bufferToString(VertexShaderBuffer, buffLength);
	//////auto s = std::string(&VertexShaderBuffer[0], &VertexShaderBuffer[buffLength]);
	////ifstream input("file.binary",std::ios::binary);
	//////����һ��
	////ofstream output("file_copy.txt", ios::binary);
	////std::copy(
	////	std::istreambuf_iterator<char>(input),
	////	std::istreambuf_iterator<char>(),
	////	std::ostreambuf_iterator<char>(output));
	////output.close();

	//////ǰ������һ�ݣ��Ͳ����õڶ��Σ��ᱨ���ˣ�������
	////std::vector<char> buffer(std::istreambuf_iterator<char>(input), {});
	////auto sSize = sizeof(buffer);//struct size���ǳ��ȣ���Ҫ��
	////char* strCharB_ = &buffer[0];
	//////auto saveFileSize = sizeof(buffer);//struct size���ǳ��ȣ���Ҫ��
	////auto saveFileSize = buffer.size();
	////auto s = bufferToString(strCharB_, saveFileSize);
	////string temp = s;
	////input.close();
	//////////////////////////////////
	//int s1 = VertexShaderBuffer->GetBufferSize();
	//int s2 = bufferReplace->GetBufferSize();

#pragma endregion

	HR2(d3dDevice->CreateVertexShader(VertexShaderBuffer->GetBufferPointer(), VertexShaderBuffer->GetBufferSize(), NULL, &md3dVertexShader));
	//HR2(d3dDevice->CreateVertexShader(bufferReplace->GetBufferPointer(), 1100, NULL, &md3dVertexShader));
	//��Ϊ�С��ֲ��������͡�����1��2���������ڣ����Է���printF
	//printf("%s", "ssss");
	//printf("%s", md3dVertexShader);
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
		
		return false;
	}
	//WriteReadShader(&PixelShaderBuffer, L"fileXPixel.binary");//д|�� binary
	WriteShaderBytecode();//д  binary.txt
	HR2(d3dDevice->CreatePixelShader(PixelShaderBuffer->GetBufferPointer(), PixelShaderBuffer->GetBufferSize(), NULL, &md3dPixelShader));

	//https://zealie.me/2018/09/01/DX1102.html
	//����,������벼�����ݽṹ��,�������벼��
	D3D11_INPUT_ELEMENT_DESC VertexInputLayout[] =
	{
		{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 }, // 96λ��12���ֽ�
		//{ "COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};

	unsigned int numElements = sizeof(VertexInputLayout) / sizeof(VertexInputLayout[0]);         //��������
	
	HR2(d3dDevice->CreateInputLayout(VertexInputLayout, numElements, VertexShaderBuffer->GetBufferPointer(), VertexShaderBuffer->GetBufferSize(), &md3dInputLayout));
	//HR2(d3dDevice->CreateInputLayout(VertexInputLayout, numElements, bufferReplace->GetBufferPointer(), bufferReplace->GetBufferSize(), &md3dInputLayout));

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
/*/// <summary>
/// һЩbufferתstring�ο���
/// https://stackoverflow.com/questions/896103/good-methods-for-converting-char-array-buffers-to-strings
/// </summary>
/// <param name="buffer"></param>
/// <param name="bufflen"></param>
/// <param name="str"></param>
/// <returns></returns>*/
//string& bufferToString(char* buffer, int bufflen, string& str)
//{
//	char temp[bufflen];//�д�Ӧ����cд����c++��֧��
//
//	memset(temp, '\0', bufflen + 1);
//	strncpy(temp, buffer, bufflen);
//
//	return(str.assign(temp));
//}


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