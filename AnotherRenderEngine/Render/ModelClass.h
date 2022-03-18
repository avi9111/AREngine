#pragma once
#ifndef _MODEL_CLASS_H
#define _MODEL_CLASS_H

#include<d3d11.h>
#include<xnamath.h>
#define HR1(X) {if(FAILED(X)) { MessageBox(0,L"Create Failed",0,0); return false;}}
#define ReleaseCOM1(x) { if (x) { x->Release(); x = 0; } }

class ModelClass
{
private:
	struct Vertex
	{
		XMFLOAT3 pos;
		XMFLOAT4 color;
	};
private:
	ID3D11Buffer* md3dVertexBuffer; //���㻺��
	ID3D11Buffer* md3dIndexBuffer;  //��������
	int mVertexCount;
	int mIndexCount;

private:
	bool InitializeBuffer(ID3D11Device* d3dDevice);
	void ShutdownBuffer();
	void RenderBuffers(ID3D11DeviceContext* d3dDeviceContext);
	float* ToMatrix(Vertex vec);

public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

public:
	//Initialize�Ǵ���Ԫ��,Render������Ԫ��,Shutdown��Release
	bool Initialize(ID3D11Device* d3dDevice);
	void Shutdown();
	void Render(ID3D11DeviceContext* d3dDeviceContext);



	int GetIndexCount() { return mIndexCount; }

};
#endif 