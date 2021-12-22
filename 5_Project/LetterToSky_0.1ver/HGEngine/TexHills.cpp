#include "TexHills.h"
#include "DDSTextureLoader.h"
#include "GameTimer.h"
#include "Vertex.h"
#include "GeometryGenerator.h"
#include "Effects.h"

TexHills::TexHills(DXDevice* pDevice)
	: TextureObject(pDevice)
{
	XMMATRIX grassTexScale = XMMatrixScaling(5.0f, 5.0f, 0.0f);
	m_TexTransform = grassTexScale;
}

TexHills::~TexHills()
{

}

float TexHills::GetHillHeight(float x, float z) const
{
	return 0.3f * (z * sinf(0.1f * x) + x * cosf(0.1f * z));
}

DirectX::XMFLOAT3 TexHills::GetHillNormal(float x, float z) const
{
	// n = (-df/dx, 1, -df/dz)
	XMFLOAT3 n(
		-0.03f * z * cosf(0.1f * x) - 0.3f * cosf(0.1f * z),
		1.0f,
		-0.3f * sinf(0.1f * x) + 0.03f * x * sinf(0.1f * z));

	XMVECTOR unitNormal = XMVector3Normalize(XMLoadFloat3(&n));
	XMStoreFloat3(&n, unitNormal);

	return n;
}
