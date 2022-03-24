#pragma once
#ifndef _MODEL_CLASS_H
#define _MODEL_CLASS_H

#include<d3d11.h>
#include<xnamath.h>
#include <vector>
#define HR1(X) {if(FAILED(X)) { MessageBox(0,L"Create Failed",0,0); return false;}}
#define ReleaseCOM1(x) { if (x) { x->Release(); x = 0; } }

class ModelClass
{
private:
	struct Vertex
	{
		XMFLOAT3 pos;
		//XMFLOAT4 color;
		XMFLOAT2 uv;
	};
	XMFLOAT3 mPosition;

private:
	ID3D11Buffer* md3dVertexBuffer; //顶点缓存
	ID3D11Buffer* md3dIndexBuffer;  //索引缓存
	int mVertexCount;
	int mIndexCount;

private:
	bool InitializeBuffer(ID3D11Device* d3dDevice);
	void ShutdownBuffer();
	void RenderBuffers(ID3D11DeviceContext* d3dDeviceContext);
	float* ToMatrix(Vertex vec);
	float* ToMatrixOffset(Vertex vec, XMFLOAT3 pos);
	ID3D11Device* device;
public:
	/// <summary>
	/// 保持上一个顶点位置，做vertex偏移用（应该可以用指针处理）
	/// </summary>
	Vertex* mVertexs;
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

public:
	void SetPosition(XMFLOAT3 v);
	XMFLOAT3 GetPosition();
	//Initialize是创建元素,Render是设置元素,Shutdown是Release
	bool Initialize(ID3D11Device* d3dDevice);
	void Shutdown();
	void Render(ID3D11DeviceContext* d3dDeviceContext);



	int GetIndexCount() { return mIndexCount; }

};
#endif 
