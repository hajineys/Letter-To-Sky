#pragma once
#include "DirectXDefine.h"
#include "DXStruct.h"
#include <unordered_map>

enum LineList
{
	AXIS, GRID,
	Max,
};

struct SGeoBuffer;

/// <summary>
/// 원본 정점 자료들의 정의를 모운 클래스
/// 
/// 기존의 Object 클래스에서 하는 BuildGeometry 함수의 정의 위치를 
/// 2021. 07. 16
/// </summary>
//class DXGeometryBuilder : public IFactory
class DXGeometry
{
public:
	DXGeometry(ID3D11Device* pDevice);
	~DXGeometry();

	void BuildAxisGeometry();
	void BuildGridGeometry();



public:
	vector<ID3D11Buffer*> m_VBVector;
	vector<ID3D11Buffer*> m_IBVector;
	vector<UINT> m_IndexCountVector;

private:
	ID3D11Device* m_pD3dDevice;

	ID3D11Buffer* mVB;
	ID3D11Buffer* mIB;
	UINT mIndexCount;



public:
	/// 새로 만듦. 아무래도 이 클래스는 Geometry Builder이다.
	SGeoBuffer* BuildAxisGeometryBuffers();
	SGeoBuffer* BuildGridGeometryBuffers();


private:
	//std::unordered_map<int, SGeoBuffer*> m_GeoBuffers;
	//std::unordered_map<std::string, SGeoBuffer*> m_GeoBuffers;

	// 오브젝트의 타입과 연관이 있는 자료구조에 넣는다.
	// 여기가 원본이다.
	std::unordered_map<eDXObjectType, SGeoBuffer*> m_GeoBuffers;
};
