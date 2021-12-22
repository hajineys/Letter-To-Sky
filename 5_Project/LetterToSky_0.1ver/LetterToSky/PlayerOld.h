#pragma once
#include <vector>
using namespace std;

#include "../DXTK/Inc/SimpleMath.h"
using namespace DirectX;

enum class ePlayerState
{
	IDLE,
	JUMP,
	FALL,
	DAMAGE,
	CLEAR
};

enum class eResetPoint
{
	BASE,
	SAVEPOINT01,
	SAVEPOINT02
};

class DXFactory;
class DXDevice;
class GameTimer;
class D3Camera;
class Stair;
class IDXObjectBase;
class BoxCollider;

/// <summary>
/// 유저가 조작하는 플레이어 캐릭터
/// 
/// 2021. 08. 15 Hacgeum
/// </summary>
class PlayerOld
{
public:
	PlayerOld(DXFactory* pFactory, DXDevice* pDevice);
	~PlayerOld();

	void Initialize(float posX, float posY, float posZ);
	void Update(GameTimer* pTimer, D3Camera* pCamera);
	void Update2(GameTimer* pTimer, D3Camera* pCamera);
	void Render();
	void KeyInput(float dTime);


	/// Player FSM
	void PlayerFSM();

	void Fall();
	void Reset();

	void GetStairMoveToLeft(Stair* pStair);
	void GetStairMoveToRight(Stair* pStair);
	void GetStairMoveToUp(Stair* pStair);
	void GetStairMoveToDown(Stair* pStair);

	void Finalize();

	// Getter / Setter
	void SetState(ePlayerState state) { m_State = state; }
	void SetResetPoint(eResetPoint point) { m_ResetPoint = point; }

	void SetResetPosition(float posX, float posY, float posZ);
	void SetResetSavePoint01Position(float posX, float posY, float posZ);
	void SetResetSavePoint02Position(float posX, float posY, float posZ);

	BoxCollider* GetMoveToLeft() const { return m_pMoveToLeft; }
	BoxCollider* GetMoveToRight() const { return m_pMoveToRight; }
	BoxCollider* GetMoveToUp() const { return m_pMoveToUp; }
	BoxCollider* GetMoveToDown() const { return m_pMoveToDown; }

public:
	vector<IDXObjectBase*> m_pPlayerList;

private:
	GameTimer* m_pTimer;
	//IDXObjectBase* m_pPlayerObject;

	ePlayerState m_State;
	eResetPoint m_ResetPoint;

	XMFLOAT3 m_ResetPosition;
	XMFLOAT3 m_ResetSavePoint01Position;
	XMFLOAT3 m_ResetSavePoint02Position;

	BoxCollider* m_pMoveToLeft;
	BoxCollider* m_pMoveToRight;
	BoxCollider* m_pMoveToUp;
	BoxCollider* m_pMoveToDown;

	XMFLOAT3 m_MoveToLeft;
	XMFLOAT3 m_MoveToRight;
	XMFLOAT3 m_MoveToUp;
	XMFLOAT3 m_MoveToDown;
};