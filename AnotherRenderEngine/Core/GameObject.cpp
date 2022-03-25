#include "GameObject.h"
#include "../Render/SystemClass.h"
//#include "../Render/SystemClass.cpp"
//#include "SDEngine/Texture/TextureSamplerManager.h"
//#include "SDEngine/Common/GraphicsConfig.h"
//#include "SDEngine/Texture/TextureManager.h"
//#include "SDEngine/Shader/Material.h"
//#include "Log.h"
//extern SystemClass* D3DAPP = NULL;
GameObject::GameObject()
{
	Init();
}

GameObject::~GameObject()
{
	Shutdown();
}

bool GameObject::Init()
{

	m_pTransform = shared_ptr<Transform>(new Transform());
	m_pTransform->localPosition = XMFLOAT3(1, -1, 20);
	m_pTransform->localScale = XMFLOAT3(0.2, 0.2, 0.2);
	return true;
}

void GameObject::Shutdown()
{
}
UINT GameObject::RenderTest(UINT stride) {
	//XMMATRIX worldMatrix = this->GetWorldMatrix();
	//设置顶点缓存
	auto deviceContext = GDirectxCore->Graphics()->D3D()->GetDeviceContext();
	vector<ModelData>& mModelList = m_pMesh->m_pFBXModel->mModelList;
	ModelData* mModelData = &mModelList[0];
	UINT count = 0;
	vector<MeshData>& mMeshList = mModelData->mMeshList;
	for (UINT i = 0; i < mMeshList.size(); ++i)
	{
		MeshData& mesh = mMeshList[i];
		////每个顶点元素的跨度大小，或者说每个顶点元素的大小
		//UINT stride = sizeof(mesh.mVertexData[0]);//TODO:这个元素，直接读取 shader 的 InputLayout 的长度
		UINT offset = 0;
		deviceContext->IASetVertexBuffers(0, 1, & mesh.mVertexBuffer, &stride, &offset);
		//设置索引缓存
		deviceContext->IASetIndexBuffer(mesh.mIndexBuffer, DXGI_FORMAT_R16_UINT, 0); //Word为两个字节
		//设置拓扑方式
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);



		//deviceContext->DrawIndexed((UINT)mesh.mIndexData.size(), 0, 0);
		count = (UINT)mesh.mIndexData.size();
	}

	return count;
}
void GameObject::Render(RenderMode renderMode)
{
	MaterialType eMaterialType = m_pMesh->m_eMaterialType;
	XMMATRIX worldMatrix = this->GetWorldMatrix();
	XMFLOAT4 errorShaderColor = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);

	auto deviceContext = GDirectxCore->Graphics()->D3D()->GetDeviceContext();
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	vector<ModelData>& mModelList = m_pMesh->m_pFBXModel->mModelList;
	for (UINT index = 0; index < mModelList.size(); ++index)
	{
		ModelData* mModelData = &mModelList[index];
		vector<MeshData>& mMeshList = mModelData->mMeshList;

		for (UINT i = 0; i < mMeshList.size(); ++i)
		{	
			MeshData& mesh = mMeshList[i];

			MaterialTexFileName& material = mModelData->mMaterialMap[mesh.materialId];
		/*	ID3D11ShaderResourceView* albedoSRV = LoadTexture(material.diffuseMapFileName);
			ID3D11ShaderResourceView* bumpSRV = LoadTexture(material.bumpMapFileName);
			ID3D11ShaderResourceView* specSRV = LoadTexture(material.specularMapFileName);
			ID3D11ShaderResourceView* alphaSRV = LoadTexture(material.alphaMapFileName);
			ID3D11ShaderResourceView* roughnessSRV = LoadTexture(material.roughnessMapFileName);
			ID3D11ShaderResourceView* metalSRV = LoadTexture(material.metalMapFileName);*/
			//roughnessSRV = (roughnessSRV == nullptr ? GWhiteTextureSrv : roughnessSRV);
			//metalSRV = (metalSRV == nullptr ? GWhiteTextureSrv : metalSRV);

			if (renderMode == RenderMode::Simple)
			{
				//GShaderManager->depthShader->SetMatrix("World", worldMatrix);
				//GShaderManager->depthShader->SetMatrix("View", GCamera_deprecated->GetViewMatrix());
				//GShaderManager->depthShader->SetMatrix("Proj", GCamera_deprecated->GetProjectionMatrix());
				//GShaderManager->depthShader->Apply();
			}

			else if (eMaterialType == MaterialType::PURE_COLOR)
			{
				/*GShaderManager->pureColorShader->SetMatrix("World", worldMatrix);
				GShaderManager->pureColorShader->SetMatrix("View", GCamera_deprecated->GetViewMatrix());
				GShaderManager->pureColorShader->SetMatrix("Proj", GCamera_deprecated->GetProjectionMatrix());
				GShaderManager->pureColorShader->SetMatrix("WorldInvTranspose", FMath::GetInvenseTranspose(worldMatrix));
				GShaderManager->pureColorShader->SetFloat4("surfaceColor", m_pMesh->pureColor);
				GShaderManager->pureColorShader->SetFloat("roughness", m_pMesh->roughness);
				GShaderManager->pureColorShader->SetFloat("metal", m_pMesh->metal);
				GShaderManager->pureColorShader->Apply();*/
			}

			else if (eMaterialType == MaterialType::DIFFUSE)
			{
				//if (albedoSRV == nullptr|| material.diffuseMapFileName =="")
				//{
				//	GShaderManager->pureColorShader->SetMatrix("World", worldMatrix);
				//	GShaderManager->pureColorShader->SetMatrix("View", GCamera_deprecated->GetViewMatrix());
				//	GShaderManager->pureColorShader->SetMatrix("Proj", GCamera_deprecated->GetProjectionMatrix());
				//	GShaderManager->pureColorShader->SetMatrix("WorldInvTranspose", FMath::GetInvenseTranspose(worldMatrix));
				//	GShaderManager->pureColorShader->SetFloat4("surfaceColor", m_pMesh->pureColor);
				//	GShaderManager->pureColorShader->SetFloat("roughness", m_pMesh->roughness);
				//	GShaderManager->pureColorShader->SetFloat("metal", m_pMesh->metal);
				//	GShaderManager->pureColorShader->Apply();
				//}
				//else if (albedoSRV)
				//{
				//	//ViewMatrix,ProjMatrix,UIOrthoMatrix
				//	GShaderManager->diffuseShader->SetMatrix("World", worldMatrix);
				//	GShaderManager->diffuseShader->SetMatrix("View", GCamera_deprecated->GetViewMatrix());
				//	GShaderManager->diffuseShader->SetMatrix("Proj", GCamera_deprecated->GetProjectionMatrix());
				//	GShaderManager->diffuseShader->SetMatrix("WorldInvTranspose", FMath::GetInvenseTranspose(worldMatrix));
				//	GShaderManager->diffuseShader->SetTexture("AlbedoTexture", albedoSRV);
				//	GShaderManager->diffuseShader->SetTexture("SpecularTexture", GWhiteTextureSrv);
				//	GShaderManager->diffuseShader->SetTexture("RoughnessTexture", roughnessSRV);
				//	GShaderManager->diffuseShader->SetTexture("MetalTexture", metalSRV);
				//	GShaderManager->diffuseShader->SetTextureSampler("SampleWrapLinear", GTextureSamplerBilinearWrap);
				//	GShaderManager->diffuseShader->Apply();
				//}
			}

			else if (eMaterialType == MaterialType::DIFFUSE_NORMAL)
			{
				/*if (albedoSRV && bumpSRV&&material.bumpMapFileName != "")
				{
					GShaderManager->diffuseNormalShader->SetMatrix("World", worldMatrix);
					GShaderManager->diffuseNormalShader->SetMatrix("View", GCamera_deprecated->GetViewMatrix());
					GShaderManager->diffuseNormalShader->SetMatrix("Proj", GCamera_deprecated->GetProjectionMatrix());
					GShaderManager->diffuseNormalShader->SetMatrix("WorldInvTranspose", FMath::GetInvenseTranspose(worldMatrix));
					GShaderManager->diffuseNormalShader->SetTexture("AlbedoTexture", albedoSRV);
					GShaderManager->diffuseNormalShader->SetTexture("NormalTexture", bumpSRV);
					GShaderManager->diffuseNormalShader->SetTexture("SpecularTexture", GWhiteTextureSrv);
					GShaderManager->diffuseNormalShader->SetTexture("RoughnessTexture", roughnessSRV);
					GShaderManager->diffuseNormalShader->SetTexture("MetalTexture", metalSRV);
					GShaderManager->diffuseNormalShader->SetTextureSampler("SampleWrapLinear", GTextureSamplerBilinearWrap);
					GShaderManager->diffuseNormalShader->Apply();
				}
				else
				{
					GShaderManager->pureColorShader->SetMatrix("World", worldMatrix);
					GShaderManager->pureColorShader->SetMatrix("View", GCamera_deprecated->GetViewMatrix());
					GShaderManager->pureColorShader->SetMatrix("Proj", GCamera_deprecated->GetProjectionMatrix());
					GShaderManager->pureColorShader->SetMatrix("WorldInvTranspose", FMath::GetInvenseTranspose(worldMatrix));
					GShaderManager->pureColorShader->SetFloat4("surfaceColor", errorShaderColor);
					GShaderManager->pureColorShader->SetFloat("roughness", m_pMesh->roughness);
					GShaderManager->pureColorShader->SetFloat("metal", m_pMesh->metal);
					GShaderManager->pureColorShader->Apply();
				}*/
			}

			else if (eMaterialType == MaterialType::DIFFUSE_SPECULAR)
			{
				/*if (albedoSRV && specSRV&&material.specularMapFileName != "")
				{
					GShaderManager->diffuseSpecShader->SetMatrix("World", worldMatrix);
					GShaderManager->diffuseSpecShader->SetMatrix("View", GCamera_deprecated->GetViewMatrix());
					GShaderManager->diffuseSpecShader->SetMatrix("Proj", GCamera_deprecated->GetProjectionMatrix());
					GShaderManager->diffuseSpecShader->SetMatrix("WorldInvTranspose", FMath::GetInvenseTranspose(worldMatrix));
					GShaderManager->diffuseSpecShader->SetTexture("AlbedoTexture", albedoSRV);
					GShaderManager->diffuseSpecShader->SetTexture("SpecularTexture", specSRV);
					GShaderManager->diffuseSpecShader->SetTexture("RoughnessTexture", roughnessSRV);
					GShaderManager->diffuseSpecShader->SetTexture("MetalTexture", metalSRV);
					GShaderManager->diffuseSpecShader->SetTextureSampler("SampleWrapLinear", GTextureSamplerBilinearWrap);
					GShaderManager->diffuseSpecShader->Apply();
				}
				else
				{
					GShaderManager->pureColorShader->SetMatrix("World", worldMatrix);
					GShaderManager->pureColorShader->SetMatrix("View", GCamera_deprecated->GetViewMatrix());
					GShaderManager->pureColorShader->SetMatrix("Proj", GCamera_deprecated->GetProjectionMatrix());
					GShaderManager->pureColorShader->SetMatrix("WorldInvTranspose", FMath::GetInvenseTranspose(worldMatrix));
					GShaderManager->pureColorShader->SetFloat4("surfaceColor", errorShaderColor);
					GShaderManager->pureColorShader->SetFloat("roughness", m_pMesh->roughness);
					GShaderManager->pureColorShader->SetFloat("metal", m_pMesh->metal);
					GShaderManager->pureColorShader->Apply();
				}*/
			}

			else if (eMaterialType == MaterialType::DIFFUSE_NORMAL_SPECULAR)
			{
				/*if (albedoSRV && bumpSRV&&specSRV && material.bumpMapFileName != "" &&  material.specularMapFileName!="")
				{
					GShaderManager->diffuseNormalSpecShader->SetMatrix("World", worldMatrix);
					GShaderManager->diffuseNormalSpecShader->SetMatrix("View", GCamera_deprecated->GetViewMatrix());
					GShaderManager->diffuseNormalSpecShader->SetMatrix("Proj", GCamera_deprecated->GetProjectionMatrix());
					GShaderManager->diffuseNormalSpecShader->SetMatrix("WorldInvTranspose", FMath::GetInvenseTranspose(worldMatrix));
					GShaderManager->diffuseNormalSpecShader->SetTexture("AlbedoTexture", albedoSRV);
					GShaderManager->diffuseNormalSpecShader->SetTexture("NormalTexture", bumpSRV);
					GShaderManager->diffuseNormalSpecShader->SetTexture("SpecularTexture", specSRV);
					GShaderManager->diffuseNormalSpecShader->SetTexture("RoughnessTexture", roughnessSRV);
					GShaderManager->diffuseNormalSpecShader->SetTexture("MetalTexture", metalSRV);
					GShaderManager->diffuseNormalSpecShader->SetTextureSampler("SampleWrapLinear", 
						GTextureSamplerBilinearWrap);
					GShaderManager->diffuseNormalSpecShader->Apply();
				}
				else
				{
					GShaderManager->pureColorShader->SetMatrix("World", worldMatrix);
					GShaderManager->pureColorShader->SetMatrix("View", GCamera_deprecated->GetViewMatrix());
					GShaderManager->pureColorShader->SetMatrix("Proj", GCamera_deprecated->GetProjectionMatrix());
					GShaderManager->pureColorShader->SetMatrix("WorldInvTranspose", FMath::GetInvenseTranspose(worldMatrix));
					GShaderManager->pureColorShader->SetFloat4("surfaceColor", errorShaderColor);
					GShaderManager->pureColorShader->SetFloat("roughness", m_pMesh->roughness);
					GShaderManager->pureColorShader->SetFloat("metal", m_pMesh->metal);
					GShaderManager->pureColorShader->Apply();
				}*/
			}

			else if(eMaterialType == MaterialType::WIRE_FRAME)
			{
			/*	GShaderManager->pureColorShader->SetMatrix("World", worldMatrix);
				GShaderManager->pureColorShader->SetMatrix("View", GCamera_deprecated->GetViewMatrix());
				GShaderManager->pureColorShader->SetMatrix("Proj", GCamera_deprecated->GetProjectionMatrix());
				GShaderManager->pureColorShader->SetMatrix("WorldInvTranspose", FMath::GetInvenseTranspose(worldMatrix));
				GShaderManager->pureColorShader->SetFloat4("surfaceColor", XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
				GShaderManager->pureColorShader->SetFloat("roughness", m_pMesh->roughness);
				GShaderManager->pureColorShader->SetFloat("metal", m_pMesh->metal);
				GShaderManager->pureColorShader->Apply();*/

			} 

			else if (eMaterialType == MaterialType::DEPTH_BUFFER)
			{
				/*GShaderManager->depthGetShader->SetMatrix("World", worldMatrix);
				GShaderManager->depthGetShader->SetMatrix("View", GCamera_deprecated->GetViewMatrix());
				GShaderManager->depthGetShader->SetMatrix("Proj", GCamera_deprecated->GetProjectionMatrix());
				GShaderManager->depthGetShader->Apply();*/
			}

			UINT stride = sizeof(mesh.mVertexData[0]);
			UINT offset = 0;
			deviceContext->IASetVertexBuffers(0, 1, &mesh.mVertexBuffer, &stride, &offset);
			deviceContext->IASetIndexBuffer(mesh.mIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
			//TODO:ZZZFDF 这个方法暂时应该无用
			deviceContext->DrawIndexed((UINT)mesh.mIndexData.size(), 0, 0);
		}
	}

}

void GameObject::RenderMesh()
{
	auto deviceContext = g_Graphics->D3D()->GetDeviceContext();
//	auto deviceContext = D3DAPP->Graphics()->D3D()->GetDeviceContext();
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	vector<ModelData>& mModelList = m_pMesh->m_pFBXModel->mModelList;

	for (UINT index = 0; index < mModelList.size(); ++index)
	{
		ModelData* mModelData = &mModelList[index];
		vector<MeshData>& mMeshList = mModelData->mMeshList;

		for (UINT i = 0; i < mMeshList.size(); ++i)
		{
			printf("mesh list i=%d \n", i);
			//MeshData& mesh = mMeshList[i];
			//UINT stride = sizeof(mesh.mVertexData[0]);
			//UINT offset = 0;
			//deviceContext->IASetVertexBuffers(0, 1, &mesh.mVertexBuffer, &stride, &offset);
			//deviceContext->IASetIndexBuffer(mesh.mIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
			////TODO:#define GDirectxCore (DirectxCore::Get())
			////GDirectxCore->DrawIndexed((UINT)mesh.mIndexData.size(), 0, 0);
			//deviceContext->DrawIndexed((UINT)mesh.mIndexData.size(), 0, 0);
		}
	}
}
XMMATRIX GameObject::GetInvenseTranspose(CXMMATRIX ma)
{
	XMMATRIX A = ma;

	A.r[3] = { 0.0f,0.0f,0.0f,1.0f };

	XMVECTOR det = XMMatrixDeterminant(A);
	XMMATRIX MaInvense = XMMatrixInverse(&det, A);
	return XMMatrixTranspose(MaInvense);
}
XMMATRIX GameObject::GetWorldMatrix()
{
	const float GAME_ENGINE_UNIT_SCALE = 1.0f;
	XMMATRIX scale = XMMatrixScaling(m_pTransform->localScale.x * GAME_ENGINE_UNIT_SCALE, 
		m_pTransform->localScale.y * GAME_ENGINE_UNIT_SCALE, m_pTransform->localScale.z * GAME_ENGINE_UNIT_SCALE);

	XMMATRIX rotation = XMMatrixRotationX(m_pTransform->localRotation.x / 180.0f * XM_PI)
		* XMMatrixRotationY(m_pTransform->localRotation.y / 180.0f * XM_PI)
		* XMMatrixRotationZ(m_pTransform->localRotation.z / 180.0f * XM_PI);

	XMMATRIX translation = XMMatrixTranslation(m_pTransform->localPosition.x * GAME_ENGINE_UNIT_SCALE,
		m_pTransform->localPosition.y * GAME_ENGINE_UNIT_SCALE, m_pTransform->localPosition.z *  GAME_ENGINE_UNIT_SCALE);

	XMMATRIX worldMatrix = scale * rotation * translation;

	return worldMatrix;
}

void GameObject::DrawMesh()
{
	shared_ptr<Material> material = m_pMesh->GetMaterial();
	if (!material)
	{
		//Log::Warn("material is nullptr");
		printf("material is nullptr");
	}

	//material->SetMatrix("World", GetWorldMatrix());
	//material->SetMatrix("View", GCamera_deprecated->GetViewMatrix());
	//material->SetMatrix("Proj", GCamera_deprecated->GetProjectionMatrix());
	//material->Apply();
	RenderMesh();
}

void GameObject::SetMesh(shared_ptr<MeshComponent> pMesh)
{
	m_pMesh = pMesh;
}