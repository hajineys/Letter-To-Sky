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
	// ȭ�� ũ�⸦ ��������� ����
	m_ScreenWidth = m_pDirectX->GetClientWidth();
	m_ScreenHeight = m_pDirectX->GetClientHeight();

	// �������� ��Ʈ���� �ȼ��� ũ�⸦ ����
	m_BitmapWidth = bitmapWidth;
	m_BitmapHeight = bitmapHeight;

	// ���� ������ ��ġ�� ������ �ʱ�ȭ�մϴ�.
	m_PreviousPosX = -1;
	m_PreviousPosY = -1;
}

void BitmapObject::LoadGeometry(SGeoBuffer* geoBuffer)
{
	// �� ����
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

	// �� ��Ʈ���� ������ �� ��ġ�� ������� ���� ��� ���� ���۸� ������Ʈ ���� ���ʽÿ�.
	// ���� �ùٸ� �Ű� ������ �ֽ��ϴ�.
	if ((m_PositionX == m_PreviousPosX) && (m_PositionY == m_PreviousPosY))
	{
		return;
	}

	// ����� ��� �������Ǵ� ��ġ�� ������Ʈ�մϴ�.
	m_PreviousPosX = m_PositionX;
	m_PreviousPosY = m_PositionY;

	// ��Ʈ �� ������ ȭ�� ��ǥ�� ����մϴ�.
	left = (float)((m_ScreenWidth / 2) * -1) + (float)m_PositionX;

	// ��Ʈ �� �������� ȭ�� ��ǥ�� ����մϴ�.
	right = left + (float)m_BitmapWidth;

	// ��Ʈ �� ����� ȭ�� ��ǥ�� ����մϴ�.
	top = (float)(m_ScreenHeight / 2) - (float)m_PositionY;

	// ��Ʈ �� �Ʒ����� ȭ�� ��ǥ�� ����մϴ�.
	bottom = top - (float)m_BitmapHeight;

	// ���� �迭�� ����ϴ�.
	vertices = new VertexInputLayout::PosTexture[m_VertexCount];

	// ���� �迭�� �����͸��ε��մϴ�.
	// ù ��° �ﰢ��
	vertices[0].Pos = XMFLOAT3(left, top, 0.0f);  // Top left.
	vertices[0].Texture = XMFLOAT2(0.0f, 0.0f);

	vertices[1].Pos = XMFLOAT3(right, bottom, 0.0f);  // Bottom right.
	vertices[1].Texture = XMFLOAT2(1.0f, 1.0f);

	vertices[2].Pos = XMFLOAT3(left, bottom, 0.0f);  // Bottom left.
	vertices[2].Texture = XMFLOAT2(0.0f, 1.0f);

	// �� ��° �ﰢ��
	vertices[3].Pos = XMFLOAT3(left, top, 0.0f);  // Top left.
	vertices[3].Texture = XMFLOAT2(0.0f, 0.0f);

	vertices[4].Pos = XMFLOAT3(right, top, 0.0f);  // Top right.
	vertices[4].Texture = XMFLOAT2(1.0f, 0.0f);

	vertices[5].Pos = XMFLOAT3(right, bottom, 0.0f);  // Bottom right.
	vertices[5].Texture = XMFLOAT2(1.0f, 1.0f);

	// ���ؽ� ���۸� �� �� �ֵ��� ��޴ϴ�.
	result = m_pDirectX->GetD3dImmediateContext()->Map(m_VB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	// ���� ������ �����͸� ����Ű�� �����͸� ��´�.
	verticesPtr = (VertexInputLayout::PosTexture*)mappedResource.pData;

	// �����͸� ���� ���ۿ� �����մϴ�.
	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexInputLayout::PosTexture) * m_VertexCount));

	// ���� ������ ����� �����մϴ�.
	m_pDirectX->GetD3dImmediateContext()->Unmap(m_VB, 0);

	// �� �̻� �ʿ����� ���� ������ �迭�� �����մϴ�.
	delete[] vertices;
	vertices = 0;
}

void BitmapObject::Render()
{
	// ��� 2D �������� �����Ϸ��� Z ���۸� ���ϴ�.
	m_pDirectX->TurnZBufferOff();

	m_pDirectX->GetD3dImmediateContext()->IASetInputLayout(InputLayouts::PosTexture);
	m_pDirectX->GetD3dImmediateContext()->IASetPrimitiveTopology(mTopology);

	// ���� ������ ������ �������� �����մϴ�.
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

	// ��� 2D �������� �Ϸ�Ǿ����Ƿ� Z ���۸� �ٽ� �ѽʽÿ�.
	m_pDirectX->TurnZBufferOn();
}

void BitmapObject::SetBitmapPosition(int x, int y)
{
	m_PositionX = x;
	m_PositionY = y;
}

void BitmapObject::CheckUIPick(int x, int y)
{
	// ���콺��ǥ x,y

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
