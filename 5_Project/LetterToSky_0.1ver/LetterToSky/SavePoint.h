#pragma once
#include "../DXTK/Inc/SimpleMath.h"
using namespace DirectX::SimpleMath;

#include <string>

/// <summary>
/// ���̺�����Ʈ�� ����
/// 
/// 2021.08.22 LeHideOffice
/// </summary>
struct SavePoint
{
public:
	SavePoint();

	std::string m_Name;
	Vector3 m_Position;
};