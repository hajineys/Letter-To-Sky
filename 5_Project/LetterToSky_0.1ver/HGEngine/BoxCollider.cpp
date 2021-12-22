#include "BoxCollider.h"
#include "DXDevice.h"
#include "DXRenderStates.h"
#include "WICTextureLoader.h"
#include "D3Camera.h"
#include "GeometryGenerator.h"
#include "Effects.h"

BoxCollider::BoxCollider(DXDevice* pDevice)
	: m_pDXDevice(pDevice),
	md3dDevice(pDevice->GetD3dDevice()), md3dImmediateContext(pDevice->GetD3dImmediateContext()), mRenderstate(DXRenderStates::m_pWireframeRS),
	mTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST),
	mView(), mProj(),
	m_BasicMap(0),
	m_TexTransform(),
	mEyePosW(0.0f, 0.0f, 0.0f),
	m_VB(0), m_IB(0), m_IndexCount(0), m_VertexOffset(0), m_IndexOffset(0)
{
	mDirLights[0].Ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	mDirLights[0].Diffuse = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	mDirLights[0].Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	mDirLights[0].Direction = XMFLOAT3(0.57735f, -0.57735f, 0.57735f);

	mDirLights[1].Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mDirLights[1].Diffuse = XMFLOAT4(0.20f, 0.20f, 0.20f, 1.0f);
	mDirLights[1].Specular = XMFLOAT4(0.25f, 0.25f, 0.25f, 1.0f);
	mDirLights[1].Direction = XMFLOAT3(-0.57735f, -0.57735f, 0.57735f);

	mDirLights[2].Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mDirLights[2].Diffuse = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	mDirLights[2].Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mDirLights[2].Direction = XMFLOAT3(0.0f, -0.707f, -0.707f);

	mMaterial.Ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	mMaterial.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	mMaterial.Specular = XMFLOAT4(0.6f, 0.6f, 0.6f, 16.0f);

	XMMATRIX I = XMMatrixIdentity();
	m_TexTransform = I;
}

BoxCollider::~BoxCollider()
{
	ReleaseCOM(m_VB);
	ReleaseCOM(m_IB);
	ReleaseCOM(m_BasicMap);
}

void BoxCollider::Initialize(float width, float height, float depth)
{
	ID3D11Resource* texResource = nullptr;
	HR(CreateWICTextureFromFile(md3dDevice, L"../Data/Textures/base_start.dds", &texResource, &m_BasicMap));
	ReleaseCOM(texResource);	// view saves reference

	BuildBoxColliderGeometryBuffers(width, height, depth);
}

void BoxCollider::Initialize(Vector3 dimension)
{
	Initialize(dimension.x, dimension.y, dimension.z);
}

void BoxCollider::Update(D3Camera* pCamera)
{
	UpdateWorldTM();
	UpdateBoundingBox();

	mEyePosW = pCamera->GetPosition();

	mView = pCamera->View();
	mProj = pCamera->Proj();
}

void BoxCollider::Render()
{
	md3dImmediateContext->IASetInputLayout(InputLayouts::PosNomTexture);
	md3dImmediateContext->IASetPrimitiveTopology(mTopology);

	UINT stride = sizeof(VertexInputLayout::PosNomTexture);
	UINT offset = 0;

	XMMATRIX view = mView;
	XMMATRIX proj = mProj;
	XMMATRIX viewProj = view * proj;

	// Set per frame constants
	Effects::BasicFX->SetDirLights(mDirLights);
	Effects::BasicFX->SetEyePosW(mEyePosW);

	// Render State
	md3dImmediateContext->RSSetState(mRenderstate);

	ID3DX11EffectTechnique* activeTech = Effects::BasicFX->Light2TexTech;

	D3DX11_TECHNIQUE_DESC techDesc;
	activeTech->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		md3dImmediateContext->IASetVertexBuffers(0, 1, &m_VB, &stride, &offset);
		md3dImmediateContext->IASetIndexBuffer(m_IB, DXGI_FORMAT_R32_UINT, 0);

		// Draw the box
		XMMATRIX world = m_WorldTM;
		XMMATRIX worldInvTranspose = MathHelper::InverseTranspose(world);
		XMMATRIX worldViewProj = world * view * proj;

		Effects::BasicFX->SetWorld(world);
		Effects::BasicFX->SetWorldInvTranspose(worldInvTranspose);
		Effects::BasicFX->SetWorldViewProj(worldViewProj);
		Effects::BasicFX->SetMaterial(mMaterial);

		//Effects::BasicFX->SetDiffuseMap(m_CollidedMap);
		Effects::BasicFX->SetDiffuseMap(m_BasicMap);

		Effects::BasicFX->SetTexTransform(m_TexTransform);

		activeTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
		md3dImmediateContext->DrawIndexed(m_IndexCount, m_IndexOffset, m_VertexOffset);
	}
}

void BoxCollider::BuildBoxColliderGeometryBuffers(float width, float height, float depth)
{
	GeometryGenerator::MeshData box;

	GeometryGenerator geoGen;
	geoGen.CreateBox(width, height, depth, box);
	//geoGen.CreateBox(0.0f, 5.0f, 0.0f, 5.0f, 0.0f, 5.0f, box);

	// Cache the vertex offsets to each object in the concatenated vertex buffer
	m_VertexOffset = 0;

	// Cache the index count of each object
	m_IndexCount = box.Indices.size();

	// Cache the starting index for each object in the concatenated index buffer
	m_IndexOffset = 0;

	UINT totalVertexCount = box.Vertices.size();

	UINT totalIndexCount = m_IndexCount;

	XMFLOAT3 vMinf3(+MathHelper::Infinity, +MathHelper::Infinity, +MathHelper::Infinity);
	XMFLOAT3 vMaxf3(-MathHelper::Infinity, -MathHelper::Infinity, -MathHelper::Infinity);

	XMVECTOR vMin = XMLoadFloat3(&vMinf3);
	XMVECTOR vMax = XMLoadFloat3(&vMaxf3);

	// Extract the vertex elements we are interested in and pack the
	// vertices of all the meshes into one vertex buffer
	std::vector<VertexInputLayout::PosNomTexture> vertices(totalVertexCount);

	UINT k = 0;
	for (size_t i = 0; i < box.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = box.Vertices[i].Position;
		vertices[k].Normal = box.Vertices[i].Normal;
		vertices[k].Texture = box.Vertices[i].TexC;

		XMVECTOR P = XMLoadFloat3(&vertices[i].Pos);

		vMin = XMVectorMin(vMin, P);
		vMax = XMVectorMax(vMax, P);
	}

	XMStoreFloat3(&m_BoundingBox.Center, 0.5f * (vMin + vMax));
	XMStoreFloat3(&m_BoundingBox.Extents, 0.5f * (vMax - vMin));

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(VertexInputLayout::PosNomTexture) * totalVertexCount;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &vertices[0];
	HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &m_VB));

	// Pack the indices of all the meshes into one index buffer
	std::vector<UINT> indices;
	indices.insert(indices.end(), box.Indices.begin(), box.Indices.end());

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * totalIndexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indices[0];
	HR(md3dDevice->CreateBuffer(&ibd, &iinitData, &m_IB));
}

bool BoxCollider::CheckCollided(BoxCollider* pBox)
{
	if (m_BBForCollisionDetection.Intersects(pBox->GetBBForCollisionDetection()))
	{
		return true;
	}

	return false;
}

void BoxCollider::UpdateBoundingBox()
{
	m_BBForCollisionDetection.Center.x = GetPosition().x;
	m_BBForCollisionDetection.Center.y = GetPosition().y;
	m_BBForCollisionDetection.Center.z = GetPosition().z;

	m_BBForCollisionDetection.Extents = m_BoundingBox.Extents;
}