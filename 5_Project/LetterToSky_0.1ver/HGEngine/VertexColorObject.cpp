#include "VertexColorObject.h"
#include "DirectXStruct.h"
#include "Vertex.h"
#include "Effects.h"

VertexColorObject::VertexColorObject(DXDevice* pDevice)
	: IDXObjectBase(pDevice),
	m_VB(0), m_IB(0), m_IndexCount(0)
{

}

VertexColorObject::~VertexColorObject()
{
	ReleaseCOM(m_VB);
	ReleaseCOM(m_IB);
}

void VertexColorObject::Render()
{
	IDXObjectBase::Render();

	// 입력 배치 객체 셋팅
	md3dImmediateContext->IASetInputLayout(InputLayouts::PosColor);
	md3dImmediateContext->IASetPrimitiveTopology(mTopology);

	// 정점 버퍼들을 생성
	UINT stride = sizeof(VertexInputLayout::PosColor);
	UINT offset = 0;
	md3dImmediateContext->IASetVertexBuffers(0, 1, &m_VB, &stride, &offset);
	md3dImmediateContext->IASetIndexBuffer(m_IB, DXGI_FORMAT_R32_UINT, 0);

	// Set constants
	XMMATRIX world = m_WorldTM;
	XMMATRIX view = mView;
	XMMATRIX proj = mProj;
	XMMATRIX worldViewProj = world * view * proj;

	// Set per frame constants
	Effects::ColorFX->SetWorldViewProj(worldViewProj);

	// Render State
	md3dImmediateContext->RSSetState(mRenderstate);

	// Figure out which technique to use
	ID3DX11EffectTechnique* mTech = Effects::ColorFX->LightTech;

	// 테크닉
	D3DX11_TECHNIQUE_DESC techDesc;
	mTech->GetDesc(&techDesc);

	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		mTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);

		// Index Count만큼 그린다
		md3dImmediateContext->DrawIndexed(m_IndexCount, 0, 0);
	}
}

void VertexColorObject::LoadGeometry(SGeoBuffer* geoBuffer)
{
	m_VB = geoBuffer->m_VB;
	m_IB = geoBuffer->m_IB;
	m_IndexCount = geoBuffer->m_IndexCount;
}
