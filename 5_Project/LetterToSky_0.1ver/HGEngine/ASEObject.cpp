#include "ASEObject.h"
#include "GameTimer.h"
#include "D3Camera.h"
#include "Vertex.h"
#include "WICTextureLoader.h"
#include "Effects.h"
#include "MathHelper.h"

ASEObject::ASEObject(DXDevice* pDevice)
	: IDXObjectBase(pDevice),
	m_VB(0),
	m_IB(0),
	m_IndexCount(0),
	mVertexOffset(0),
	mIndexOffset(0),
	m_MaxParentMeshCount(0),
	mDiffuseMapSRV(0),
	m_LightCount(4)
{
	XMMATRIX I = XMMatrixIdentity();
	mTexTransform = I;

	m_DirLights[0].Ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	m_DirLights[0].Diffuse = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	m_DirLights[0].Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	m_DirLights[0].Direction = XMFLOAT3(0.57735f, -0.57735f, 0.57735f);

	m_DirLights[1].Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	m_DirLights[1].Diffuse = XMFLOAT4(0.20f, 0.20f, 0.20f, 1.0f);
	m_DirLights[1].Specular = XMFLOAT4(0.25f, 0.25f, 0.25f, 1.0f);
	m_DirLights[1].Direction = XMFLOAT3(-0.57735f, -0.57735f, 0.57735f);

	m_DirLights[2].Ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	m_DirLights[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_DirLights[2].Specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f);
	m_DirLights[2].Direction = XMFLOAT3(-0.3f, 0.0f, 0.3f);
}

ASEObject::~ASEObject()
{
	ReleaseCOM(m_VB);
	ReleaseCOM(m_IB);
	ReleaseCOM(mDiffuseMapSRV);
}

void ASEObject::Update(GameTimer* pTimer /*= nullptr*/, D3Camera* pCamera /*= nullptr*/)
{
	IDXObjectBase::Update(pTimer, pCamera);

	// 최종 사용할 World TM을 구한다
	for (int i = 0; i < m_MaxParentMeshCount; i++)
	{
		for (UINT j = 0; j < mMeshList.size(); j++)
		{
			if (mMeshList[j]->m_parentMeshCount == i)
			{
				// 부모가 없는 경우
				if (i == 0)
				{
					if (j != 0)
					{
						mMeshList[j]->m_worldTM = mMeshList[j]->m_nodeTM;
					}
				}
				else
				{
					mMeshList[j]->m_worldTM = mMeshList[j]->m_localTM * mMeshList[j]->m_parent->m_worldTM;
				}
			}
		}
	}

	// Switch the number of lights based on key presses
	if (GetAsyncKeyState('1') & 0x8000)
		m_LightCount = 1;

	if (GetAsyncKeyState('2') & 0x8000)
		m_LightCount = 2;

	if (GetAsyncKeyState('3') & 0x8000)
		m_LightCount = 3;

	if (GetAsyncKeyState('4') & 0x8000)
		m_LightCount = 4;

	if (GetAsyncKeyState('5') & 0x8000)
		m_LightCount = 5;

	if (GetAsyncKeyState('6') & 0x8000)
		m_LightCount = 6;

	if (GetAsyncKeyState('7') & 0x8000)
		m_LightCount = 7;
}

void ASEObject::Render()
{
	IDXObjectBase::Render();

	for (UINT i = 0; i < mASEObjectMesh.size(); i++)
	{
		// 입력 배치 객체 셋팅
		md3dImmediateContext->IASetInputLayout(InputLayouts::PosNomTexture);
		md3dImmediateContext->IASetPrimitiveTopology(mTopology);

		// 렌더 스테이트
		md3dImmediateContext->RSSetState(mRenderstate);

		// 버텍스버퍼와 인덱스버퍼 셋팅
		UINT stride = sizeof(VertexInputLayout::PosNomTexture);
		UINT offset = 0;

		/// WVP TM등을 셋팅
		// Set constants
		XMMATRIX view = mView;
		XMMATRIX proj = mProj;
		XMMATRIX world = mASEObjectMesh[i]->pMesh->m_worldTM * m_WorldTM;
		XMMATRIX worldViewProj = world * view * proj;

		// Set per frame constants
		Effects::BasicFX->SetDirLights(m_DirLights);
		Effects::BasicFX->SetEyePosW(mEyePosW);

		// 월드의 역행렬
		XMMATRIX worldInvTranspose = MathHelper::InverseTranspose(world);

		// Figure out which technique to use
		ID3DX11EffectTechnique* mTech = Effects::BasicFX->Light0TexTech;

		switch (m_LightCount)
		{
		case 1:
			mTech = Effects::BasicFX->Light1Tech;
			break;
		case 2:
			mTech = Effects::BasicFX->Light2Tech;
			break;
		case 3:
			mTech = Effects::BasicFX->Light3Tech;
			break;
		case 4:
			mTech = Effects::BasicFX->Light0TexTech;
			break;
		case 5:
			mTech = Effects::BasicFX->Light1TexTech;
			break;
		case 6:
			mTech = Effects::BasicFX->Light2TexTech;
			break;
		case 7:
			mTech = Effects::BasicFX->Light3TexTech;
			break;
		}

		// 테크닉
		D3DX11_TECHNIQUE_DESC techDesc;
		mTech->GetDesc(&techDesc);
		for (UINT p = 0; p < techDesc.Passes; ++p)
		{
			md3dImmediateContext->IASetVertexBuffers(0, 1, &mASEObjectMesh[i]->VB, &stride, &offset);
			md3dImmediateContext->IASetIndexBuffer(mASEObjectMesh[i]->IB, DXGI_FORMAT_R32_UINT, 0);

			// WVP는 위에서
			worldInvTranspose = MathHelper::InverseTranspose(world);

			Effects::BasicFX->SetWorld(world);
			Effects::BasicFX->SetWorldInvTranspose(worldInvTranspose);
			Effects::BasicFX->SetWorldViewProj(worldViewProj);
			Effects::BasicFX->SetTexTransform(mTexTransform);
			Effects::BasicFX->SetMaterial(mMaterial);
			Effects::BasicFX->SetDiffuseMap(mDiffuseMapSRV);

			mTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
			md3dImmediateContext->DrawIndexed(mASEObjectMesh[i]->IndexCount, mIndexOffset, mVertexOffset);
		}
	}
}

void ASEObject::LoadGeometry(CASEParser* m_pASEParser)
{
	mMeshList = m_pASEParser->m_MeshList;
	m_MaxParentMeshCount = m_pASEParser->m_MaxParentMeshCount;

	// ASE Parser에서 버텍스 정보, 인덱스 정보를 얻어온다
	UINT vertexCount = 0;
	UINT faceCount = 0;

	for (UINT j = 0; j < mMeshList.size(); j++)
	{
		// 현재 들어온 Mesh의 Type이 HelperObject라면 Vectex가 없으므로 continue;
		if (mMeshList.at(j)->m_type == eObjectType_HelperObject)
		{
			continue;
		}

		ASEObjectMesh* nowASEMesh = new ASEObjectMesh;
		nowASEMesh->pMesh = mMeshList[j];

		vertexCount = mMeshList[j]->m_opt_vertex.size();
		std::vector<VertexInputLayout::PosNomTexture> vertices(vertexCount);

		// for (ASEParser::Vertex* vertex : meshData->m_meshvertex)
		for (UINT i = 0; i < vertexCount; i++)
		{
			vertices[i].Pos.x = mMeshList[j]->m_opt_vertex[i]->m_pos.x;
			vertices[i].Pos.y = mMeshList[j]->m_opt_vertex[i]->m_pos.y;
			vertices[i].Pos.z = mMeshList[j]->m_opt_vertex[i]->m_pos.z;

			vertices[i].Normal.x = mMeshList[j]->m_opt_vertex[i]->m_normal.x;
			vertices[i].Normal.y = mMeshList[j]->m_opt_vertex[i]->m_normal.y;
			vertices[i].Normal.z = mMeshList[j]->m_opt_vertex[i]->m_normal.z;

			vertices[i].Texture.x = mMeshList[j]->m_opt_vertex[i]->u;
			vertices[i].Texture.y = mMeshList[j]->m_opt_vertex[i]->v;

			// Weight 넣기
			//for (UINT b = 0; b < mMeshList[i]->m_opt_vertex[i]->m_bone_blending_weight.size(); b++)
			//{
			//	switch (b)
			//	{
			//	case 0:
			//		vertices[i].Weights.x = mMeshList[i]->m_opt_vertex[i]->m_bone_blending_weight[b]->m_bone_weight;
			//		break;
			//	case 1:
			//		vertices[i].Weights.y = mMeshList[i]->m_opt_vertex[i]->m_bone_blending_weight[b]->m_bone_weight;
			//		break;
			//	case 2:
			//		vertices[i].Weights.z = mMeshList[i]->m_opt_vertex[i]->m_bone_blending_weight[b]->m_bone_weight;
			//		break;
			//	}
			//
			//	// Bone Index 넣기
			//	vertices[i].BoneIndices[b] = mMeshList[i]->m_opt_vertex[i]->m_bone_blending_weight[b]->m_bone_number;
			//}
		}

		faceCount = mMeshList[j]->m_mesh_numfaces;
		m_IndexCount = 3 * faceCount;

		std::vector<UINT> indices(m_IndexCount);
		for (UINT i = 0; i < faceCount; ++i)
		{
			indices[i * 3 + 0] = mMeshList[j]->m_opt_index[i].index[0];
			indices[i * 3 + 1] = mMeshList[j]->m_opt_index[i].index[1];
			indices[i * 3 + 2] = mMeshList[j]->m_opt_index[i].index[2];
		}

		// 버텍스나 인덱스가 없으면 그냥 리턴 (본, 헬퍼 등)
		if (vertexCount == 0 || m_IndexCount == 0) return;

		D3D11_BUFFER_DESC vbd;
		vbd.Usage = D3D11_USAGE_IMMUTABLE;
		vbd.ByteWidth = sizeof(VertexInputLayout::PosNomTexture) * vertexCount;
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbd.CPUAccessFlags = 0;
		vbd.MiscFlags = 0;
		D3D11_SUBRESOURCE_DATA vinitData;
		vinitData.pSysMem = &vertices[0];
		HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &m_VB));

		// Pack the indices of all the meshes into one index buffer
		D3D11_BUFFER_DESC ibd;
		ibd.Usage = D3D11_USAGE_IMMUTABLE;
		ibd.ByteWidth = sizeof(UINT) * m_IndexCount;
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.CPUAccessFlags = 0;
		ibd.MiscFlags = 0;
		D3D11_SUBRESOURCE_DATA iinitData;
		iinitData.pSysMem = &indices[0];
		HR(md3dDevice->CreateBuffer(&ibd, &iinitData, &m_IB));

		nowASEMesh->VB = m_VB;
		nowASEMesh->IB = m_IB;
		nowASEMesh->IndexCount = m_IndexCount;
		mASEObjectMesh.push_back(nowASEMesh);
	}

	m_pASEParser->m_MeshList.clear();
}

void ASEObject::LoadTextureFile(const wchar_t* TextureFile)
{
	ID3D11Resource* texResource = nullptr;
	HR(CreateWICTextureFromFile(md3dDevice, TextureFile, &texResource, &mDiffuseMapSRV));
	ReleaseCOM(texResource);	// view saves reference
}

void ASEObject::Finalize()
{
	ReleaseCOM(m_VB);
	ReleaseCOM(m_IB);
	ReleaseCOM(mDiffuseMapSRV);
}
