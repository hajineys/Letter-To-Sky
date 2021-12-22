#include "BitmapObject.h"
#include "DXDevice.h"
#include "DirectXStruct.h"
#include "WICTextureLoader.h"
#include "DXRenderStates.h"
#include "Vertex.h"
#include "Effects.h"

BitmapObject::BitmapObject(DXDevice* pDevice)
	: IDXObjectBase(pDevice),
	m_pDirectX(pDevice),
	m_VB(0), m_IB(0),
	m_VertexCount(0), m_IndexCount(0),
	mDiffuseMapSRV(0),
	m_ScreenWidth(0), m_ScreenHeight(0),
	m_BitmapWidth(0), m_BitmapHeight(0),
	m_PositionX(0), m_PositionY(0),
	m_PreviousPosX(0), m_PreviousPosY(0)
{

}

BitmapObject::~BitmapObject()
{
	ReleaseCOM(m_VB);
	ReleaseCOM(m_IB);
	ReleaseCOM(mDiffuseMapSRV);
}

void BitmapObject::Initialize(int bitmapWidth, int bitmapHeight)
{
	// 화면 크기를 멤버변수에 저장
	m_ScreenWidth = m_pDirectX->GetClientWidth();
	m_ScreenHeight = m_pDirectX->GetClientHeight();

	// 렌더링할 비트맵의 픽셀의 크기를 저장
	m_BitmapWidth = bitmapWidth;
	m_BitmapHeight = bitmapHeight;

	// 이전 렌더링 위치를 음수로 초기화합니다.
	m_PreviousPosX = -1;
	m_PreviousPosY = -1;
}

void BitmapObject::LoadGeometry(SGeoBuffer* geoBuffer)
{
	// 값 복사
	m_VB = geoBuffer->m_VB;
	m_IB = geoBuffer->m_IB;
	m_IndexCount = geoBuffer->m_IndexCount;
	m_VertexCount = geoBuffer->m_VertexCount;
}

void BitmapObject::LoadTextureFile(const wchar_t* TextureFile)
{
	ID3D11Resource* texResource = nullptr;
	HR(CreateWICTextureFromFile(m_pDirectX->GetD3dDevice(), TextureFile, &texResource, &mDiffuseMapSRV));
	ReleaseCOM(texResource);	// view saves reference
}

void BitmapObject::Update(GameTimer* pTimer /*= nullptr*/, D3Camera* pCamera /*= nullptr*/)
{
	UpdateWorldTM();

	mProj = m_pDirectX->GetOrthoMatrix();

	float left, right, top, bottom;
	VertexInputLayout::PosTexture* vertices;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexInputLayout::PosTexture* verticesPtr;
	HRESULT result;

	// 이 비트맵을 렌더링 할 위치가 변경되지 않은 경우 정점 버퍼를 업데이트 하지 마십시오.
	// 현재 올바른 매개 변수가 있습니다.
	if ((m_PositionX == m_PreviousPosX) && (m_PositionY == m_PreviousPosY))
	{
		return;
	}

	// 변경된 경우 렌더링되는 위치를 업데이트합니다.
	m_PreviousPosX = m_PositionX;
	m_PreviousPosY = m_PositionY;

	// 비트 맵 왼쪽의 화면 좌표를 계산합니다.
	left = (float)((m_ScreenWidth / 2) * -1) + (float)m_PositionX;

	// 비트 맵 오른쪽의 화면 좌표를 계산합니다.
	right = left + (float)m_BitmapWidth;

	// 비트 맵 상단의 화면 좌표를 계산합니다.
	top = (float)(m_ScreenHeight / 2) - (float)m_PositionY;

	// 비트 맵 아래쪽의 화면 좌표를 계산합니다.
	bottom = top - (float)m_BitmapHeight;

	// 정점 배열을 만듭니다.
	vertices = new VertexInputLayout::PosTexture[m_VertexCount];

	// 정점 배열에 데이터를로드합니다.
	// 첫 번째 삼각형
	vertices[0].Pos = XMFLOAT3(left, top, 0.0f);  // Top left.
	vertices[0].Texture = XMFLOAT2(0.0f, 0.0f);

	vertices[1].Pos = XMFLOAT3(right, bottom, 0.0f);  // Bottom right.
	vertices[1].Texture = XMFLOAT2(1.0f, 1.0f);

	vertices[2].Pos = XMFLOAT3(left, bottom, 0.0f);  // Bottom left.
	vertices[2].Texture = XMFLOAT2(0.0f, 1.0f);

	// 두 번째 삼각형
	vertices[3].Pos = XMFLOAT3(left, top, 0.0f);  // Top left.
	vertices[3].Texture = XMFLOAT2(0.0f, 0.0f);

	vertices[4].Pos = XMFLOAT3(right, top, 0.0f);  // Top right.
	vertices[4].Texture = XMFLOAT2(1.0f, 0.0f);

	vertices[5].Pos = XMFLOAT3(right, bottom, 0.0f);  // Bottom right.
	vertices[5].Texture = XMFLOAT2(1.0f, 1.0f);

	// 버텍스 버퍼를 쓸 수 있도록 잠급니다.
	result = m_pDirectX->GetD3dImmediateContext()->Map(m_VB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	// 정점 버퍼의 데이터를 가리키는 포인터를 얻는다.
	verticesPtr = (VertexInputLayout::PosTexture*)mappedResource.pData;

	// 데이터를 정점 버퍼에 복사합니다.
	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexInputLayout::PosTexture) * m_VertexCount));

	// 정점 버퍼의 잠금을 해제합니다.
	m_pDirectX->GetD3dImmediateContext()->Unmap(m_VB, 0);

	// 더 이상 필요하지 않은 꼭지점 배열을 해제합니다.
	delete[] vertices;
	vertices = 0;
}

void BitmapObject::Render()
{
	// 모든 2D 렌더링을 시작하려면 Z 버퍼를 끕니다.
	m_pDirectX->TurnZBufferOff();

	m_pDirectX->GetD3dImmediateContext()->IASetInputLayout(InputLayouts::PosTexture);
	m_pDirectX->GetD3dImmediateContext()->IASetPrimitiveTopology(mTopology);

	// 정점 버퍼의 단위와 오프셋을 설정합니다.
	UINT stride = sizeof(VertexInputLayout::PosTexture);
	UINT offset = 0;
	m_pDirectX->GetD3dImmediateContext()->IASetVertexBuffers(0, 1, &m_VB, &stride, &offset);
	m_pDirectX->GetD3dImmediateContext()->IASetIndexBuffer(m_IB, DXGI_FORMAT_R32_UINT, 0);

	// Set constants
	XMMATRIX world = XMMatrixIdentity();
	XMMATRIX view = XMMatrixIdentity();
	XMMATRIX proj = mProj;
	XMMATRIX worldViewProj = world * view * proj;

	Effects::BitmapFX->SetWorldViewProj(worldViewProj);

	// Render State
	m_pDirectX->GetD3dImmediateContext()->RSSetState(DXRenderStates::m_pSolidRS);

	ID3DX11EffectTechnique* activeTech = Effects::BitmapFX->LightTech;

	D3DX11_TECHNIQUE_DESC techDesc;
	activeTech->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		Effects::BitmapFX->SetDiffuseMap(mDiffuseMapSRV);

		activeTech->GetPassByIndex(p)->Apply(0, m_pDirectX->GetD3dImmediateContext());
		m_pDirectX->GetD3dImmediateContext()->DrawIndexed(m_IndexCount, 0, 0);
	}

	// 모든 2D 렌더링이 완료되었으므로 Z 버퍼를 다시 켜십시오.
	m_pDirectX->TurnZBufferOn();
}

void BitmapObject::SetBitmapPosition(int x, int y)
{
	m_PositionX = x;
	m_PositionY = y;
}

void BitmapObject::CheckUIPick(int x, int y)
{
	// 마우스좌표 x,y

	if (x > m_PositionX && y > m_PositionY && x < m_PositionX + m_BitmapWidth && y < m_PositionY + m_BitmapHeight)
		m_IsPicked = true;
	else
		m_IsPicked = false;
}

void BitmapObject::CheckUICollided(int x, int y)
{
	if (x > m_PositionX && y > m_PositionY && x < m_PositionX + m_BitmapWidth && y < m_PositionY + m_BitmapHeight)
		m_IsCollided = true;
	else
		m_IsCollided = false;
}
