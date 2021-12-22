#include "TextureObject.h"
#include "Vertex.h"
#include "Effects.h"
#include "MathHelper.h"
#include "DirectXStruct.h"
#include "WICTextureLoader.h"

TextureObject::TextureObject(DXDevice* pDevice)
	: IDXObjectBase(pDevice),
	m_VB(0), m_IB(0), m_IndexCount(0),
	m_DiffuseMapSRV(0),
	m_TexTransform(),
	m_VertexOffset(0), m_IndexOffset(0)
{
	XMMATRIX I = XMMatrixIdentity();
	m_TexTransform = I;
}

TextureObject::~TextureObject()
{
	ReleaseCOM(m_DiffuseMapSRV);
}

void TextureObject::Render()
{
	IDXObjectBase::Render();

	md3dImmediateContext->IASetInputLayout(InputLayouts::PosNomTexture);
	md3dImmediateContext->IASetPrimitiveTopology(mTopology);

	UINT stride = sizeof(VertexInputLayout::PosNomTexture);
	UINT offset = 0;

	XMMATRIX view = mView;
	XMMATRIX proj = mProj;
	XMMATRIX viewProj = view * proj;

	// Set per frame constants
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
		Effects::BasicFX->SetDiffuseMap(m_DiffuseMapSRV);
		Effects::BasicFX->SetTexTransform(m_TexTransform);

		activeTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
		md3dImmediateContext->DrawIndexed(m_IndexCount, m_IndexOffset, m_VertexOffset);
	}
}

void TextureObject::LoadGeometry(SGeoBuffer* geoBuffer)
{
	// °ª º¹»ç
	m_VB = geoBuffer->m_VB;
	m_IB = geoBuffer->m_IB;
	m_IndexCount = geoBuffer->m_IndexCount;
	m_VertexOffset = geoBuffer->m_VertexOffset;
	m_IndexOffset = geoBuffer->m_IndexOffset;
}

void TextureObject::LoadTextureFile(const wchar_t* TextureFile)
{
	ID3D11Resource* texResource = nullptr;
	HR(CreateWICTextureFromFile(md3dDevice, TextureFile, &texResource, &m_DiffuseMapSRV));
	ReleaseCOM(texResource);	// view saves reference
}
