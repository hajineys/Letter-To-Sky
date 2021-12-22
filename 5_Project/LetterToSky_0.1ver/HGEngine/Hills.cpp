#include "Hills.h"
#include "DirectXStruct.h"

Hills::Hills(DXDevice* pDevice)
	: VertexColorObject(pDevice)
{

}

Hills::~Hills()
{

}

void Hills::LoadGeometry(SGeoBuffer* geoBuffer)
{
	// °ª º¹»ç
	m_VB = geoBuffer->m_VB;
	m_IB = geoBuffer->m_IB;
	m_IndexCount = geoBuffer->m_IndexCount;
}

float Hills::GetHeight(float x, float z)
{
	return 0.3f * (z * sinf(0.1f * x) + x * cosf(0.1f * z));
}
