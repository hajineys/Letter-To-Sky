/// <summary>
/// ConstValue.h
/// 
/// ���� ������ ���̴� ��� ������ ��Ƴ��� ��
/// 2021. 08. 22 Hacgeum
/// </summary>
#pragma once
#include "../DXTK/Inc/SimpleMath.h"
using namespace DirectX;

/// <summary>
/// ����, ���ӽ����̽��� Ȱ��
/// </summary>
namespace NConstValue
{
	float XAxisBoxColliderPosMargin = 8.0f;
	float YAxisBoxColliderPosMargin = 1.5f;
}

/// <summary>
/// Ŭ������ static ������ ���� ���
/// </summary>
class CConstValue
{
public:
	static float YCharacterOffset;
	static float FallingTime;
};

float CConstValue::YCharacterOffset = 2.8f;
float CConstValue::FallingTime = 1.5f;

/// <summary>
/// ������ C++������ ������ ���� ����
/// </summary>
class ConstValue2
{
public:
	static ConstValue2* GetInstance() 
	{
		if (m_Instance == nullptr)
		{
			m_Instance = new ConstValue2();
		}

		return m_Instance; 
	}

	const float LeftBoxColliderPos = -8.0f;


private:
	static ConstValue2* m_Instance;
};

ConstValue2* ConstValue2::m_Instance = nullptr;