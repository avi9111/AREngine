#pragma once
#ifndef _COLOR_SHADER_CLASS_H
#define _COLOR_SHADER_CLASS_H
#define HR2(X) {if(FAILED(X)) { MessageBox(0,L"Create Failed",0,0); return false;}}

#include<d3d11.h>
#include<xnamath.h>
#include<D3DX11.h> //含编译Shader程序的函数
#include<d3dcompiler.h>
#include<fstream>
using namespace std;

class ColorShaderClass
{

private:
	//常量缓存结构体
	struct CBMatrix
	{
		XMMATRIX mWorldMatrix;
		XMMATRIX mViewMatrix;
		XMMATRIX mProjMatrix;
	};

private:

	ID3D11Buffer* mCBMatrixBuffer; //(常量)缓存,顶点索引也是用这个类型

private:
	//bool InitShaderWithException(ID3D11Device*, HWND, WCHAR const*, WCHAR const*);
	bool InitializeShader(ID3D11Device*, HWND, WCHAR const *, WCHAR const *); //用于创建InputLayout,VertexShader,PixelShader,常量缓存
	bool ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR const *);

	bool SetShaderParameter(ID3D11DeviceContext*, CXMMATRIX, CXMMATRIX, CXMMATRIX);
	void RenderShader(ID3D11DeviceContext*, int);

public:
	ID3D11VertexShader* md3dVertexShader;
	ID3D11PixelShader* md3dPixelShader;
	ID3D11InputLayout* md3dInputLayout; //这与VertexShader相关联,因此要放在ColorShaderClass里,而不是D3DClass

	bool SetShaderParameterOutside(ID3D11DeviceContext*, CXMMATRIX, CXMMATRIX, CXMMATRIX);
	int ShaderIndex;
	ColorShaderClass();
	ColorShaderClass(const ColorShaderClass&);
	~ColorShaderClass();

public:
	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, CXMMATRIX, CXMMATRIX, CXMMATRIX);

};
#endif 
