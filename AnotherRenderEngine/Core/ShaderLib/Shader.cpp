#include "Shader.h"
#include "../../Render/D3DClass.h"
#include "../../Render/SystemClass.h"
//#include "Log.h"


const int CONSTANT_BUFFER_SIZE = 16;
const int SHADER_FLOAT_SIZE = 4;
const int SHADER_TWO_FLOAT_SIZE = SHADER_FLOAT_SIZE * 2;
const int SHADER_THREE_FLOAT_SIZE = SHADER_FLOAT_SIZE * 3;
const int SHADER_FOUR_FLOAT_SIZE = SHADER_FLOAT_SIZE * 4;
const int SHADER_MATRIX_SIZE = SHADER_FLOAT_SIZE * 16;


Shader::Shader()
{
}

Shader::~Shader()
{
	ShutDown();
}

void Shader::ShutDown()
{
}

bool Shader::ReflectShaderConstantBuffer(ID3D11ShaderReflection* reflection)
{
	D3D11_SHADER_DESC shaderDesc;
	HR(reflection->GetDesc(&shaderDesc));

	for (int cbIndex = 0; cbIndex < (int)shaderDesc.ConstantBuffers; ++cbIndex)
	{
		int registerIndex = 0;
		ID3D11ShaderReflectionConstantBuffer* buffer = nullptr;
		buffer = reflection->GetConstantBufferByIndex(cbIndex);

		D3D11_SHADER_BUFFER_DESC shaderBufferDesc;
		buffer->GetDesc(&shaderBufferDesc);

		if(shaderBufferDesc.Type == D3D_CBUFFER_TYPE::D3D11_CT_RESOURCE_BIND_INFO)
			continue;

		for (int brIndex = 0; brIndex < (int)shaderDesc.BoundResources; ++brIndex)
		{
			D3D11_SHADER_INPUT_BIND_DESC shaderIbDesc;
			reflection->GetResourceBindingDesc(brIndex, &shaderIbDesc);

			if (0 == strcmp(shaderIbDesc.Name, shaderBufferDesc.Name))
			{
				registerIndex = shaderIbDesc.BindPoint;
				break;
			}
		}

		if (mapShaderContantBuffer.end() == mapShaderContantBuffer.find(shaderBufferDesc.Name))
		{
			shared_ptr<ShaderConstantBuffer> shaderCb =
				shared_ptr<ShaderConstantBuffer>(new ShaderConstantBuffer{ (int)shaderBufferDesc.Size, registerIndex, shaderBufferDesc.Name });

			mapShaderContantBuffer[shaderBufferDesc.Name] = shaderCb;

			for (int shaderVarIndex = 0; shaderVarIndex < (int)shaderBufferDesc.Variables; ++shaderVarIndex)
			{
				ID3D11ShaderReflectionVariable* shaderRefVarible = buffer->GetVariableByIndex(shaderVarIndex);		
				D3D11_SHADER_VARIABLE_DESC shaderVarDesc;
				shaderRefVarible->GetDesc(&shaderVarDesc);
				if (mapShaderVariable.find(shaderVarDesc.Name) == mapShaderVariable.end())
				{
					shared_ptr<ShaderVariable> shaderVarible = shared_ptr<ShaderVariable>
						(new ShaderVariable({ (int)shaderVarDesc.StartOffset,
						(int)shaderVarDesc.Size, shaderVarDesc.Name, shaderBufferDesc.Name }));
					
					shaderCb->vecShaderVariableName.push_back(shaderVarDesc.Name);
					shaderCb->bNeedUpdated = true;
					ShaderVariableType variableType =  ShaderVariableType::SHADER_MATRIX;
					
					if (0 == shaderVarDesc.Size % SHADER_MATRIX_SIZE)
					{
						variableType = ShaderVariableType::SHADER_MATRIX;
					}
					else
					{
						switch (shaderVarDesc.Size)
						{

						case SHADER_FLOAT_SIZE:
							variableType = ShaderVariableType::SHADER_FLOAT;
							break;

						case SHADER_TWO_FLOAT_SIZE:
							variableType = ShaderVariableType::SHADER_FLOAT2;
							break;

						case SHADER_THREE_FLOAT_SIZE:
							variableType = ShaderVariableType::SHADER_FLOAT3;
							break;

						case SHADER_FOUR_FLOAT_SIZE:
							variableType = ShaderVariableType::SHADER_FLOAT4;
							break;
						default:
							break;
						}
					}
						
					shaderVarible->variableType = variableType;
					shaderVarible->variablePre = malloc(shaderVarible->size);
					shaderVarible->variableCurrent = malloc(shaderVarible->size);
					ZeroMemory(shaderVarible->variableCurrent, shaderVarible->size);
					ZeroMemory(shaderVarible->variablePre, shaderVarible->size);
					mapShaderVariable[shaderVarDesc.Name] = shaderVarible;
				}
			}
		}
	}

	return true;
}


bool Shader::CreateConstantBuffer()
{
	for (auto iterator = mapShaderContantBuffer.begin(); iterator != mapShaderContantBuffer.end(); ++iterator)
	{
		shared_ptr<ShaderConstantBuffer> shaderConstantBuffer = iterator->second;
		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.ByteWidth = ((shaderConstantBuffer->size / CONSTANT_BUFFER_SIZE) + 1) * CONSTANT_BUFFER_SIZE;
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		HR(g_Graphics->D3D()->GetDevice()->CreateBuffer(&bufferDesc, NULL, &shaderConstantBuffer->constantBuffer));
	}

	return true;
 }
bool Shader::SetMatrixRaw(const string& variableName, const XMMATRIX& matrix)
{
	if (mapShaderVariable.find(variableName) != mapShaderVariable.end())
	{
		shared_ptr<ShaderVariable> shaderVariable = mapShaderVariable[variableName];
		if (ShaderVariableType::SHADER_MATRIX == shaderVariable->variableType)
		{
			memcpy(shaderVariable->variablePre, shaderVariable->variableCurrent, shaderVariable->size);
			memcpy(shaderVariable->variableCurrent, (void*)&matrix, shaderVariable->size);
		}
	}
	return true;
}
//应该出自xna::Matrix
//https://blog.csdn.net/qq_29523119/article/details/52974572
bool Shader::SetMatrix(const string& variableName, const CXMMATRIX& matrix)
{
	if (mapShaderVariable.find(variableName) != mapShaderVariable.end())
	{
		shared_ptr<ShaderVariable> shaderVariable = mapShaderVariable[variableName];
		if (ShaderVariableType::SHADER_MATRIX == shaderVariable->variableType && sizeof(CXMMATRIX) == shaderVariable->size)
		{
			XMMATRIX memMatrix = XMMatrixTranspose(matrix);
			memcpy(shaderVariable->variablePre, shaderVariable->variableCurrent, shaderVariable->size);
			memcpy(shaderVariable->variableCurrent, (void*)&memMatrix, shaderVariable->size);
		}
	}

	return true;
}


bool Shader::SetFloat(const string& variableName, float value)
{
	if (mapShaderVariable.find(variableName) != mapShaderVariable.end())
	{
		shared_ptr<ShaderVariable> shaderVariable = mapShaderVariable[variableName];
		if (ShaderVariableType::SHADER_FLOAT == shaderVariable->variableType && sizeof(float) == shaderVariable->size)
		{
			memcpy(shaderVariable->variablePre, shaderVariable->variableCurrent, shaderVariable->size);
			memcpy(shaderVariable->variableCurrent, (void*)&value, shaderVariable->size);
		}
	}

	return true;
}

bool Shader::SetFloat2(const string& variableName, XMFLOAT2 value)
{
	if (mapShaderVariable.find(variableName) != mapShaderVariable.end())
	{
		shared_ptr<ShaderVariable> shaderVariable = mapShaderVariable[variableName];
		if (ShaderVariableType::SHADER_FLOAT2 == shaderVariable->variableType && sizeof(XMFLOAT2) == shaderVariable->size)
		{
			memcpy(shaderVariable->variablePre, shaderVariable->variableCurrent, shaderVariable->size);
			memcpy(shaderVariable->variableCurrent, (void*)&value, shaderVariable->size);
		}
	}

	return true;
}


bool Shader::SetFloat3(const string& variableName, XMFLOAT3 value)
{
	if (mapShaderVariable.find(variableName) != mapShaderVariable.end())
	{
		shared_ptr<ShaderVariable> shaderVariable = mapShaderVariable[variableName];
		if (ShaderVariableType::SHADER_FLOAT3 == shaderVariable->variableType && sizeof(XMFLOAT3) == shaderVariable->size)
		{
			memcpy(shaderVariable->variablePre, shaderVariable->variableCurrent, shaderVariable->size);
			memcpy(shaderVariable->variableCurrent, (void*)&value, shaderVariable->size);
		}
	}

	return true;
}

bool Shader::SetFloat4(const string& variableName, XMFLOAT4 value)
{
	if (mapShaderVariable.find(variableName) != mapShaderVariable.end())
	{
		shared_ptr<ShaderVariable> shaderVariable = mapShaderVariable[variableName];
		if (ShaderVariableType::SHADER_FLOAT4 == shaderVariable->variableType && sizeof(XMFLOAT4) == shaderVariable->size)
		{
			memcpy(shaderVariable->variablePre, shaderVariable->variableCurrent, shaderVariable->size);
			memcpy(shaderVariable->variableCurrent, (void*)&value, shaderVariable->size);
		}
	}

	return true;
}

bool Shader::SetUint4(const string& variableName, UINT8 value[4])
{
	if (mapShaderVariable.find(variableName) != mapShaderVariable.end())
	{
		shared_ptr<ShaderVariable> shaderVariable = mapShaderVariable[variableName];
		if (ShaderVariableType::SHADER_FLOAT4 == shaderVariable->variableType && sizeof(XMFLOAT4) == shaderVariable->size)
		{
			memcpy(shaderVariable->variablePre, shaderVariable->variableCurrent, shaderVariable->size);
			memcpy(shaderVariable->variableCurrent, (void*)&value, shaderVariable->size);
		}
	}

	return true;
}

bool Shader::ReflectShaderTexture(ID3D11ShaderReflection* shaderReflection)
{
	D3D11_SHADER_DESC shaderDesc;
	HR(shaderReflection->GetDesc(&shaderDesc));
	for (int brIndex = 0; brIndex < (int)shaderDesc.BoundResources; ++brIndex)
	{
		D3D11_SHADER_INPUT_BIND_DESC shaderIbDesc;
		shaderReflection->GetResourceBindingDesc(brIndex, &shaderIbDesc);
		if (D3D_SIT_TEXTURE == shaderIbDesc.Type)
		{
			shared_ptr<ShaderTexture> shaderTexture = shared_ptr<ShaderTexture>(new ShaderTexture());
			shaderTexture->name = shaderIbDesc.Name;
			shaderTexture->bindPoint = shaderIbDesc.BindPoint;
			if (mapShaderTexture.end() == mapShaderTexture.find(shaderIbDesc.Name))
			{
				mapShaderTexture[shaderIbDesc.Name] = shaderTexture;
			}
		}
	}
	return true;
}

bool Shader::ReflectShaderSampler(ID3D11ShaderReflection* shaderReflection)
{
	D3D11_SHADER_DESC shaderDesc;
	HR(shaderReflection->GetDesc(&shaderDesc));
	for (int brIndex = 0; brIndex < (int)shaderDesc.BoundResources; ++brIndex)
	{
		D3D11_SHADER_INPUT_BIND_DESC shaderIbDesc;
		shaderReflection->GetResourceBindingDesc(brIndex, &shaderIbDesc);
		if (D3D_SIT_SAMPLER == shaderIbDesc.Type)
		{
			shared_ptr<ShaderSampler> shaderTexture = shared_ptr<ShaderSampler>(new ShaderSampler());
			shaderTexture->name = shaderIbDesc.Name;
			shaderTexture->bindPoint = shaderIbDesc.BindPoint;
			if (mapShaderSampler.end() == mapShaderSampler.find(shaderIbDesc.Name))
			{
				mapShaderSampler[shaderIbDesc.Name] = shaderTexture;
			}
		}
	}
	return true;
}

bool Shader::ReflectShaderRWStructBuffer(ID3D11ShaderReflection* reflection)
{
	D3D11_SHADER_DESC shaderDesc;
	HR(reflection->GetDesc(&shaderDesc));

	for (int index = 0; index < (int)shaderDesc.BoundResources; ++index)
	{
		D3D11_SHADER_INPUT_BIND_DESC sibd;
		reflection->GetResourceBindingDesc(index, &sibd);
		if (sibd.Type == D3D_SHADER_INPUT_TYPE::D3D_SIT_UAV_RWSTRUCTURED
			&& mapShaderRWStructBuffer.find(sibd.Name) == mapShaderRWStructBuffer.end())
		{
			shared_ptr<ShaderRWStructBuffer> ssrwb = shared_ptr<ShaderRWStructBuffer>(new ShaderRWStructBuffer());
			ssrwb->bNeedUpdated = false;
			ssrwb->name = sibd.Name;
			ssrwb->registerIndex = sibd.BindPoint;
			ssrwb->size = sibd.NumSamples;
			ssrwb->structNum = 0;
			ssrwb->buffer = nullptr;
			ssrwb->data = nullptr;
			ssrwb->srv = nullptr;
			ssrwb->uav = nullptr;
			mapShaderRWStructBuffer[ssrwb->name] = ssrwb;
		}
	}

	return true;
}

bool Shader::SetMatrixArrayElement(const string& variableName, const CXMMATRIX& matrix, int index)
{
	if (mapShaderVariable.find(variableName) != mapShaderVariable.end())
	{
		shared_ptr<ShaderVariable> shaderVariable = mapShaderVariable[variableName];
		if (ShaderVariableType::SHADER_MATRIX == shaderVariable->variableType && 0 == shaderVariable->size % sizeof(CXMMATRIX))
		{
			XMMATRIX memMatrix = XMMatrixTranspose(matrix);

			//暂时不支持记忆之前的值,等以后实现了3D数学库支持“==”在搞
			memcpy((unsigned char*)shaderVariable->variableCurrent + index * sizeof(CXMMATRIX), (void*)&memMatrix, sizeof(CXMMATRIX));
		}
	}

	return true;
}

bool Shader::SetFloat3ArrayElement(const string& variableName, XMFLOAT3 value, int index)
{
	if (mapShaderVariable.find(variableName) != mapShaderVariable.end())
	{
		shared_ptr<ShaderVariable> shaderVariable = mapShaderVariable[variableName];
		memcpy((unsigned char*)shaderVariable->variableCurrent + index * sizeof(XMFLOAT4), (void*)&value, sizeof(XMFLOAT3));
	}

	return true;
}


void Shader::SetRWStructBufferInData(const string& bufferName, void* data, int num)
{
	if (mapShaderRWStructBuffer.find(bufferName) != mapShaderRWStructBuffer.end())
	{
		shared_ptr<ShaderRWStructBuffer> ssb = mapShaderRWStructBuffer[bufferName];
		ssb->data = data;
		ssb->structNum = num;
	}
}

ID3D11ShaderResourceView* Shader::GetSrvOfUav(const string& bufferName)
{
	if (mapShaderRWStructBuffer.find(bufferName) != mapShaderRWStructBuffer.end())
	{
		shared_ptr<ShaderRWStructBuffer> ssrwb = mapShaderRWStructBuffer[bufferName];
		return ssrwb->srv;
	}

	return nullptr;
}

ID3D11UnorderedAccessView* Shader::GetUav(const string& bufferName)
{
	if (mapShaderRWStructBuffer.find(bufferName) != mapShaderRWStructBuffer.end())
	{
		shared_ptr<ShaderRWStructBuffer> ssrwb = mapShaderRWStructBuffer[bufferName];
		return ssrwb->uav;
	}

	return nullptr;
}

bool Shader::SetRWStructBuffer(const string& bufferName, ID3D11UnorderedAccessView* uav)
{
	if (mapShaderRWStructBuffer.find(bufferName) != mapShaderRWStructBuffer.end())
	{
		shared_ptr<ShaderRWStructBuffer> ssb = mapShaderRWStructBuffer[bufferName];
		ssb->uav = uav;
	}

	return true;
}

//----------------------------------------------------------------
//-------------------------VertexPixelShader----------------------
//----------------------------------------------------------------
VertexPixelShader::VertexPixelShader(const string& shaderFile):
	Shader(),
	vertexShader(nullptr),
	pixelShader(nullptr),
	inputLayout(nullptr)
{
	Init(shaderFile);
}

VertexPixelShader::VertexPixelShader(const VertexPixelShader& other)
{
}

VertexPixelShader::~VertexPixelShader()
{
	printf("------ VertexPixelShader 释放？？");
	ShutDown();
}

/// <summary>
/// 这个方法。。。。。。。这么搞笑的。。。。。
/// </summary>
/// <param name="shaderFile"></param>
/// <returns></returns>
bool VertexPixelShader::Init(const string& shaderFile)
{

	bool result;
	result = InitShader(shaderFile);
	if (!result)
		return false;

	return true;
}

bool VertexPixelShader::InitShader(const string& shaderFile)
{
	HRESULT result;
	ID3D10Blob* pErrorMessage;
	ID3D10Blob* pVertexShaderBlob;
	ID3D10Blob* pPixelShaderBlob;

	pErrorMessage = nullptr;
	pVertexShaderBlob = nullptr;
	pPixelShaderBlob = nullptr;
	DWORD flag = D3DCOMPILE_ENABLE_STRICTNESS;
	wstring wShaderFile = Str2Wstr(shaderFile);

#if _DEBUG
	flag |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	//compile vertexShaer
	//result = D3DCompileFromFile(wShaderFile.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VS", "vs_5_0", flag, 0, &pVertexShaderBlob, &pErrorMessage);
	result = D3DX11CompileFromFile(wShaderFile.c_str(), nullptr, NULL, "VS", "vs_5_0", flag, 0, NULL,&pVertexShaderBlob, &pErrorMessage,NULL);
	
	if (FAILED(result))
	{
		if (pErrorMessage)
		{
			//Log::LogShaderCompileInfo(pErrorMessage, shaderFile);
			printf("Log::LogShaderCompileInfo(pErrorMessage, shaderFile)");
		}
		else
		{
			//Log::Error("can not find VS file {0} ", shaderFile);
			printf("can not find VS file {0} ", shaderFile);
		}

		return false;
	}

	HR(g_Device->CreateVertexShader(pVertexShaderBlob->GetBufferPointer(),
		pVertexShaderBlob->GetBufferSize(), nullptr, &vertexShader));

	//compile pixelShaer
	//result = D3DCompileFromFile(wShaderFile.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PS", "ps_5_0", flag, 0,
	//	&pPixelShaderBlob, &pErrorMessage);
	result = D3DX11CompileFromFile(wShaderFile.c_str(), nullptr, NULL, "PS", "ps_5_0", flag, 0,NULL,
		&pPixelShaderBlob, &pErrorMessage,NULL);

	if (FAILED(result))
	{
		if (pErrorMessage)
		{
			//Log::LogShaderCompileInfo(pErrorMessage, shaderFile);
			printf("Log::LogShaderCompileInfo(pErrorMessage, shaderFile)");
		}
		else
		{
			//Log::Error("can not find PS file {0} ", shaderFile);
			printf("can not find PS file {0} ", shaderFile);
		}

		return false;
	}

	HR(g_Device->CreatePixelShader(pPixelShaderBlob->GetBufferPointer(), pPixelShaderBlob->GetBufferSize(), nullptr, &pixelShader));

	ID3D11ShaderReflection* vsShaderReflection = nullptr;
	HR(D3DReflect(pVertexShaderBlob->GetBufferPointer(), pVertexShaderBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&vsShaderReflection));
	ID3D11ShaderReflection* psShaderReflection = nullptr;
	HR(D3DReflect(pPixelShaderBlob->GetBufferPointer(), pPixelShaderBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&psShaderReflection));

	if (nullptr == vsShaderReflection || nullptr == psShaderReflection)
		return false;

	ReflectInputLayout(vsShaderReflection, pVertexShaderBlob);
	ReflectShaderConstantBuffer(vsShaderReflection);
	ReflectShaderConstantBuffer(psShaderReflection);
	ReflectShaderTexture(vsShaderReflection);
	ReflectShaderTexture(psShaderReflection);
	ReflectShaderSampler(psShaderReflection);
	ReflectShaderRWStructBuffer(psShaderReflection);
	CreateConstantBuffer();

	//释放Blob
	ReleaseCOM(pVertexShaderBlob);
	ReleaseCOM(pPixelShaderBlob);
	return true;
}
void VertexPixelShader::ApplyDirectXOnly(ID3D11VertexShader* md3dVertexShader, ID3D11PixelShader* md3dPixelShader, ID3D11InputLayout* md3dInputLayout)
{
	//SetShaderParam();
	//g_DeviceContext->IASetInputLayout(md3dInputLayout);
	//g_DeviceContext->VSSetShader(md3dVertexShader, NULL, 0);
	//g_DeviceContext->PSSetShader(md3dPixelShader, NULL, 0);
	g_DeviceContext->IASetInputLayout(inputLayout);
	g_DeviceContext->VSSetShader(vertexShader, NULL, 0);
	g_DeviceContext->PSSetShader(pixelShader, NULL, 0);
}
void VertexPixelShader::ApplyDirectXOnly() {
	SetShaderParam();
}
void VertexPixelShader::Apply()
{
	UpdateConstantBuffer();
	//UpdateRWStructBuffer();
	SetShaderParam();
}

void VertexPixelShader::SetShaderParam()
{
	g_DeviceContext->IASetInputLayout(inputLayout);
	g_DeviceContext->VSSetShader(vertexShader, NULL, 0);
	g_DeviceContext->PSSetShader(pixelShader, NULL, 0);
}

bool VertexPixelShader::ReflectInputLayout(ID3D11ShaderReflection* vertexShaderReflection, ID3D10Blob* vertexShaderBlob)
{
	vector<D3D11_INPUT_ELEMENT_DESC> vecInputElementDesc;
	D3D11_SHADER_DESC vertexShaderDesc;
	HR(vertexShaderReflection->GetDesc(&vertexShaderDesc));

	unsigned int alignedByteOffset = 0;
	mReflectLayoutSize = 0;
	for (int inputParamIndex = 0; inputParamIndex < (int)vertexShaderDesc.InputParameters; ++inputParamIndex)
	{
		D3D11_SIGNATURE_PARAMETER_DESC signatureParamDesc;
		vertexShaderReflection->GetInputParameterDesc(inputParamIndex, &signatureParamDesc);

		D3D11_INPUT_ELEMENT_DESC inputElementDesc;
		inputElementDesc.SemanticName = signatureParamDesc.SemanticName;
		inputElementDesc.SemanticIndex = signatureParamDesc.SemanticIndex;
		inputElementDesc.InputSlot = 0;
		inputElementDesc.AlignedByteOffset = alignedByteOffset;
		inputElementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		inputElementDesc.InstanceDataStepRate = 0;

		if (1 == signatureParamDesc.Mask)
		{
			if (D3D_REGISTER_COMPONENT_UINT32 == signatureParamDesc.ComponentType)
			{
				inputElementDesc.Format = DXGI_FORMAT_R32_UINT;
			}
			else if (D3D_REGISTER_COMPONENT_SINT32 == signatureParamDesc.ComponentType)
			{
				inputElementDesc.Format = DXGI_FORMAT_R32_SINT;
			}
			else if (D3D_REGISTER_COMPONENT_FLOAT32 == signatureParamDesc.ComponentType)
			{
				inputElementDesc.Format = DXGI_FORMAT_R32_FLOAT;
			}

			alignedByteOffset += 4;
		}
		else if (signatureParamDesc.Mask <= 3)
		{
			if (D3D_REGISTER_COMPONENT_UINT32 == signatureParamDesc.ComponentType)
			{
				inputElementDesc.Format = DXGI_FORMAT_R32G32_UINT;
			}
			else if (D3D_REGISTER_COMPONENT_SINT32 == signatureParamDesc.ComponentType)
			{
				inputElementDesc.Format = DXGI_FORMAT_R32G32_SINT;

			}
			else if (D3D_REGISTER_COMPONENT_FLOAT32 == signatureParamDesc.ComponentType)
			{
				inputElementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
			}

			alignedByteOffset += 8;
		}
		else if (signatureParamDesc.Mask <= 7)
		{
			if (D3D_REGISTER_COMPONENT_UINT32 == signatureParamDesc.ComponentType)
			{
				inputElementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
			}
			else if (D3D_REGISTER_COMPONENT_SINT32 == signatureParamDesc.ComponentType)
			{
				inputElementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
			}
			else if (D3D_REGISTER_COMPONENT_FLOAT32 == signatureParamDesc.ComponentType)
			{
				inputElementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
			}

			alignedByteOffset += 12;
		}
		else if (signatureParamDesc.Mask <= 15)
		{
			if (D3D_REGISTER_COMPONENT_UINT32 == signatureParamDesc.ComponentType)
			{
				//inputElementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
				inputElementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
			}
			else if (D3D_REGISTER_COMPONENT_SINT32 == signatureParamDesc.ComponentType)
			{
				//inputElementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
				inputElementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
			}
			else if (D3D_REGISTER_COMPONENT_FLOAT32 == signatureParamDesc.ComponentType)
			{
				//inputElementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
				inputElementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
			}


			alignedByteOffset += 12;
			////mReflectLayoutSize += 12;
			//mReflectLayoutSize -=4;//减去4位
			////shader文件定义是flot4 ，而渲染管线一直推的是XMFLOAT3
			////这个之后再看看怎么处理
			////所以这里每个变量先减去4位
		}

		vecInputElementDesc.push_back(inputElementDesc);
	}
	mReflectLayoutSize += alignedByteOffset;
	mRefInputLayout = vecInputElementDesc;
	if (0 == vecInputElementDesc.size())
		return false;

	unsigned int numElements = sizeof(vecInputElementDesc) / sizeof(vecInputElementDesc[0]);         //布局数量
	HR(g_Device->CreateInputLayout(&vecInputElementDesc[0], (UINT)vecInputElementDesc.size(), vertexShaderBlob->GetBufferPointer(),
		(UINT)vertexShaderBlob->GetBufferSize(), &inputLayout));

	//mReflectLayoutSize = (UINT)vecInputElementDesc.size();
	return true;
}

XMMATRIX _worldMatrix;
XMMATRIX _viewMatrix;
XMMATRIX _ProjMatrix;
void VertexPixelShader::RenderTradition(XMMATRIX world, XMMATRIX view, XMMATRIX proj)
{
	for (auto cbIterator = mapShaderContantBuffer.begin(); cbIterator != mapShaderContantBuffer.end(); ++cbIterator)
	{
		shared_ptr<ShaderConstantBuffer> shaderConstantBuffer = cbIterator->second;
		if (nullptr == shaderConstantBuffer)
			continue;
		_worldMatrix = world;
		_viewMatrix = view;
		_ProjMatrix = proj;
		SetShaderParameterTradition(shaderConstantBuffer->registerIndex, shaderConstantBuffer->constantBuffer);
	}
}
bool VertexPixelShader::SetShaderParameterTradition(unsigned int index,ID3D11Buffer* matrixBuffer)
{
	//D3D11_MAPPED_SUBRESOURCE mappedResource;
	//CBMatrix* cbPtr;
	//unsigned int bufferNum;

	//将矩阵转置,在传入常量缓存前进行转置,因为GPU对矩阵数据会自动进行一次转置
	Shader::CBMatrix cb;


	//XMMATRIX worldMa = XMMatrixTranspose(_worldMatrix);
	//XMMATRIX viewMa = XMMatrixTranspose(_viewMatrix);
	//XMMATRIX ProjMa = XMMatrixTranspose(_ProjMatrix);
	//cb.mWorldMatrix = worldMa;
	//cb.mViewMatrix = viewMa;
	//cb.mProjMatrix = ProjMa;
	cb.mWorldMatrix = _worldMatrix;
	cb.mViewMatrix = _viewMatrix;
	cb.mProjMatrix = _ProjMatrix;
	g_DeviceContext->UpdateSubresource(matrixBuffer, 0, NULL, &cb, 0, 0);

	//设置在顶点缓存中常量缓存的位置,注册号
	//bufferNum = 0;
	printf("VSSetConstantBuffers \n");
	//设置在VertexShader的常量缓存的值(带着更新的值)
	g_DeviceContext->VSSetConstantBuffers(index, 1, &matrixBuffer);

	return true;
}
bool VertexPixelShader::UpdateConstantBuffer()
{
	for (auto cbIterator = mapShaderContantBuffer.begin(); cbIterator != mapShaderContantBuffer.end(); ++cbIterator)
	{
		shared_ptr<ShaderConstantBuffer> shaderConstantBuffer = cbIterator->second;
		if (nullptr == shaderConstantBuffer)
			continue;

		////采用旧的方法
		//for (auto shaderVarIterator = shaderConstantBuffer->vecShaderVariableName.begin();
		//	shaderVarIterator != shaderConstantBuffer->vecShaderVariableName.end(); ++shaderVarIterator)
		//{
		//	if (mapShaderVariable.end() != mapShaderVariable.find(*shaderVarIterator))
		//	{
		//		shared_ptr<ShaderVariable> shaderVariable = mapShaderVariable[*shaderVarIterator];
		//		if (nullptr != shaderVariable)
		//		{
		//			if (shaderVariable->name == "World") {
		//				memcpy(&_worldMatrix + shaderVariable->startOffset ,shaderVariable->variableCurrent,shaderVariable->size);
		//			}
		//			else if (shaderVariable->name == "View") {
		//				memcpy(&_viewMatrix + shaderVariable->startOffset, shaderVariable->variableCurrent, shaderVariable->size);
		//			}
		//			else if (shaderVariable->name == "Proj")
		//			{
		//				memcpy(&_ProjMatrix + shaderVariable->startOffset, shaderVariable->variableCurrent, shaderVariable->size);
		//			}
		//			/*memcpy((unsigned char*)mappedSubresource.pData + shaderVariable->startOffset,
		//				shaderVariable->variableCurrent, shaderVariable->size);*/
		//		}
		//		
		//	}
		//}
		////采用旧的方法
		//SetShaderParameterTradition(shaderConstantBuffer->registerIndex,shaderConstantBuffer->constantBuffer);
		

		// 采用新
		if (!shaderConstantBuffer->bNeedUpdated)
		{
			//VS PS
			g_DeviceContext->VSSetConstantBuffers(shaderConstantBuffer->registerIndex, 1, &(shaderConstantBuffer->constantBuffer));
			g_DeviceContext->PSSetConstantBuffers(shaderConstantBuffer->registerIndex, 1, &(shaderConstantBuffer->constantBuffer));
		}

		//Map：获取的数据中包含一个子资源的指针，并拒绝该子资源的访问GPU。
		D3D11_MAPPED_SUBRESOURCE mappedSubresource;
		HR(g_DeviceContext->Map(shaderConstantBuffer->constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource));

		for (auto shaderVarIterator = shaderConstantBuffer->vecShaderVariableName.begin();
			shaderVarIterator != shaderConstantBuffer->vecShaderVariableName.end(); ++shaderVarIterator)
		{
			if (mapShaderVariable.end() != mapShaderVariable.find(*shaderVarIterator))
			{
				shared_ptr<ShaderVariable> shaderVariable = mapShaderVariable[*shaderVarIterator];
				if (nullptr != shaderVariable)
				{
					memcpy((unsigned char*)mappedSubresource.pData + shaderVariable->startOffset,
						shaderVariable->variableCurrent, shaderVariable->size);
				}
			}
		}

		g_DeviceContext->Unmap(shaderConstantBuffer->constantBuffer, 0);
		g_DeviceContext->VSSetConstantBuffers(shaderConstantBuffer->registerIndex, 1, &(shaderConstantBuffer->constantBuffer));
		g_DeviceContext->PSSetConstantBuffers(shaderConstantBuffer->registerIndex, 1, &(shaderConstantBuffer->constantBuffer));
	}

	return true;
}

bool VertexPixelShader::UpdateRWStructBuffer()
{
	struct UavParam
	{
		int registerIndex;
		ID3D11UnorderedAccessView* uav;
	};

	struct CompareClass {
		bool operator() (UavParam a, UavParam b) { return (a.registerIndex < b.registerIndex); }
	} compareObject;


	vector<UavParam> uavParamsArray;

	for (auto cbIterator = mapShaderRWStructBuffer.begin(); cbIterator != mapShaderRWStructBuffer.end(); ++cbIterator)
	{
		shared_ptr<ShaderRWStructBuffer> ssrwb = cbIterator->second;
		if (nullptr == ssrwb->buffer && nullptr == ssrwb->uav && nullptr == ssrwb->srv)
		{
			if (ssrwb->data != nullptr && ssrwb->size != 0
				&& ssrwb->structNum != 0)
			{
				//create struct buffer
				D3D11_BUFFER_DESC desc;
				ZeroMemory(&desc, sizeof(desc));
				desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
				desc.ByteWidth = ssrwb->size * ssrwb->structNum;
				desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
				desc.StructureByteStride = ssrwb->size;
				D3D11_SUBRESOURCE_DATA InitData;
				InitData.pSysMem = ssrwb->data;
				HR(g_Device->CreateBuffer(&desc, &InitData, &ssrwb->buffer));

				//create srv
				D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
				srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
				srvDesc.BufferEx.FirstElement = 0;
				srvDesc.Format = DXGI_FORMAT_UNKNOWN;
				srvDesc.BufferEx.NumElements = desc.ByteWidth / desc.StructureByteStride;
				HR(g_Device->CreateShaderResourceView(ssrwb->buffer, &srvDesc, &ssrwb->srv));

				//create rw struct buffer
				D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
				ZeroMemory(&uavDesc, sizeof(uavDesc));
				uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
				uavDesc.Buffer.FirstElement = 0;
				uavDesc.Format = DXGI_FORMAT_UNKNOWN;
				uavDesc.Buffer.NumElements = desc.ByteWidth / desc.StructureByteStride;
				g_Device->CreateUnorderedAccessView(ssrwb->buffer, &uavDesc, &ssrwb->uav);
			}
		}

		uavParamsArray.push_back({ ssrwb->registerIndex, ssrwb->uav });
	}

	if (uavParamsArray.size() > 0)
	{
		sort(uavParamsArray.begin(), uavParamsArray.end(), compareObject);

		vector<ID3D11UnorderedAccessView*> uavArray;
		for (auto& It : uavParamsArray)
		{
			uavArray.push_back(It.uav);
		}

		g_DeviceContext->OMSetRenderTargetsAndUnorderedAccessViews(D3D11_KEEP_RENDER_TARGETS_AND_DEPTH_STENCIL, nullptr, nullptr, uavParamsArray[0].registerIndex, (UINT)uavArray.size(), uavArray.data(), nullptr);
	}
	
	return true;
}

bool VertexPixelShader::SetTexture(const string& variableName, ID3D11ShaderResourceView* texture)
{
	if (mapShaderTexture.end() != mapShaderTexture.find(variableName) && nullptr != texture)
	{
		shared_ptr<ShaderTexture> shaderTexture = mapShaderTexture[variableName];
		g_DeviceContext->PSSetShaderResources(shaderTexture->bindPoint, 1, &texture);
	}
	else
	{
		return false;
	}
	return true;
}

bool VertexPixelShader::SetTextureSampler(const string& variableName, ID3D11SamplerState* sampler)
{
	if (mapShaderSampler.end() != mapShaderSampler.find(variableName) && nullptr != sampler)
	{
		shared_ptr<ShaderSampler> shaderSampler = mapShaderSampler[variableName];
		g_DeviceContext->PSSetSamplers(shaderSampler->bindPoint, 1, &sampler);
	}
	else
	{
		return false;
	}
	return true;
}


//----------------------------------------------------------------
//-------------------------VertexPixelShader----------------------
//----------------------------------------------------------------
ComputeShader::ComputeShader(const string& shaderFile)
{
	InitShader(shaderFile);
}

ComputeShader::ComputeShader(const ComputeShader& other)
{

}

ComputeShader::~ComputeShader()
{

}

void ComputeShader::SetShaderParam()
{
	g_DeviceContext->CSSetShader(computeShader, nullptr, 0);
}

void ComputeShader::Apply()
{
	UpdateConstantBuffer();
	UpdateSutrctBuffer();
	UpdateRWStructBuffer();
	SetShaderParam();;
}

bool ComputeShader::InitShader(const string& shaderFile)
{
	HRESULT result;
	ID3D10Blob* errorMessage = nullptr;
	ID3D10Blob* csBlob = nullptr;
	DWORD flag = D3DCOMPILE_ENABLE_STRICTNESS;
	wstring wShaderFile = Str2Wstr(shaderFile);

#if _DEBUG
	flag |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	//result = D3DCompileFromFile(wShaderFile.c_str(), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "CS", "cs_5_0", flag, 0, &csBlob, &errorMessage);
	result = D3DX11CompileFromFile(wShaderFile.c_str(), NULL, NULL, "CS", "cs_5_0", flag, 0,NULL,
		&csBlob, &errorMessage,NULL);
	if (FAILED(result))
	{
		if (errorMessage)
		{
			//Log::LogShaderCompileInfo(errorMessage, shaderFile);
			printf("(SystemClass::Get()->Graphics()->D3D()->GetDevice()");
		}
		else
		{
			//Log::Error("can not find CS file {0} ", shaderFile);
			printf("can not find CS file {0} ", shaderFile);
		}

		return false;
	}

	g_Device->CreateComputeShader(csBlob->GetBufferPointer(), csBlob->GetBufferSize(),
		NULL, &computeShader);

	ID3D11ShaderReflection* csShaderReflection = nullptr;
	HR(D3DReflect(csBlob->GetBufferPointer(), csBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&csShaderReflection));

	if (nullptr == csShaderReflection)
		return false;

	ReflectShaderConstantBuffer(csShaderReflection);
	ReflectShaderTexture(csShaderReflection);
	ReflectShaderSampler(csShaderReflection);
	ReflectShaderStructBuffer(csShaderReflection);
	ReflectShaderUAVTexture(csShaderReflection);
	ReflectShaderRWStructBuffer(csShaderReflection);
	CreateConstantBuffer();

	ReleaseCOM(csBlob);
	ReleaseCOM(errorMessage);
	ReleaseCOM(csShaderReflection);
	return true;
}

bool ComputeShader::UpdateConstantBuffer()
{
	for (auto cbIterator = mapShaderContantBuffer.begin(); cbIterator != mapShaderContantBuffer.end(); ++cbIterator)
	{
		shared_ptr<ShaderConstantBuffer> shaderConstantBuffer = cbIterator->second;
		if (nullptr == shaderConstantBuffer)
			continue;

		if (!shaderConstantBuffer->bNeedUpdated)
		{
			//CS
			g_DeviceContext->CSSetConstantBuffers(shaderConstantBuffer->registerIndex, 1, &(shaderConstantBuffer->constantBuffer));
		}

		D3D11_MAPPED_SUBRESOURCE mappedSubresource;
		HR(g_DeviceContext->Map(shaderConstantBuffer->constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource));

		for (auto shaderVarIterator = shaderConstantBuffer->vecShaderVariableName.begin();
			shaderVarIterator != shaderConstantBuffer->vecShaderVariableName.end(); ++shaderVarIterator)
		{
			if (mapShaderVariable.end() != mapShaderVariable.find(*shaderVarIterator))
			{
				shared_ptr<ShaderVariable> shaderVariable = mapShaderVariable[*shaderVarIterator];
				if (nullptr != shaderVariable)
				{
					memcpy((unsigned char*)mappedSubresource.pData + shaderVariable->startOffset,
						shaderVariable->variableCurrent, shaderVariable->size);
				}
			}
		}
		g_DeviceContext->Unmap(shaderConstantBuffer->constantBuffer, 0);
		g_DeviceContext->CSSetConstantBuffers(shaderConstantBuffer->registerIndex, 1, &(shaderConstantBuffer->constantBuffer));
	}

	return true;
}

bool ComputeShader::UpdateRWStructBuffer()
{
	for (auto cbIterator = mapShaderRWStructBuffer.begin(); cbIterator != mapShaderRWStructBuffer.end(); ++cbIterator)
	{
		shared_ptr<ShaderRWStructBuffer> ssrwb = cbIterator->second;
		if (nullptr == ssrwb->uav)
		{
			if (ssrwb->size != 0
				&& ssrwb->structNum != 0)
			{
				//create struct buffer
				D3D11_BUFFER_DESC desc;
				ZeroMemory(&desc, sizeof(desc));
				desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
				desc.ByteWidth = ssrwb->size * ssrwb->structNum;
				desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
				desc.StructureByteStride = ssrwb->size;
				HR(g_Device->CreateBuffer(&desc, nullptr, &ssrwb->buffer));

				//create srv
				D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
				srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
				srvDesc.BufferEx.FirstElement = 0;
				srvDesc.Format = DXGI_FORMAT_UNKNOWN;
				srvDesc.BufferEx.NumElements = desc.ByteWidth / desc.StructureByteStride;
				HR(g_Device->CreateShaderResourceView(ssrwb->buffer, &srvDesc, &ssrwb->srv));

				//create rw struct buffer
				D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
				ZeroMemory(&uavDesc, sizeof(uavDesc));
				uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
				uavDesc.Buffer.FirstElement = 0;
				uavDesc.Format = DXGI_FORMAT_UNKNOWN;
				uavDesc.Buffer.NumElements = desc.ByteWidth / desc.StructureByteStride;
				g_Device->CreateUnorderedAccessView(ssrwb->buffer, &uavDesc, &ssrwb->uav);
			}
		}

		g_DeviceContext->CSSetUnorderedAccessViews(ssrwb->registerIndex, 1, &ssrwb->uav, nullptr);
	}

	return true;
}

bool ComputeShader::SetTexture(const string& variableName, ID3D11ShaderResourceView* texture)
{
	if (mapShaderTexture.end() != mapShaderTexture.find(variableName) && nullptr != texture)
	{
		shared_ptr<ShaderTexture> shaderTexture = mapShaderTexture[variableName];
		g_DeviceContext->CSSetShaderResources(shaderTexture->bindPoint, 1, &texture);
	}
	else
	{
		return false;
	}

	return true;
}

bool ComputeShader::SetRWTexture(const string& variableName, ID3D11UnorderedAccessView* texture)
{
	if (mapRWShaderTexture.end() != mapRWShaderTexture.find(variableName) && nullptr != texture)
	{
		shared_ptr<ShaderTexture> shaderTexture = mapRWShaderTexture[variableName];
		g_DeviceContext->CSSetUnorderedAccessViews(shaderTexture->bindPoint, 1, &texture, nullptr);
	}
	else
	{
		return false;
	}

	return true;
}

bool ComputeShader::SetTextureSampler(const string& variableName, ID3D11SamplerState* sampler)
{
	if (mapShaderSampler.end() != mapShaderSampler.find(variableName) && nullptr != sampler)
	{
		shared_ptr<ShaderSampler> shaderSampler = mapShaderSampler[variableName];
		g_DeviceContext->CSSetSamplers(shaderSampler->bindPoint, 1, &sampler);
	}
	else
	{
		return false;
	}

	return true;
}

bool ComputeShader::ReflectShaderStructBuffer(ID3D11ShaderReflection* reflection)
{
	D3D11_SHADER_DESC shaderDesc;
	HR(reflection->GetDesc(&shaderDesc));

	for (int index = 0; index < (int)shaderDesc.BoundResources; ++index)
	{
		D3D11_SHADER_INPUT_BIND_DESC sibd;
		reflection->GetResourceBindingDesc(index, &sibd);
		if (sibd.Type == D3D_SHADER_INPUT_TYPE::D3D_SIT_STRUCTURED
			&& mapShaderStructBuffer.find(sibd.Name) == mapShaderStructBuffer.end())
		{
			shared_ptr<ShaderStructBuffer> ssb = shared_ptr<ShaderStructBuffer>(new ShaderStructBuffer());
			ssb->bNeedUpdated = false;
			ssb->name = sibd.Name;
			ssb->registerIndex = sibd.BindPoint;
			ssb->size = sibd.NumSamples;
			ssb->structNum = 0;
			ssb->buffer = nullptr;
			ssb->data = nullptr;
			ssb->srv = nullptr;
			mapShaderStructBuffer[ssb->name] = ssb;
		}
	}

	return true;
}

bool ComputeShader::ReflectShaderUAVTexture(ID3D11ShaderReflection* reflection)
{
	D3D11_SHADER_DESC shaderDesc;
	HR(reflection->GetDesc(&shaderDesc));

	for (int brIndex = 0; brIndex < (int)shaderDesc.BoundResources; ++brIndex)
	{
		D3D11_SHADER_INPUT_BIND_DESC shaderIbDesc;
		reflection->GetResourceBindingDesc(brIndex, &shaderIbDesc);
		if (D3D_SIT_UAV_RWTYPED == shaderIbDesc.Type)
		{
			shared_ptr<ShaderTexture> shaderTexture = shared_ptr<ShaderTexture>(new ShaderTexture());
			shaderTexture->name = shaderIbDesc.Name;
			shaderTexture->bindPoint = shaderIbDesc.BindPoint;
			if (mapRWShaderTexture.end() == mapRWShaderTexture.find(shaderIbDesc.Name))
			{
				mapRWShaderTexture[shaderIbDesc.Name] = shaderTexture;
			}
		}
	}

	return true;
}

void ComputeShader::SetStructBuffer(const string& bufferName, void* data, int num)
{
	if (mapShaderStructBuffer.find(bufferName) != mapShaderStructBuffer.end())
	{
		shared_ptr<ShaderStructBuffer> ssb = mapShaderStructBuffer[bufferName];
		ssb->data = data;
		ssb->structNum = num;
	}
}

bool ComputeShader::UpdateSutrctBuffer()
{
	for (auto cbIterator = mapShaderStructBuffer.begin(); cbIterator != mapShaderStructBuffer.end(); ++cbIterator)
	{
		shared_ptr<ShaderStructBuffer> ssb = cbIterator->second;
		if (nullptr == ssb->buffer)
		{
			if (ssb->data != nullptr && ssb->size != 0
				&& ssb->structNum != 0)
			{
				D3D11_BUFFER_DESC desc;
				ZeroMemory(&desc, sizeof(desc));
				desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
				desc.ByteWidth = ssb->size * ssb->structNum;
				desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
				desc.StructureByteStride = ssb->size;
				D3D11_SUBRESOURCE_DATA InitData;
				InitData.pSysMem = ssb->data;
				HR(g_Device->CreateBuffer(&desc, &InitData, &ssb->buffer));

				D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
				srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
				srvDesc.BufferEx.FirstElement = 0;
				srvDesc.Format = DXGI_FORMAT_UNKNOWN;
				srvDesc.BufferEx.NumElements = desc.ByteWidth / desc.StructureByteStride;
				HR(g_Device->CreateShaderResourceView(ssb->buffer, &srvDesc, &ssb->srv));
			}
		}

		g_DeviceContext->CSSetShaderResources(ssb->registerIndex, 1, &ssb->srv);
	}

	return true;
}

ID3D11Buffer* ComputeShader::GetBufferOfUav(const string& variableName)
{
	if (mapShaderRWStructBuffer.find(variableName) != mapShaderRWStructBuffer.end())
	{
		shared_ptr<ShaderRWStructBuffer> ssrwb = mapShaderRWStructBuffer[variableName];
		return ssrwb->buffer;
	}

	return nullptr;
}

void ComputeShader::Dispatch(UINT ThreadGroupCountX, UINT ThreadGroupCountY, UINT ThreadGroupCountZ)
{
	Apply();
	g_DeviceContext->Dispatch(ThreadGroupCountX, ThreadGroupCountY, ThreadGroupCountZ);
	ID3D11ShaderResourceView* nullSRV[1] = { nullptr };
	ID3D11UnorderedAccessView* nullUAV[1] = { nullptr };
	g_DeviceContext->CSSetShaderResources(0, 1, nullSRV);
	g_DeviceContext->CSSetUnorderedAccessViews(0, 1, nullUAV, nullptr);
}