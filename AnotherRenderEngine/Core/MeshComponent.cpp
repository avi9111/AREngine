#include "MeshComponent.h"
#include "ImportFbx2.h"
#include "../Render/D3DClass.h"
#include "../Render/SystemClass.h"
//#include "../Render/SystemClass.h"
//#include "../Render/SystemClass.cpp"
//#include "SDEngine/Shader/Material.h"//����һ��û�õ�����
MeshComponent::MeshComponent(string strFbxFileName) :
	pureColor(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)),
	roughness(1.0f),
	metal(1.0f),
	bTransparent(false),
	bReflect(false),
	bCastShadow(true)
{
	SetMesh(strFbxFileName);
}


MeshComponent::~MeshComponent()
{

}

bool MeshComponent::LoadFBXModel(string strFbxFileName)
{
	if (GImportFBX->m_mapFBXModel.find(strFbxFileName) == GImportFBX->m_mapFBXModel.end())
	{
		m_pFBXModel = shared_ptr<FBXModelData>(new FBXModelData());
		GImportFBX->ImportFbxFile(strFbxFileName, m_pFBXModel->mModelList);
		GImportFBX->m_mapFBXModel[strFbxFileName] = m_pFBXModel;
	}
	else
	{
		m_pFBXModel = GImportFBX->m_mapFBXModel[strFbxFileName];
	}

	if (nullptr != m_pFBXModel)
	{

	}
	return true;
}

void MeshComponent::SetMesh(const string& meshFile)
{
	this->meshFileName = meshFile;
	LoadFBXModel(meshFile);
	InitBuffer();//������ʱȥ����InitBuffer,ע����Ҫ�ֶ�����
}

void MeshComponent::ShutDown()
{
}
D3D11_BUFFER_DESC toBufferDesc(UINT structLen,UINT size)
{
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	//vertexBufferDesc.ByteWidth = sizeof(mMesh.mVertexData[0]) * (UINT)mMesh.mVertexData.size();
	vertexBufferDesc.ByteWidth = structLen * size;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;
	return vertexBufferDesc;
}
void MeshComponent::SetFirstMesh() {
	//�õ�ָ�룿��
	vector<ModelData>& mModelList = m_pFBXModel->mModelList;
	//vector<ModelData> mModelList = m_pFBXModel->mModelList;

	ModelData* mModelData = &mModelList[0];
	MeshData &mesh = mModelData->mMeshList[0];
	//����һ���ķ���ƽ��
	mesh.mVertexData.clear();
	VertexPCNTT vertex =  VertexPCNTT();
	vertex.pos = XMFLOAT3(-1.0f, -1.0f, 0.0f);
	vertex.uv = XMFLOAT2(0, 0.9);
	mesh.mVertexData.push_back(vertex);
	VertexPCNTT vertex2 = VertexPCNTT();
	vertex2.pos = XMFLOAT3(1.0f, 1.0f, 0.0f);
	vertex2.uv = XMFLOAT2(0.9, 0);
	mesh.mVertexData.push_back(vertex2);
	VertexPCNTT vertex3 = VertexPCNTT();
	vertex3.pos = XMFLOAT3(1.0f, -1.0f, 0.0f);
	vertex3.uv = XMFLOAT2(0.9, 0.9);
	mesh.mVertexData.push_back(vertex3);
	VertexPCNTT vertex4 = VertexPCNTT();
	vertex4.pos = XMFLOAT3(-1.0f, 1.0f, 0.0f);
	vertex4.uv = XMFLOAT2(0, 0);
	mesh.mVertexData.push_back(vertex4);

	mesh.mIndexData.clear();
	mesh.mIndexData.push_back(0);
	mesh.mIndexData.push_back(3);
	mesh.mIndexData.push_back(2);
	mesh.mIndexData.push_back(1);
	mesh.mIndexData.push_back(2);
	mesh.mIndexData.push_back(3);

	InitBuffer();
}
MeshData& MeshComponent::GetFirstMesh()
{
	/*if (nullptr == m_pFBXModel)
		return ;*/

	vector<ModelData>& mModelList = m_pFBXModel->mModelList;
	ModelData* mModelData = &mModelList[0];
	MeshData& mesh = mModelData->mMeshList[0];
	return mesh;
}
//public 
void MeshComponent::ResetBuffer(vector<D3D11_INPUT_ELEMENT_DESC> shareInputs) {
	MeshData& mesh = GetFirstMesh();
	if (&mesh == nullptr) return;
	////////// ����һ�� ////////////
	//TDOO:����shader ��̬������ƥ���Bufferָ��
	VertexPCNTT* newPtr = new VertexPCNTT();
	//auto vectex = &mesh.mVertexData[0];
	////newPtr->pos.x = vectex.pos.x;
	////newPtr->pos.y = vectex.pos.y;
	////newPtr->pos.z = vectex.pos.z;
	//newPtr->pos = vectex->pos;
	////newPtr->uv = mesh.mVertexData[0].uv;//��ôЩֻ�Ǳ�����ˡ�������ʵ���ַ�ҵ�
	//newPtr->uv = vectex->uv;
	////newPtr->uv = & mes
	/*///////// ָ�븴��ԭ�� //////////////
	int *a,*b��//��������ָ�����

a=b; //ָ�븴�ƣ������ʹaָ��ĵ�ַ��bָ��ĵ�ַ��ͬ�����Ƕ�ָ��ͬһ�ڴ�����

*a=*b ; //ָ�븳ֵ�������ʹaָ��ĵ�ַ���ڴ�λ�ã���������bָ���ַ���ڴ�λ�ã���������ͬ��
		//��a��bָ��ĵ�ַ��һ����ͬ

&a=&b ; //���һ��Ƚ����ã�ʹָ��a�����������ָ��b�����������ͬ

	*/
	//���ԡ����ơ�ָ�루a=b)
	newPtr = &mesh.mVertexData[0];
	//////////
	D3D11_BUFFER_DESC desc = toBufferDesc(sizeof(mesh.mVertexData[0]) ,(UINT)mesh.mVertexData.size());
	D3D11_SUBRESOURCE_DATA data;
	//data.pSysMem = &mesh.mVertexData[0];
	data.pSysMem = newPtr;
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;
	g_Device->CreateBuffer(&desc, &data, &mesh.mVertexBuffer); //������ظ�����ô��TODO:��ȷ����û���ԭbuffr
}
void MeshComponent::InitBuffer()
{
	if (nullptr == m_pFBXModel)
		return;

	vector<ModelData>& mModelList = m_pFBXModel->mModelList;

	for (UINT index = 0; index < mModelList.size(); ++index)
	{
		ModelData* mModelData = &mModelList[index];

		for (int i = 0; i < (int)mModelData->mMeshList.size(); ++i)
		{
			MeshData& mMesh = mModelData->mMeshList[i];

			D3D11_BUFFER_DESC vertexBufferDesc;
			vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			vertexBufferDesc.ByteWidth = sizeof(mMesh.mVertexData[0]) * (UINT)mMesh.mVertexData.size();
			vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			vertexBufferDesc.CPUAccessFlags = 0;
			vertexBufferDesc.MiscFlags = 0;
			vertexBufferDesc.StructureByteStride = 0;

			D3D11_SUBRESOURCE_DATA vertexData;
			vertexData.pSysMem = &mMesh.mVertexData[0];//TODO:Ҫ���¹���һ��ָ�루ʹ��shader reflect)
			vertexData.SysMemPitch = 0;
			vertexData.SysMemSlicePitch = 0;
			g_Device->CreateBuffer(&vertexBufferDesc, &vertexData, &mMesh.mVertexBuffer);

			D3D11_BUFFER_DESC  indexBufferDesc;
			indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			indexBufferDesc.ByteWidth = sizeof(WORD) * (UINT)mMesh.mIndexData.size();
			indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			indexBufferDesc.CPUAccessFlags = 0;
			indexBufferDesc.MiscFlags = 0;
			indexBufferDesc.StructureByteStride = 0;

			D3D11_SUBRESOURCE_DATA indexData;
			indexData.pSysMem = &mMesh.mIndexData[0];
			indexData.SysMemPitch = 0;
			indexData.SysMemSlicePitch = 0;
			g_Device->CreateBuffer(&indexBufferDesc, &indexData, &mMesh.mIndexBuffer);
		}

	}

}

void MeshComponent::ShutDownBuffer()
{
	vector<ModelData>& mModelList = m_pFBXModel->mModelList;
	for (UINT index = 0; index < mModelList.size(); ++index)
	{
		ModelData* mModelData = &mModelList[index];
		for (int i = 0; i < (int)mModelData->mMeshList.size(); ++i)
		{
			MeshData& mMesh = mModelData->mMeshList[i];
			ReleaseCOM(mMesh.mVertexBuffer);
			ReleaseCOM(mMesh.mIndexBuffer);
		}
	}
}

void MeshComponent::SetAlbedoTexture(string fileName)
{
	SetTexture(fileName, AlbedoMap);
}

void MeshComponent::SetNormalTexture(string fileName)
{
	SetTexture(fileName, NormalMap);
}

void MeshComponent::SetSpecularTexture(string fileName)
{
	SetTexture(fileName, SpecularMap);
}

void MeshComponent::SetRoughnessTexture(string fileName)
{
	SetTexture(fileName, RoughnessMap);
}

void MeshComponent::SetMetalTexture(string fileName)
{
	SetTexture(fileName, MetalMap);
}

void MeshComponent::SetTexture(string fileName, TextureType textureType)
{
	if (nullptr == m_pFBXModel)
		return;

	vector<ModelData>& modelList = m_pFBXModel->mModelList;
	for (UINT index = 0; index < modelList.size(); ++index)
	{
		ModelData* modelData = &modelList[index];

		if (nullptr != modelData)
		{
			for (auto& it : modelData->mMaterialMap)
			{
				switch (textureType)
				{

				case AlbedoMap:
					it.second.diffuseMapFileName = fileName;
					break;
				case NormalMap:
					it.second.bumpMapFileName = fileName;
					break;
				case SpecularMap:
					it.second.specularMapFileName = fileName;
					break;
				case RoughnessMap:
					it.second.roughnessMapFileName = fileName;
					break;
				case MetalMap:
					it.second.metalMapFileName = fileName;
					break;
				default:
					break;
				}

				it.second.diffuseMapFileName = fileName;
			}
		}
	}
}

void MeshComponent::SetMaterial(shared_ptr<Material> material)
{
	this->material = material;
}

shared_ptr<Material> MeshComponent::GetMaterial()
{
	return this->material;
}

shared_ptr<Material> MeshComponent::GetMaterial() const
{
	return this->material;
}