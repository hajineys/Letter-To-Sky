/// <summary>
/// ConstValue.h
/// 
/// 게임 내에서 쓰이는 상수 값들을 모아놓은 곳
/// 2021. 08. 22 Hacgeum
/// </summary>
#pragma once
#include "../DXTK/Inc/SimpleMath.h"
using namespace DirectX;

/// <summary>
/// 전역, 네임스페이스를 활용
/// </summary>
namespace NConstValue
{
	float XAxisBoxColliderPosMargin = 8.0f;
	float YAxisBoxColliderPosMargin = 1.5f;
}

/// <summary>
/// 클래스의 static 문법을 쓰는 방식
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
/// 진보된 C++형태의 구조를 위한 구조
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