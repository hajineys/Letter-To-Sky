#pragma once
#include <vector>
#include <unordered_map>
#include "DirectXStruct.h"

class DXDevice;
class DXGeometryBuilder;
class CASEParser;
class IDXObjectBase;

/// <summary>
/// DirectX Object들을 생성해주는 Factory
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

	// C++중반, 모던 enum 사용
	//IDXObject* CreateObject(eDXObjectType type);	// 새로운 DX오브젝트가 생기면, 부대 비용이 있다.

	// C++, 템플릿 사용
	//template<typename T>
	//IDXObject* CreateObject(T type);

	void Finalize();

	/// 임시
public:
	/// Helper Object가 따로 존재하는 이유
	/// 1. 일반적인 오브젝트가 아니라서 때에 따라서 선별적으로 그리거나, 안그리거나를 해야 하기 때문
	/// 2. 선후관계 때문에
	//std::vector<IDXObject*> m_pHelperObjects;
	//IDXObject* m_pHelperObject[(int)LineList::Max];
	//std::unordered_map<int, IDXObject*> m_pHelperObjectPool;

private:
	DXDevice* m_pDirectX;
	DXGeometryBuilder* m_pDXGeometry;

	// ASE Parser
	CASEParser* m_pASEParser;
};