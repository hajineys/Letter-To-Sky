#pragma once
#include <vector>
using namespace std;

#include "../DXTK/Inc/SimpleMath.h"
using namespace DirectX;

#include "SavePoint.h"

#define BOXCOLLIDER_WIDTH 4.5f
#define BOXCOLLIDER_HEIGHT 9.0f
#define BOXCOLLIDER_DEPTH 4.5f

#define X_VECTOR 8.0f
#define X_INVERSEVECTOR -8.0f
#define Y_EVECTOR 3.5f
#define Y_INVERSEVECTOR -3.0f

#define FORWARD 0.0f
#define BACKWARD 180.0f
#define LEFT -90.0f
#define RIGHT 90.0f

#define DELAYTIME 1.0f
#define STATECHANGETIME 1.0f;
#define FALLINGTIME 1.5f

#define Y_CHARACTER_OFFSET_STARTBASE 0.3f
#define Y_CHARACTER_OFFSET_CLOUDBASE 0.4f
#define Y_CHARACTER_OFFSET_ENDBASE 0.8f
#define Y_CHARACTER_OFFSET_STAIR 2.8f
#define Y_CHARACTER_OFFSET_CLOUD 2.8f

// �÷��̾��� �̵� ����
enum class ePlayerMoveState
{
	Stopped,				// (����) ����� ���� �� �ִ� ����. (����)
	Moving,					/// (����) ������ �ϴ� �� (�̵��ϴ� ��)
};

// �÷��̾��� ���� ���� ����
enum class ePlayerGameState
{
	StandBy,	// ��Ʈ�� �����ϰ�, �÷��� ���� ����
	OnHit,		// ���� ����
	Dying,		// �������� �����̹Ƿ� ���� �Ұ�
	Clear		// ���� Ŭ���� ����
};

// �ִϸ��̼� ����(�ϴ��� ����)
enum class eAnimationState
{
	Idle,
	Jump,
	Fall,
	Damage,
	Clear
};

enum class eMoveDirection
{
	Neutral,
	Forward,
	Left,
	Right,
	Backward
};

enum class eSavePoint
{
	Base,
	SavePoint01,
	SavePoint02,
	Cheat
};

enum class eTextState
{
	TextOn,
	TextOff,
	None
};

class DXFactory;
class DXDevice;
class GameTimer;
class D3Camera;
class Stair;
class IDXObjectBase;
class BoxCollider;

class IScene;
//class MainInGameScene;

/// <summary>
/// �� �� ������ �̵��� �ϴ� �÷��̾� ĳ����
/// 
/// 2021.08.22 LeHideOffice
/// </summary>
class PlayerNew
{
public:
	PlayerNew(DXDevice* pDevice, DXFactory* pFactory);
	~PlayerNew();

	void Initialize(float posX, float posY, float posZ, IScene* pGameScene);
	void Update(GameTimer* pTimer, D3Camera* pCamera);
	void Render();
	void Finalize();

	void KeyInput(float dTime);
	void ProcessMove(float dTime);
	void Move(Vector3 dXZVec);
	void Fall();
	void Strafe(float start, float end, float value, bool forward);

private:
	float CalcJumpYPos(float per, float a, float b, float c);
	void SetFootStepCollider(eMoveDirection direction);
	void ApplyStairYPos();

public:
	void ResetBySavePoint();

	bool IsYPosNeedRefresh();
	void YPosCompleted();

	// Getter / Setter
	Vector3 GetPosition();
	void SetPosition(Vector3 pos);

	void SetFallValue(float y) { m_FallValue = y; }

	void SetGameState(ePlayerGameState gameState) { m_GameState = gameState; }
	void SetNowSavePoint(SavePoint* savePoint) { m_pNowSavePoint = savePoint; }

	eTextState GetTextState() const { return m_TextState; }
	void SetTextState(eTextState textState) { m_TextState = textState; }

	void OnAirCheck();

	BoxCollider* GetFootStep() const { return m_pFootStep; }

private:
	void Forward();
	void Backward();
	void Left();
	void Right();

public:
	// ����׿� ������
	std::wstring GetMoveState_Debug();
	std::wstring GetGameState_Debug();
	std::wstring GetDirection_Debug();

public:
	vector<IDXObjectBase*> m_pPlayerList;

private:
	GameTimer* m_pTimer;

	ePlayerMoveState m_MoveState;		// ���鿡 �پ� �ִ°� �̵�(����)�ΰ�
	ePlayerGameState m_GameState;
	eMoveDirection m_Direction;

	// �� ĳ������ ���� ���̺�����Ʈ
	SavePoint* m_pNowSavePoint;

	// Text On / Off
	eTextState m_TextState;

	float m_FallValue;

	// y�� ������ �ؾ߸� �ϴ� �÷���
	bool m_YPosDirty;

	// �ð� ����
	float m_CheckFallTime;
	float m_KeyInputDelayTime;

	// �̵� ����
	Vector3 m_Origin;		// �����
	Vector3 m_Destination;	// ������
	float m_Per;				// �̵��� ����

	BoxCollider* m_pFootStep;

	IScene* m_pGameScene;

	bool m_DebugOn;
};