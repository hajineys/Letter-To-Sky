#pragma once
#include "../DXTK/Inc/SimpleMath.h"
using namespace DirectX;

enum class eBirdType
{
	BIG,
	SMALL
};

class DXFactory;
class GameTimer;
class D3Camera;
class IDXObjectBase;

/// <summary>
/// 플레이어 캐릭터의 이동을 방해하는 새 클래스
/// 
/// 2021. 08. 16 Hacgeum
/// </summary>
class Bird
{
public:
	Bird(DXFactory* pFactory, eBirdType type);
	~Bird();

	void Initialize(float posX, float posY, float posZ);
	void Update(GameTimer* pTimer, D3Camera* pCamera);
	void Render();
	void Reset();
	void Finalize();

	// Getter / Setter
	IDXObjectBase* GetBirdObject() const { return m_pBirdObject; }

	void SetResetPosition(float posX, float posY, float posZ);

private:
	IDXObjectBase* m_pBirdObject;

	eBirdType m_Type;

	XMFLOAT3 m_ResetPosition;
};