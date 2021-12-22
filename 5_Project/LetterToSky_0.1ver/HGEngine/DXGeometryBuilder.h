#pragma once
#include <unordered_map>
#include "DirectXDefine.h"
#include "DirectXStruct.h"

class DXDevice;
struct SGeoBuffer;

class Waves;
class Hills;
class TexHills;

/// <summary>
/// ���� ���� �ڷ���� ���Ǹ� ��� Ŭ����
/// 
/// ������ Object Ŭ�����ȿ��� �ߴ� BuildGeometry �Լ��� ���� ��ġ�� �Ű��
/// 2021. 07. 16 Hacgeum
/// </summary>
//class DXGeometryBuilder : public IFactory
class DXGeometryBuilder
{
public:
	DXGeometryBuilder(DXDevice* pDevice);
	~DXGeometryBuilder();

	// Helper Object
	SGeoBuffer* BuildGridGeometryBuffers();
	SGeoBuffer* BuildAxisGeometryBuffers();

	// VertexColor Object
	SGeoBuffer* BuildBoxGeometryBuffers();
	SGeoBuffer* BuildCarGeometryBuffers();
	SGeoBuffer* BuildSkullsGeometryBuffers();
	SGeoBuffer* BuildWavesGeometryBuffers();
	SGeoBuffer* BuildHillsGeometryBuffers();
	SGeoBuffer* BuildMikeWazowskisGeometryBuffers();

	// Texture Object
	SGeoBuffer* BuildTexBoxGeometryBuffers();
	SGeoBuffer* BuuildTexHillGeometryBuffers();

	// Bitmap
	SGeoBuffer* BuildBitmapGeometryBuffers();

private:
	ID3D11Device* m_pD3dDevice;

	//std::unordered_map<int, SGeoBuffer*> m_GeoBuffers;
	//std::unordered_map<std::string, SGeoBuffer*> m_GeoBuffers;

	// ������Ʈ�� Ÿ�԰� ������ �ִ� �ڷᱸ���� �ִ´�.
	// ���Ⱑ �����̴�.
	std::unordered_map<eDXObjectType, SGeoBuffer*> m_GeoBuffers;

	// VertexColor Object �� Ŭ���� ������ �Լ��� �����;� �ϱ⶧���� ������ ������ �߰�
	Waves* m_pWaves;
	Hills* m_pHills;
	TexHills* m_pTexHill;
};