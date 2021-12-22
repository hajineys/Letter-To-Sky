#pragma once
#include <map>
#include <string>

class IDXObjectBase;

/// <summary>
/// DirectX Factory에서 생성한 DXObject들을 관리하는 매니저
/// 
/// 2021. 07. 28 Hacgeum
/// </summary>
class DXObjectManager
{
public:
	DXObjectManager();
	~DXObjectManager();

	void Finalize();

public:
	std::map<std::string, IDXObjectBase*> m_Objects;
};
