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
/// ���� ���� �ڷ���� ���Ǹ� ��� Ŭ����
/// 
/// ������ Object Ŭ�������� �ϴ� BuildGeometry �Լ��� ���� ��ġ�� 
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
	/// ���� ����. �ƹ����� �� Ŭ������ Geometry Builder�̴�.
	SGeoBuffer* BuildAxisGeometryBuffers();
	SGeoBuffer* BuildGridGeometryBuffers();


private:
	//std::unordered_map<int, SGeoBuffer*> m_GeoBuffers;
	//std::unordered_map<std::string, SGeoBuffer*> m_GeoBuffers;

	// ������Ʈ�� Ÿ�԰� ������ �ִ� �ڷᱸ���� �ִ´�.
	// ���Ⱑ �����̴�.
	std::unordered_map<eDXObjectType, SGeoBuffer*> m_GeoBuffers;
};
