//#pragma once
#ifndef _COMMOM_VERTEX_FORMAT_H
#define _COMMOM_VERTEX_FORMAT_H

//#include<d3d11_1.h>//一直，d11_1 慎用
#include<Windows.h>
//#include<DirectXMath.h>//！！注意，有毒，不要用，不需要再次引入，已经引入
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


//临时的FBX模型数据
struct MemFBXModelData
{
	vector<Triangle> mTriangleData;
	map<int, MaterialTexFileName> mMaterialMap;
};

//FBX一个节点解析出多个MeshData，也就是一个Model
struct ModelData
{
	vector<MeshData> mMeshList;
	map<int, MaterialTexFileName> mMaterialMap;
};


//保证了一个FBX加载的所有纹理文件都仅仅加载一次
//一个FBX文件可以解析出n个ModelData
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