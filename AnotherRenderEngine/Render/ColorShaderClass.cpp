#include"ColorShaderClass.h"

#include <vector> 
#include <string> 
#include <fstream> 
#include <iostream> 

using namespace std;

/// <summary>
/// 原Shader类，增加了一些读二进制测试（个人目标），但现在改用Core/Shader.cpp
/// </summary>
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
	// flags 参考：
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
		return false;
	}
	
	//WriteReadShader(&VertexShaderBuffer, L"fileX.binary");//会读临时文件，是Pos,color格式(第一版），没有uv

#pragma region 保存二进制文件，错误的测试 一堆



	///////////////////////// 插入一个保存文件测试  /////////////////////////////////////
	//// #include <fstream>//STD::写法，初级程序员写法，不能处理大文件
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

	//const unsigned long long size = 8ULL * 1024ULL * 1024ULL;//80G大文件，测试用。。。。
	//unsigned long long a[size];
	//FILE* pFile;
	//pFile = fopen("file.binary", "wb");
	//for (unsigned long long j = 0; j < 1024; ++j) {
	//	//Some calculations to fill a[]
	//	fwrite(a, 1, size * sizeof(unsigned long long), pFile);
	//}
	//fclose(pFile);
	
	//////////////// Read 1 有错 //////////////////////
	//////TODO:需要添加预处理定义：_CRT_SECURE_NO_WARNINGS，所以开源上传时需去掉这段代码
	////long size = sizeof(ID3D10Blob);
	//////int n = 555;
	//////int n = 15;
	//////size = sizeof(int);//测试用，注意发布时不要，要用Blob*指针
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
	/////////////////// IO 库 ///////////////////////////
	//////auto s = bufferToString(VertexShaderBuffer, buffLength);
	//////auto s = std::string(&VertexShaderBuffer[0], &VertexShaderBuffer[buffLength]);
	////ifstream input("file.binary",std::ios::binary);
	//////复制一份
	////ofstream output("file_copy.txt", ios::binary);
	////std::copy(
	////	std::istreambuf_iterator<char>(input),
	////	std::istreambuf_iterator<char>(),
	////	std::ostreambuf_iterator<char>(output));
	////output.close();

	//////前面用了一份，就不能用第二次，会报错了？？？？
	////std::vector<char> buffer(std::istreambuf_iterator<char>(input), {});
	////auto sSize = sizeof(buffer);//struct size不是长度，不要用
	////char* strCharB_ = &buffer[0];
	//////auto saveFileSize = sizeof(buffer);//struct size不是长度，不要用
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
	//因为有【局部变量】和【监视1】2个帮助窗口，所以放弃printF
	//printf("%s", "ssss");
	//printf("%s", md3dVertexShader);
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
		
		return false;
	}
	//WriteReadShader(&PixelShaderBuffer, L"fileXPixel.binary");//写|读 binary
	WriteShaderBytecode();//写  binary.txt
	HR2(d3dDevice->CreatePixelShader(PixelShaderBuffer->GetBufferPointer(), PixelShaderBuffer->GetBufferSize(), NULL, &md3dPixelShader));

	//https://zealie.me/2018/09/01/DX1102.html
	//第四,填充输入布局形容结构体,创建输入布局（注意必须和 ModelClass.h->Vertex 的结构图对应
	D3D11_INPUT_ELEMENT_DESC VertexInputLayout[] =
	{
		{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 }, // 96位即12个字节
		//{ "COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};

	unsigned int numElements = sizeof(VertexInputLayout) / sizeof(VertexInputLayout[0]);         //布局数量
	
	HR2(d3dDevice->CreateInputLayout(VertexInputLayout, numElements, VertexShaderBuffer->GetBufferPointer(), VertexShaderBuffer->GetBufferSize(), &md3dInputLayout));
	//HR2(d3dDevice->CreateInputLayout(VertexInputLayout, numElements, bufferReplace->GetBufferPointer(), bufferReplace->GetBufferSize(), &md3dInputLayout));

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
/*/// <summary>
/// 一些buffer转string参考：
/// https://stackoverflow.com/questions/896103/good-methods-for-converting-char-array-buffers-to-strings
/// </summary>
/// <param name="buffer"></param>
/// <param name="bufflen"></param>
/// <param name="str"></param>
/// <returns></returns>*/
//string& bufferToString(char* buffer, int bufflen, string& str)
//{
//	char temp[bufflen];//有错，应该是c写法，c++不支持
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
bool ColorShaderClass::SetShaderParameterOutside(ID3D11DeviceContext* d3dDeviceContext, CXMMATRIX worldMatrix, CXMMATRIX viewMatrix, CXMMATRIX projMatrix)
{
	return SetShaderParameter(d3dDeviceContext, worldMatrix, viewMatrix, projMatrix);
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