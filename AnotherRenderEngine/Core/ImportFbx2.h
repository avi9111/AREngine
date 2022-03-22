#ifndef _IMPORT_FBX2_H
#define _IMPORT_FBX2_H
//#pragma once
//#include<d3d11_1.h>//无端端，这里会引入一个d3d11_1
#include<fbxsdk.h>

using namespace std;
#include<vector>//这几个include安全
#include<map>
#include<string>
#include<memory>
#include<d3d11.h>
#include<xnamath.h>//必须先调用 #include<d3d11.h>
#include "CommomVertexFormat.h";
class ImportFbx2
{
private:
	vector<MemFBXModelData> mMemFBXModelData;
	FbxScene* mScene;

public:
	map<string, shared_ptr<FBXModelData>> m_mapFBXModel;

private:
	FbxManager* mFbxManager = nullptr;
	string fbxFileNamePre;
private:
	//读取一个场景的mesh类型数据
	void ReadSceneMeshData(FbxScene* scene);
	void ReadMeshNodeData(FbxNode* node);
	void ProcessMesh(FbxMesh* mesh, MemFBXModelData* fbxModel);
	void ReadMeshMaterialIndex(FbxMesh* mesh, vector<Triangle>& triangleData);
	void ChangeModelData(ModelData* destModel, MemFBXModelData* srcMemFBXModel);

	//三角化mesh
	void TriangulateRecursive(FbxNode* node);

private:
	//回收分配的数据
	void ClearMemFBXModelData();

private:
	//读取顶点的常见几何属性
	void ReadVertexPos(FbxMesh* mesh, int ctrlPointIndex, XMFLOAT3* pos);
	void ReadVertexColor(FbxMesh* mesh, int ctrlPointIndex, int vertexCount, XMFLOAT3* color);
	void ReadVertexNormal(FbxMesh* mesh, int ctrlPointIndex, int vertexCount, XMFLOAT3* normal);
	void ReadVertexTangent(FbxMesh* mesh, int ctrlPointIndex, int vertexCount, XMFLOAT3* normal);
	void ReadVertexUV(FbxMesh* mesh, int ctrlPointIndex, int uvIndex, XMFLOAT2* uv);

private:
	//读取材质属性
	//void LoadMaterial(FbxMesh* mesh, map<int, MaterialTexFileName>& materialMap);
	//void LoadMaterialTexture(FbxSurfaceMaterial* surfaceMaterial, int materialIndex, map<int, MaterialTexFileName>& materialMap);
	//void ReadReletiveTextureFileName(FbxProperty* property, int materialIndex, map<int, MaterialTexFileName>& materialMap);

private:

	void CalculateTriangleTangent(Triangle& triangle);
public:
	static shared_ptr<ImportFbx2> m_pImportFBX;
public:
	ImportFbx2();
	~ImportFbx2();

public:
	static ImportFbx2* Get();
	void ClearMem();
	void ImportFbxFile(string fbxFileName, vector<ModelData>& mFBXModel);
	void ImportFbxFileTest(string fbxFileName);
};

#define GImportFBX (ImportFbx2::Get())
#endif 