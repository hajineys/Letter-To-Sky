#pragma once
#include <map>
#include <string>

class IDXObjectBase;

/// <summary>
/// DirectX Factory���� ������ DXObject���� �����ϴ� �Ŵ���
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
