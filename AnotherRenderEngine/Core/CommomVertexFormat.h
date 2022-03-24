//#pragma once
#ifndef _COMMOM_VERTEX_FORMAT_H
#define _COMMOM_VERTEX_FORMAT_H

//#include<d3d11_1.h>//һֱ��d11_1 ����
#include<Windows.h>
//#include<DirectXMath.h>//����ע�⣬�ж�����Ҫ�ã�����Ҫ�ٴ����룬�Ѿ�����
#include<string>
#include<vector>
#include<map>
//#include <minwindef.h>
#include <D3D11.h>
#include <xnamath.h>
using namespace std;
//using namespace DirectX;


struct VertexPCNTT
{
	XMFLOAT3 pos;
	XMFLOAT2 uv;
	XMFLOAT3 normal;
	//XMFLOAT3 tangent;
	//XMFLOAT3 color;
};

struct Triangle
{
	VertexPCNTT vertexs[3];
	int MaterialId;
};


struct MaterialTexFileName
{
	string diffuseMapFileName;
	string specularMapFileName;
	string alphaMapFileName;
	string bumpMapFileName;
	string roughnessMapFileName;
	string metalMapFileName;
};


struct MeshData
{
	vector<VertexPCNTT> mVertexData;
	vector<WORD> mIndexData;
	int materialId;
	ID3D11Buffer* mVertexBuffer;
	ID3D11Buffer* mIndexBuffer;
};


//��ʱ��FBXģ������
struct MemFBXModelData
{
	vector<Triangle> mTriangleData;
	map<int, MaterialTexFileName> mMaterialMap;
};

//FBXһ���ڵ���������MeshData��Ҳ����һ��Model
struct ModelData
{
	vector<MeshData> mMeshList;
	map<int, MaterialTexFileName> mMaterialMap;
};


//��֤��һ��FBX���ص����������ļ�����������һ��
//һ��FBX�ļ����Խ�����n��ModelData
struct FBXModelData
{
	vector<ModelData> mModelList;
};

//todo: delete the code, it will be TransformComponent
struct Transform
{
	XMFLOAT3 localPosition = XMFLOAT3(1.0f,1.0f,1.0f);
	XMFLOAT3 localScale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	XMFLOAT3 localRotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
};


#endif // !_COMMOM_VERTEX_FORMAT_H