#pragma once
#include "DirectXDefine.h"

enum class eDXObjectType
{
	HELPER,
	VERTEXCOLOR,
	ASE,
};

enum class eDXHelperObject
{
	GRID,
	AXIS,
};

enum class eDXVertexColorObject
{
	BOX,
	CAR,
	SKULL,
	WAVES,
	HILLS,
	MIKEWAZOWSKI
};

enum class eDXTextureObject
{
	TBOX,
	THILLS,
};

/// <summary>
/// POD버전
/// Plain old Data
/// https://en.wikipedia.org/wiki/Passive_data_structure
/// </summary>
struct SGeoBuffer
{
public:
	ID3D11Device* m_pD3dDevice = nullptr;

	ID3D11Buffer* m_VB = nullptr;
	ID3D11Buffer* m_IB = nullptr;
	UINT m_IndexCount = 0;
	int m_VertexCount = 0;

	int m_VertexOffset = 0;
	UINT m_IndexOffset = 0;
};

/// <summary>
/// 클래스 버전
/// </summary>
class CGeoBuffer
{
public:
	CGeoBuffer()
		: m_pD3dDevice(nullptr),
		m_VB(0), m_IB(0), m_IndexCount(0), m_VertexCount(0), m_VertexOffset(0), m_IndexOffset(0)
	{

	}
	~CGeoBuffer() {}

private:
	ID3D11Device* m_pD3dDevice;

	ID3D11Buffer* m_VB;
	ID3D11Buffer* m_IB;
	UINT m_IndexCount;
	int m_VertexCount;

	int m_VertexOffset;
	UINT m_IndexOffset;
};
