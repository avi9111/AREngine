#include"ModelClass.h"
#include "../Maths/matrix.h"

ModelClass::ModelClass()
{
    md3dVertexBuffer=NULL; //顶点缓存
    md3dIndexBuffer=NULL;  //索引缓存
	mVertexCount = 0;
	mIndexCount = 0;

}


ModelClass::ModelClass(const ModelClass& other)
{

}

ModelClass::~ModelClass()
{

}

bool ModelClass::Initialize(ID3D11Device* d3dDevice)
{
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
	//设置渲染管线的顶点缓存和索引缓存(IA阶段)
	RenderBuffers(d3dDeviceContext);
}
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

bool ModelClass::InitializeBuffer(ID3D11Device* d3dDevice)
{
	Vertex* vertexs=NULL;
	WORD*indices=NULL;  //一个字为两个字节 

	mVertexCount = 4;
	mIndexCount = 6;

	//创建顶点数组
	vertexs = new Vertex[mVertexCount];
	if (!vertexs)
		return false;

	//创建索引数组
	indices = new WORD[mIndexCount];
	if (!indices)
		return false;
	
	//赋予顶点数组数据和索引数组数据
	vertexs[0].pos = XMFLOAT3(-1.0f, -1.0f, 0.0f);
	vertexs[0].color = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	vertexs[1].pos = XMFLOAT3(1.0f, 1.0f, 0.0f);
	vertexs[1].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	vertexs[2].pos = XMFLOAT3(1.0f, -1.0f, 0.0f);
	vertexs[2].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	vertexs[3].pos = XMFLOAT3(-1.0f, 1.0f, 0.0f);
	vertexs[3].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	/* int const data[ 4 * 4 ] =
     {
       1, 0, 0, 0,
       0, 1, 0, 0,
	   0, 0, 1, 0,
	   0, 0, 0, 1
     };
    Matrix< int > matrix( 4, 4 );
    matrix = data;*/

	//vertexs[0] = matrix * vertexs[0];
	for (int i = 0; i < 4; i++)
	{
		float* f = ToMatrix(vertexs[i]);
		vertexs[i].pos.x = f[0];
		vertexs[i].pos.y = f[1];
		vertexs[i].pos.z = f[2];
	}


	//赋予索引数组数据

	//注意用左手定则判定是不是背面
	indices[0] = 0;
	indices[1] = 3;
	indices[2] = 2;
	indices[3] = 1;
	indices[4] = 2;
	indices[5] = 3;


	//第一,填充(顶点)缓存形容结构体和子资源数据结构体,并创建顶点缓存
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * mVertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = vertexs;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;
	HR1(d3dDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &md3dVertexBuffer));

	//第二,填充(索引)缓存形容结构体和子资源数据结构体,并创建索引缓存
	D3D11_BUFFER_DESC  indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(WORD) * mIndexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;
    HR1(d3dDevice->CreateBuffer(&indexBufferDesc, &indexData, &md3dIndexBuffer));

	//释放顶点数组和索引数组(这时数据已经载入缓存,不需要这些数组了)
	delete[]vertexs;
	vertexs = NULL;
	delete[]indices;
	indices = NULL;
	
	return true;
}

void ModelClass::ShutdownBuffer()
{
	//释放顶点缓存和索引缓存
	ReleaseCOM1(md3dIndexBuffer);
	ReleaseCOM1(md3dVertexBuffer);
}

void ModelClass::RenderBuffers(ID3D11DeviceContext* d3dDeviceContext)
{
	//设置顶点缓存
	UINT stride = sizeof(Vertex); //每个顶点元素的跨度大小，或者说每个顶点元素的大小
	UINT offset = 0;
	d3dDeviceContext->IASetVertexBuffers(0, 1, &md3dVertexBuffer, &stride, &offset);

	//设置索引缓存
	d3dDeviceContext->IASetIndexBuffer(md3dIndexBuffer, DXGI_FORMAT_R16_UINT, 0); //Word为两个字节

	//设置拓扑方式
	d3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}