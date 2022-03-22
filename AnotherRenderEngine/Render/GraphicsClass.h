#pragma once
#ifndef _GRAPHICS_CLASS_H
#define _GRAPHICS_CLASS_H

#include"D3DClass.h"
#include"CameraClass.h"
#include"ColorShaderClass.h"
#include"ModelClass.h"


//ȫ�ֱ���
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLE = true;  //�Ǿ����ܿ���Ⱦ��������֡��Ⱦ
const float SCREEN_FAR = 1000.0f;  //�ӽ���Զ����
const float SCREEN_NEAR = 0.1f;  //�ӽ��������



class GraphicsClass
{

private:
	//D3D��
	D3DClass* mD3D;

	//����࣬���ڿ��Ƴ��������
	CameraClass* mCamera; 

	//���ڿ���VertexShader PixelShader,InputLayout
	ColorShaderClass* mColorShader;

	//���ڿ���VertexBuffer��IndexBuffer
	ModelClass* mModel;

private:
	bool Render();

public:
	D3DClass* D3D();
	ModelClass* Model();//��ʱ�Ļ���model
	ColorShaderClass* Shader();
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

public:
	bool Initialize(int ScreenWidth, int ScreenHeight, HWND hwnd);
	void Shutdown();
	bool Frame(); 
};

#endif // !_GRAPHICS_CLASS_H
