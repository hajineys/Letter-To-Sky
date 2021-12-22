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
/// 원본 정점 자료들의 정의를 모운 클래스
/// 
/// 기존의 Object 클래스안에서 했던 BuildGeometry 함수의 정의 위치를 옮겼다
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

	// 오브젝트의 타입과 연관이 있는 자료구조에 넣는다.
	// 여기가 원본이다.
	std::unordered_map<eDXObjectType, SGeoBuffer*> m_GeoBuffers;

	// VertexColor Object 중 클래스 내부의 함수를 가져와야 하기때문에 포인터 변수로 추가
	Waves* m_pWaves;
	Hills* m_pHills;
	TexHills* m_pTexHill;
};