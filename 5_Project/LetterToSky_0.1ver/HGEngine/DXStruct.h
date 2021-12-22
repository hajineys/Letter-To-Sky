#pragma once
#include "DirectXDefine.h"

enum class eDXObjectType
{
	HelperObject,
	LineListObject,
	MeshObject,
};

/// <summary>
/// POD����
/// Plain old Data
/// https://en.wikipedia.org/wiki/Passive_data_structure
/// </summary>
struct SGeoBuffer
{
public:
	ID3D11Device* m_pD3dDevice;

	ID3D11Buffer* mVB;
	ID3D11Buffer* mIB;
	UINT mIndexCount;
};

/// <summary>
/// Ŭ���� ����
/// </summary>
class CGeoBuffer
{
public:
	CGeoBuffer() {}
	~CGeoBuffer() {}

private:
	ID3D11Device* m_pD3dDevice;

	ID3D11Buffer* mVB;
	ID3D11Buffer* mIB;
	UINT mIndexCount;
};