#pragma once
#ifndef D3D_CLASS_H
#define D3D_CLASS_H
#define HR(X) {if(FAILED(X)) { MessageBox(0,L"Create Failed",0,0); return false;}}
#define ReleaseCOM(x) { if (x) { x->Release(); x = 0; } }

#include<Windows.h>
#include<d3d11.h>
#include<D3Dcommon.h>  //???
#include<D3DX11.h>
#include<xnamath.h>
#include<iostream>
#include<istream>
#include<ostream>
#include<strstream>
#include<fstream>
using namespace std;

class D3DClass
{
private:
	bool mVsyncEnable;  //�Ƿ���֡��Ⱦ
	int mVideoCardMemory; //�Կ��ڴ�
	char mVideoCardDescription[128]; //�Կ�����
    
private:
	
	ID3D11Device* md3dDevice;//D3D11�豸
	ID3D11DeviceContext* md3dImmediateContext;//D3D11�豸������
	IDXGISwapChain* md3dSwapChain;//D3D������
	ID3D11RenderTargetView* md3dRenderTargetView; //D3D11��ȾĿ����ͼ
	ID3D11DepthStencilView* md3dDepthStencilView; //D3D11���(ģ��)��ͼ
	ID3D11Texture2D* md3dDepthStencilBuffer; //D3D11�ġ�DepthStencil���桱
	ID3D11DepthStencilState* md3dDepthStencilState; //���(ģ��)����״̬
	ID3D11RasterizerState* md3dRasterizerState; //D3D�Ĺ�դ��״̬

private:
	XMMATRIX mWorldMatrix;  //����任����
	XMMATRIX mOrthoMatrix;; //��������??
	XMMATRIX mProjMatrix;  //ͶӰ����


public:
	//����,��������,��������
	D3DClass();
	D3DClass(const D3DClass&);
	~D3DClass();

	//D3DClass��ʼ������
	bool Initialize(int ScreenWidth, int ScreenHeight, bool vsync, HWND hwnd, bool fullscreen, float ScreenDepth, float ScreenNear);

	//�ر�D3DClass����
	void Shutdown();

	//���Ƴ�������
	void BeginScene(float red, float green, float blue, float alpha);
	void EndScene();

	//Get����
	ID3D11Device* GetDevice() { return md3dDevice; }
	ID3D11DeviceContext* GetDeviceContext(){ return md3dImmediateContext; }
	XMMATRIX GetWorldMatrix() { return mWorldMatrix; }
	XMMATRIX GetOrthoMatrix() { return mOrthoMatrix; }
	XMMATRIX GetProjMatrix() { return mProjMatrix; };

	void GetVideoCardInfo(char*, int&); //��ȡ�Կ���Ϣ
};
#endif // !D3D_CLASS_H
