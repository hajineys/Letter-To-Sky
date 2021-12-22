#pragma once
#include "../DXTK/Inc/SimpleMath.h"
using namespace DirectX;
using namespace DirectX::SimpleMath;

enum class eStairType
{
	Start_Base,
	Cloud_Base,
	End_Base,
	Area01_Normal,
	Area01_Broken,
	Area02_Normal,
	Area02_Broken,
	Area03_Normal,
	Area03_Broken,
	Cloud
};

enum class eStairState
{
	Base,
	Normal,
	Move,
	Fall,
	Strafe,
	Strafe_Inverse,
	Fall_Strafe,
	UpDown,
	FrontBack,
	Diagonal
};

class DXFactory;
class GameTimer;
class D3Camera;
class PlayerOld;
class PlayerNew;
class BoxCollider;
class IDXObjectBase;

/// <summary>
/// 플레이어 캐릭터가 밟는 계단 클래스
/// 
/// 2021. 08. 15 Hacgeum
/// </summary>
class Stair
{
public:
	Stair(DXFactory* pFactory, eStairType type, eStairState state);
	~Stair();

	void Initialize(float posX, float posY, float posZ);
	void Update(GameTimer* pTimer, D3Camera* pCamera, PlayerNew* pPlayer);
	void Render();

	// 기믹
	void Move(Vector3 pos, BoxCollider* pCollider, int number, PlayerNew* pPlayer);
	void Fall(float y, PlayerNew* pPlayer);
	void Strafe(float start, float end, float value, bool forward, PlayerNew* pPlayer);
	void UpDown(float start, float end, float value, PlayerNew* pPlayer);
	void FrontBack(float start, float end, float value, PlayerNew* pPlayer);
	void StrafeDiagonal(float startX, float endX, float startY, float endY, float startZ, float endZ, float dTime, PlayerNew* pPlayer);
	void FallStrafe(PlayerNew* pPlayer, float y, float start, float end, float value, bool forward);
	void Save();
	void Clear();

	void Reset();
	void Finalize();

	IDXObjectBase* GetStairObject() const { return m_pStairObject; }

	eStairType GetType() const { return m_Type; }

	void GetResetPosition(float posX, float posY, float posZ);

public:
	// 시간 측정
	float m_CheckFallTime;
	float m_CloudCreateTime;

private:
	GameTimer* m_pTimer;

	IDXObjectBase* m_pStairObject;

	eStairType m_Type;
	eStairState m_State;

	XMFLOAT3 m_ResetPosition;
};