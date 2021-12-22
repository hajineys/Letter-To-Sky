#pragma once
#include <vector>
#include <unordered_map>
#include "DirectXStruct.h"

class DXDevice;
class DXGeometryBuilder;
class CASEParser;
class IDXObjectBase;

/// <summary>
/// DirectX Object���� �������ִ� Factory
/// 
/// 2021. 07. 15 Hacgeum
/// </summary>
class DXFactory
{
public:
	DXFactory(DXDevice* pDirectX);
	~DXFactory();

	IDXObjectBase* CreateHelperObject(eDXHelperObject object);
	IDXObjectBase* CreateVertexColorObject(eDXVertexColorObject object);
	IDXObjectBase* CreateTextureObject(eDXTextureObject object, const wchar_t* fileName);
	IDXObjectBase* CreateASEObject(LPSTR pFile, const wchar_t* textureFile);
	IDXObjectBase* CreateBitmapObject(int bitmapWidth, int bitmapHeight, const wchar_t* textureFile);

	// C++�߹�, ��� enum ���
	//IDXObject* CreateObject(eDXObjectType type);	// ���ο� DX������Ʈ�� �����, �δ� ����� �ִ�.

	// C++, ���ø� ���
	//template<typename T>
	//IDXObject* CreateObject(T type);

	void Finalize();

	/// �ӽ�
public:
	/// Helper Object�� ���� �����ϴ� ����
	/// 1. �Ϲ����� ������Ʈ�� �ƴ϶� ���� ���� ���������� �׸��ų�, �ȱ׸��ų��� �ؾ� �ϱ� ����
	/// 2. ���İ��� ������
	//std::vector<IDXObject*> m_pHelperObjects;
	//IDXObject* m_pHelperObject[(int)LineList::Max];
	//std::unordered_map<int, IDXObject*> m_pHelperObjectPool;

private:
	DXDevice* m_pDirectX;
	DXGeometryBuilder* m_pDXGeometry;

	// ASE Parser
	CASEParser* m_pASEParser;
};