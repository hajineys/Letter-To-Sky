#include "pch.h"
#include "ColorObject.h"
#include "Effects.h"

ColorObject::ColorObject(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, ID3D11RasterizerState* pRS)
	:IDXObject(pDevice, pDeviceContext, pRS)
{

}

ColorObject::~ColorObject()
{
	
}

void ColorObject::Initialize()
{
	BuildGeometryBuffers();
}

void ColorObject::Render()
{
	// 입력 배치 객체 셋팅
	md3dImmediateContext->IASetInputLayout(InputLayouts::PosColor);
	md3dImmediateContext->IASetPrimitiveTopology(mTopology);

	// 정점 버퍼들을 생성
	UINT stride = sizeof(VertexInputLayout::PosColor);
	UINT offset = 0;
	md3dImmediateContext->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
	md3dImmediateContext->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);

	// Set constants
	XMMATRIX world = mWorld;
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
		md3dImmediateContext->DrawIndexed(mIndexCount, 0, 0);
	}
}
