﻿#pragma once
#ifndef _GRAPHICS_CLASS_H
#define _GRAPHICS_CLASS_H

#include"D3DClass.h"
#include"CameraClass.h"
#include"ColorShaderClass.h"
#include"ModelClass.h"
#include "../Core/ShaderLib/Shader.h"
#include "../Core/Light.h"


//全局变量
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLE = true;  //是尽可能快渲染还是限制帧渲染
const float SCREEN_FAR = 1000.0f;  //视截体远裁面
const float SCREEN_NEAR = 0.1f;  //视截体近裁面



class GraphicsClass
{

private:
	//D3D类
	D3DClass* mD3D;


	//用于控制VertexShader PixelShader,InputLayout
	ColorShaderClass* mColorShader;
	
	//用于控制VertexBuffer和IndexBuffer
	ModelClass* mModel;

private:
	bool Render();
	// 常量缓冲
	ID3D11Buffer* mBoxCB;
	ID3D11Buffer* mFrameBoxCB;

	// 顶点着色器的常量缓存
	struct ConstantBuffer
	{
		XMMATRIX gWorld;
		XMMATRIX gWorldInvTranspose;	// World逆转置矩阵，用于将法线从模型坐标系中转换到世界坐标系
		XMMATRIX gWorldViewProj;	 // wvp
	};

	// 片元着色器的常量缓存
	struct FrameConstantBuffer
	{
		DirectionalLight gDirLight;	// 直射光
		PointLight gPointLight;	// 点光源
		SpotLight gSpotLight;	// 聚光灯
		Material gMaterial;	// 物体材质
		XMFLOAT4 gEyePosW;	// 当前点到相机向量
	};

	// 常量数据结构
	ConstantBuffer mCBuffer;
	FrameConstantBuffer mFrameCBuffer;


	DirectionalLight mDirLight;
public:
	//相机类，用于控制场景的相机
	CameraClass* mCamera;

	//使用 shaderLib 创建一个测试 shader
	shared_ptr<VertexPixelShader> mShader2;
	D3DClass* D3D();
	ModelClass* Model();//临时的绘制model
	ColorShaderClass* Shader();
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();
	
	ID3D11ShaderResourceView* LoadTexture(const WCHAR*);
public:
	bool Initialize(int ScreenWidth, int ScreenHeight, HWND hwnd);
	void Shutdown();
	bool Frame(); 
};

#endif // !_GRAPHICS_CLASS_H
