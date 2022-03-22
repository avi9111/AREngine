#include"ModelClass.h"
#include "../Maths/matrix.h"
//���ԣ��ú�д�����ο�
//https://www.cnblogs.com/kenter/archive/2011/09/08/2170516.html


//#include "../Core/ImportFbx2.h"
ModelClass::ModelClass()
{
    md3dVertexBuffer=NULL; //���㻺��
    md3dIndexBuffer=NULL;  //��������
	mVertexCount = 0;
	mIndexCount = 0;

}


ModelClass::ModelClass(const ModelClass& other)
{

}

ModelClass::~ModelClass()
{

}

void ModelClass::SetPosition(XMFLOAT3 v)
{
	mPosition = v;
	
	mVertexs[0].pos = XMFLOAT3(-1.0f, -1.0f, 0.0f);
	mVertexs[1].pos = XMFLOAT3(1.0f, 1.0f, 0.0f);
	mVertexs[2].pos = XMFLOAT3(1.0f, -1.0f, 0.0f);
	mVertexs[3].pos = XMFLOAT3(-1.0f, 1.0f, 0.0f);
	//TODO:�ı䶥��λ��
	for (int i = 0; i < 4; i++)
	{
	
		float* f = ToMatrixOffset(mVertexs[i], mPosition);
		mVertexs[i].pos.x = f[0];
		mVertexs[i].pos.y = f[1];
		mVertexs[i].pos.z = f[2];
	}

	
	Initialize(device);
	
}
XMFLOAT3 ModelClass :: GetPosition() {
	return mPosition;
}

bool ModelClass::Initialize(ID3D11Device* d3dDevice)
{
	device = d3dDevice;
	bool result;
	result = InitializeBuffer(d3dDevice);

	if (!result)
		return false;

		return true;
}

void ModelClass::Shutdown()
{
	ShutdownBuffer();
}


void ModelClass::Render(ID3D11DeviceContext* d3dDeviceContext)
{
	//������Ⱦ���ߵĶ��㻺�����������(IA�׶�)
	RenderBuffers(d3dDeviceContext);
}
/// <summary>
/// ���Է������̶�����-3����
/// </summary>
/// <param name="vec"></param>
/// <returns></returns>
float* ModelClass::ToMatrix(Vertex vec) {
	float pos[4 * 1] = { vec.pos.x,vec.pos.y,vec.pos.z,1 };
	float const data[4 * 4] =
	{
	  1, 0, 0, -3,
	  0, 1, 0, 0,
	  0, 0, 1, 0,
	  0, 0, 0, 1
	};
	Matrix< float > matrix(4, 4);
	matrix = data;

	Matrix<float> matrixCurr(4, 1);
	matrixCurr = pos;
	
	Matrix<float> ret = matrix * matrixCurr;
	float retData[4 * 1] = { ret.get(0,0),ret.get(1,0),ret.get(2,0) };
	return retData;
}
float* ModelClass::ToMatrixOffset(Vertex vec, XMFLOAT3 offsetPosition)
{
	float pos[4 * 1] = { vec.pos.x,vec.pos.y,vec.pos.z,1 };
	float const data[4 * 4] =
	{
	  1, 0, 0, offsetPosition.x,
	  0, 1, 0, offsetPosition.y,
	  0, 0, 1, offsetPosition.z,
	  0, 0, 0, 1
	};
	Matrix< float > matrix(4, 4);
	matrix = data;

	Matrix<float> matrixCurr(4, 1);
	matrixCurr = pos;

	Matrix<float> ret = matrix * matrixCurr;
	float retData[4 * 1] = { ret.get(0,0),ret.get(1,0),ret.get(2,0) };
	return retData;
}
/// <summary>
/// directX���㻺�������ο���
/// https://www.cnblogs.com/X-Jun/p/10359345.html
/// </summary>
/// <param name="d3dDevice"></param>
/// <returns></returns>
bool ModelClass::InitializeBuffer(ID3D11Device* d3dDevice)
{
	Vertex* vertexs=NULL;
	WORD*indices=NULL;  //һ����Ϊ�����ֽ� 

	mVertexCount = 4;
	mIndexCount = 6;

	//������������
	vertexs = new Vertex[mVertexCount];
	if (!vertexs)
		return false;

	//������������
	indices = new WORD[mIndexCount];
	if (!indices)
		return false;
	if (mVertexs != NULL)
	{
		vertexs[0].pos = mVertexs[0].pos;
		vertexs[1].pos = mVertexs[1].pos;
		vertexs[2].pos = mVertexs[2].pos;
		vertexs[3].pos = mVertexs[3].pos;
	}
	else
	{
		//���趥���������ݺ�������������
		vertexs[0].pos = XMFLOAT3(-1.0f, -1.0f, 0.0f);

		vertexs[1].pos = XMFLOAT3(1.0f, 1.0f, 0.0f);

		vertexs[2].pos = XMFLOAT3(1.0f, -1.0f, 0.0f);

		vertexs[3].pos = XMFLOAT3(-1.0f, 1.0f, 0.0f);
		mVertexs = new Vertex[mVertexCount];
		
	}
	
	//vertexs[0].color = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	//vertexs[1].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	//vertexs[2].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	//vertexs[3].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	vertexs[0].uv = XMFLOAT2(0, 0.9);
	vertexs[1].uv = XMFLOAT2(0.9, 0);
	vertexs[2].uv = XMFLOAT2(0.9, 0.9);
	vertexs[3].uv = XMFLOAT2(0, 0);
	/* int const data[ 4 * 4 ] =
     {
       1, 0, 0, 0,
       0, 1, 0, 0,
	   0, 0, 1, 0,
	   0, 0, 0, 1
     };
    Matrix< int > matrix( 4, 4 );
    matrix = data;*/

	////�ƶ�������2��λ�ƣ�
	//for (int i = 0; i < 4; i++)
	//{
	//	float* f = ToMatrix(vertexs[i]);
	//	vertexs[i].pos.x = f[0];
	//	vertexs[i].pos.y = f[1];
	//	vertexs[i].pos.z = f[2];
	//}


	//����������������

	//ע�������ֶ����ж��ǲ��Ǳ���
	indices[0] = 0;
	indices[1] = 3;
	indices[2] = 2;
	indices[3] = 1;
	indices[4] = 2;
	indices[5] = 3;


	//��һ,���(����)�������ݽṹ�������Դ���ݽṹ��,���������㻺��
	D3D11_BUFFER_DESC vertexBufferDesc;
	//vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * mVertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	//vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = vertexs;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;
	HR1(d3dDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &md3dVertexBuffer));

	//�ڶ�,���(����)�������ݽṹ�������Դ���ݽṹ��,��������������
	D3D11_BUFFER_DESC  indexBufferDesc;
	//indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	indexBufferDesc.ByteWidth = sizeof(WORD) * mIndexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	//indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;
    HR1(d3dDevice->CreateBuffer(&indexBufferDesc, &indexData, &md3dIndexBuffer));

	//�ͷŶ����������������(��ʱ�����Ѿ����뻺��,����Ҫ��Щ������)
	//delete[]vertexs;
	//vertexs = NULL;
	//delete[]indices;
	//indices = NULL;
	//		�ƶ�������2��λ�ƣ�
	for (int i = 0; i < 4; i++)
	{
		float* f = ToMatrix(vertexs[i]);
		vertexs[i].pos.x = f[0];
		vertexs[i].pos.y = f[1];
		vertexs[i].pos.z = f[2];
	}
//	vertexData.pSysMem = vertexs;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	//float2 vertices[] =
	//{
	//	float2(-1.0f, -1.0f);
	//	float2(0.0f, 1.0f);
	//	float2(1.0f, -1.0f);
	//};

	//1.
	//// ����GPU����
	////auto m_d3dContext = m_deviceResources->GetD3DDeviceContext();
	////auto m_d3dContent = d3dDevice->GetImmediateContext();
	////m_d3dContext->Map(vertexBuffer2.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	//// ��������
	//memccpy(mappedResource.pData, vertexs, sizeof(vertexs));
	//// ����GPU ���ݷ���Ȩ��
	//m_d3dContext->Unmap(vertexBuffer2.Get(), 0);

	//2.д���ͷ������ο���
	//https://forum.unity.com/threads/how-to-use-id3d11buffer-compute-buffer-native-pointer-in-c.495609/
	ID3D11DeviceContext* pImmediateContext = nullptr;
	d3dDevice->GetImmediateContext(&pImmediateContext);
	//pImmediateContext->Map(md3dVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	//memcpy(mappedResource.pData, vertexs, sizeof(vertexs));
	//pImmediateContext->Unmap(md3dVertexBuffer, 0);
	return true;
}

void ModelClass::ShutdownBuffer()
{
	//�ͷŶ��㻺�����������
	ReleaseCOM1(md3dIndexBuffer);
	ReleaseCOM1(md3dVertexBuffer);
}

void ModelClass::RenderBuffers(ID3D11DeviceContext* d3dDeviceContext)
{
	//���ö��㻺��
	UINT stride = sizeof(Vertex); //ÿ������Ԫ�صĿ�ȴ�С������˵ÿ������Ԫ�صĴ�С
	UINT offset = 0;
	d3dDeviceContext->IASetVertexBuffers(0, 1, &md3dVertexBuffer, &stride, &offset);

	//������������
	d3dDeviceContext->IASetIndexBuffer(md3dIndexBuffer, DXGI_FORMAT_R16_UINT, 0); //WordΪ�����ֽ�

	//�������˷�ʽ
	d3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}