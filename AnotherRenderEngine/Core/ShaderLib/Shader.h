#ifndef _SHADER_H
#define _SHADER_H
#pragma once


#include <D3D11.h>
#include <xnamath.h>
#include <vector>
#include <D3D11Shader.h>
#include <map>
#include <algorithm>//sort()需要

//#include "CoreMinimal.h"
#include "../CommonFunction.h"
#include<D3DX11.h> //含编译Shader程序的函数
#include<d3dcompiler.h>
enum ShaderVariableType
{
	SHADER_MATRIX,
	SHADER_FLOAT,
	SHADER_FLOAT2,
	SHADER_FLOAT3,
	SHADER_FLOAT4
};

struct ShaderVariable
{
	int startOffset;
	int size;
	string name;
	string constantBufferName;
	ShaderVariableType variableType;
	//当variablePre与variableCurrent指向的值一致时，将不进行常量缓存更新
	void* variablePre; 	//variablePre 指向之前变量的值
	void* variableCurrent; 	//variableCurrent 指向目前变量的值
};

struct ShaderTexture
{
	string name;
	int bindPoint;
};

struct ShaderSampler
{
	string name;
	int bindPoint;
};

struct ShaderConstantBuffer
{
	int size;
	int registerIndex;
	string name;
	ID3D11Buffer* constantBuffer;
	vector<string> vecShaderVariableName;
	void* data;
	bool bNeedUpdated;
};

struct ShaderStructBuffer
{
	int size;
	int registerIndex;
	string name;
	void* data;
	bool bNeedUpdated;
	int structNum;
	ID3D11Buffer* buffer;
	ID3D11ShaderResourceView* srv;
};

struct ShaderRWStructBuffer
{
	int size;
	int registerIndex;
	string name;
	void* data;
	bool bNeedUpdated;
	int structNum;
	ID3D11Buffer* buffer;
	ID3D11ShaderResourceView* srv;
	ID3D11UnorderedAccessView* uav;
};

//TODO:部分代码重复,待重构
class Shader
{

public:
	Shader();
	virtual ~Shader();

protected:
	virtual void ShutDown();
	struct CBMatrix
	{
		XMMATRIX mWorldMatrix;
		XMMATRIX mViewMatrix;
		XMMATRIX mProjMatrix;
	};
public:
	vector<D3D11_INPUT_ELEMENT_DESC> mRefInputLayout;
	UINT mReflectLayoutSize;//TODO:容易溢出
	virtual void Apply() = 0;
	bool SetMatrixRaw(const string& variableName, const XMMATRIX& matrix);
	bool SetMatrix(const string& variableName, const CXMMATRIX& matrix);
	bool SetMatrixArrayElement(const string& variableName, const CXMMATRIX& matrix, int index);

	//float
	bool SetFloat(const string& variableName, float value);
	bool SetFloat2(const string& variableName, XMFLOAT2 value);
	bool SetFloat3(const string& variableName, XMFLOAT3 value);
	bool SetFloat3ArrayElement(const string& variableName, XMFLOAT3 value, int index);
	bool SetFloat4(const string& variableName, XMFLOAT4 value);

	//uint
	bool SetUint4(const string& variableName, UINT8 value[4]);

	void SetRWStructBufferInData(const string& bufferName, void* data, int num);
	bool SetRWStructBuffer(const string& bufferName, ID3D11UnorderedAccessView* uav);
	virtual bool SetTexture(const string& variableName, ID3D11ShaderResourceView* texture) = 0;
	virtual bool SetTextureSampler(const string& variableName, ID3D11SamplerState* sampler) = 0;
	ID3D11ShaderResourceView* GetSrvOfUav(const string& bufferName);
	ID3D11UnorderedAccessView* GetUav(const string& bufferName);

protected:
	void OutputShaderErrorMessage(ID3D10Blob* , HWND, WCHAR*);
	bool ReflectShaderConstantBuffer(ID3D11ShaderReflection* reflection);
	bool ReflectShaderTexture(ID3D11ShaderReflection* shaderReflection);
	bool ReflectShaderSampler(ID3D11ShaderReflection* shaderReflection);
	bool ReflectShaderRWStructBuffer(ID3D11ShaderReflection* reflection);
	bool CreateConstantBuffer();

	// TODO: refactor UpdateConstantBuffer and UpdateRWStructBuffer, too much reappearing code
	virtual bool UpdateConstantBuffer() = 0;
	virtual bool UpdateRWStructBuffer() = 0;
	virtual void SetShaderParam() = 0;

protected:
	map<string, shared_ptr<ShaderConstantBuffer>> mapShaderContantBuffer;
	map<string, shared_ptr<ShaderVariable>> mapShaderVariable;
	map<string, shared_ptr<ShaderTexture>> mapShaderTexture;
	map<string, shared_ptr<ShaderSampler>> mapShaderSampler;

	//RWStructBuffer exit in PixelShader And ComputeShader
	map<string, shared_ptr<ShaderRWStructBuffer>> mapShaderRWStructBuffer;
};

class VertexPixelShader : public Shader
{

public:
	VertexPixelShader(const string& shaderFile);
	VertexPixelShader(const VertexPixelShader& other);
	~VertexPixelShader();

public:
	void ApplyDirectXOnly(ID3D11VertexShader* md3dVertexShader,ID3D11PixelShader* md3dPixelShader,ID3D11InputLayout* md3dInputLayout );
	void ApplyDirectXOnly();
	void RenderTradition( XMMATRIX world, XMMATRIX view, XMMATRIX proj);
	virtual void Apply() override;
	virtual bool SetTexture(const string& variableName, ID3D11ShaderResourceView* texture) override;
	virtual bool SetTextureSampler(const string& variableName, ID3D11SamplerState* sampler) override;
	bool Init(const string& shaderFile);
private:
	
	bool InitShader(const string& shaderFile);
	virtual void SetShaderParam() override;
	bool ReflectInputLayout(ID3D11ShaderReflection* vertexShaderReflection, ID3D10Blob* vertexShaderBlob);

	/// <summary>
	/// 测试方法，无什么卵用。。。实现不了
	/// </summary>
	/// <param name="index"></param>
	/// <param name="d3dDeviceContext"></param>
	/// <returns></returns>
	bool SetShaderParameterTradition(unsigned int index,ID3D11Buffer* matrixBuffer);
	virtual bool UpdateConstantBuffer() override;
	virtual bool UpdateRWStructBuffer() override;

private:
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	ID3D11InputLayout* inputLayout;

public:
	friend class Material;
};

class ComputeShader : public Shader
{
public:
	ComputeShader(const string& shaderFile);
	ComputeShader(const ComputeShader& other);
	~ComputeShader();

public:
	virtual bool SetTexture(const string& variableName, ID3D11ShaderResourceView* texture) override;
	bool SetRWTexture(const string& variableName, ID3D11UnorderedAccessView* texture);
	virtual bool SetTextureSampler(const string& variableName, ID3D11SamplerState* sampler) override;
	void SetStructBuffer(const string& bufferName, void* data, int num);
	void Dispatch(UINT ThreadGroupCountX, UINT ThreadGroupCountY, UINT ThreadGroupCountZ);
	ID3D11Buffer* GetBufferOfUav(const string& variableName);

private:
	bool ReflectShaderStructBuffer(ID3D11ShaderReflection* reflection);
	bool ReflectShaderUAVTexture(ID3D11ShaderReflection* reflection);
	virtual bool UpdateConstantBuffer() override;
	virtual bool UpdateRWStructBuffer() override;
	bool UpdateSutrctBuffer();
	void SetShaderParam();
	bool InitShader(const string& shaderFile);
	virtual void Apply() override;

private:
	ID3D11ComputeShader* computeShader;

private:
	map<string, shared_ptr<ShaderTexture>> mapRWShaderTexture;
	map<string, shared_ptr<ShaderStructBuffer>> mapShaderStructBuffer;
};

#endif