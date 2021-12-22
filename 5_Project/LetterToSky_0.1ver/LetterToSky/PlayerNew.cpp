#include "PlayerNew.h"
#include "DXFactory.h"
#include "TutorialInGameScene.h"
#include "MainInGameScene.h"
#include "GameTimer.h"
#include "IDXObjectBase.h"
#include "BoxCollider.h"
#include "Transform.h"
#include "MathHelper.h"
#include "Stair.h"

#include "SoundManager.h"

PlayerNew::PlayerNew(DXDevice* pDevice, DXFactory* pFactory)
	: m_pTimer(nullptr),
	m_MoveState(ePlayerMoveState::Stopped),
	m_GameState(ePlayerGameState::StandBy),
	m_Direction(eMoveDirection::Neutral),
	m_pNowSavePoint(nullptr),
	m_TextState(eTextState::None),
	m_FallValue(0.0f),
	m_YPosDirty(false),
	m_CheckFallTime(0.0f),
	m_KeyInputDelayTime(0.0f),
	m_Per(0.0f),
	m_pFootStep(nullptr),
	m_pGameScene(nullptr),
	m_DebugOn(false)
{
	m_pPlayerList.push_back(pFactory->CreateASEObject((LPSTR)"../Data/ASE/Angel_mail_girl_idle.ASE", L"../Data/Textures/idle_512.dds"));
	m_pPlayerList.push_back(pFactory->CreateASEObject((LPSTR)"../Data/ASE/Angel_mail_girl_jump.ASE", L"../Data/Textures/jump_512.dds"));
	m_pPlayerList.push_back(pFactory->CreateASEObject((LPSTR)"../Data/ASE/Angel_mail_girl_fall.ASE", L"../Data/Textures/fall_512.dds"));
	m_pPlayerList.push_back(pFactory->CreateASEObject((LPSTR)"../Data/ASE/Angel_mail_girl_damage.ASE", L"../Data/Textures/damage_512.dds"));
	m_pPlayerList.push_back(pFactory->CreateASEObject((LPSTR)"../Data/ASE/Angel_mail_girl_clear.ASE", L"../Data/Textures/clear_512.dds"));

	m_pFootStep = new BoxCollider(pDevice);
	m_pFootStep->Initialize(BOXCOLLIDER_WIDTH, BOXCOLLIDER_HEIGHT, BOXCOLLIDER_DEPTH);
}

PlayerNew::~PlayerNew()
{

}

void PlayerNew::Initialize(float posX, float posY, float posZ, IScene* pGameScene)
{
	m_pGameScene = pGameScene;

	for (UINT i = 0; i < m_pPlayerList.size(); i++)
	{
		m_pPlayerList[i]->SetPosition(posX, posY, posZ);
		m_pPlayerList[i]->SetBoxColliderScale(BOXCOLLIDER_WIDTH, BOXCOLLIDER_HEIGHT, BOXCOLLIDER_DEPTH);
	}
}

void PlayerNew::Update(GameTimer* pTimer, D3Camera* pCamera)
{
	m_pTimer = pTimer;

	for (UINT i = 0; i < m_pPlayerList.size(); i++)
	{
		m_pPlayerList[i]->Update(pTimer, pCamera);
		m_pPlayerList[i]->GetBoxCollider()->Update(pCamera);

		Vector3 _nowPlayerPos = m_pPlayerList[i]->GetPosition();

		// 본체의 충돌감지영역
		m_pPlayerList[i]->GetBoxCollider()->SetPosition(_nowPlayerPos.x, _nowPlayerPos.y + Y_EVECTOR, _nowPlayerPos.z);
	}

	m_pFootStep->Update(pCamera);

	if (GetAsyncKeyState(VK_F5) & 0x0001)
	{
		m_DebugOn ^= true;
	}
}

void PlayerNew::Render()
{
	if (m_DebugOn)
	{
		m_pFootStep->Render();
	}

	/// (가장 낮은 레이어) 게임 상태를 체크
	switch (m_GameState)
	{
		case ePlayerGameState::StandBy:
		{
			if (m_MoveState == ePlayerMoveState::Moving)
			{
				m_pPlayerList[(int)eAnimationState::Jump]->Render();
			}
			else
			{
				m_pPlayerList[(int)eAnimationState::Idle]->Render();
			}
		}
		break;

		case ePlayerGameState::OnHit:
		{
			m_pPlayerList[(int)eAnimationState::Damage]->Render();
		}
		return;

		case ePlayerGameState::Dying:
		{
			// 죽음 상태는 최우선 처리
			m_pPlayerList[(int)eAnimationState::Fall]->SetRotation('Y', FORWARD);
			m_pPlayerList[(int)eAnimationState::Fall]->Render();
		}
		return;

		case ePlayerGameState::Clear:
		{
			m_pPlayerList[(int)eAnimationState::Clear]->Render();
		}
		break;
	}
}

void PlayerNew::Finalize()
{
	m_pPlayerList.clear();
}

/// <summary>
/// 1. 키 입력을 받는다.
/// 2. 캐릭터의 포지션 셋팅도 한다.
/// 3. 캐릭터의 회전값도 만들어서 적용시켜준다.
/// 4. 그에 따른 충돌감지용 컬라이더도 (위치를) 만들어준다.
/// </summary>
void PlayerNew::KeyInput(float dTime)
{
	// 텍스트를 보고있을땐 움직이지 못한다
	if (m_TextState == eTextState::TextOn)
	{
		return;
	}

	// 죽음 상태일때는 키 입력을 받지 않는다.
	if (m_GameState == ePlayerGameState::Dying)
	{
		return;
	}

	// 이동 중 일때는 키 입력을 받지 않는다
	if (m_MoveState == ePlayerMoveState::Moving)
	{
		return;
	}

	if (m_GameState == ePlayerGameState::Clear)
	{
		return;
	}

	/// 키 입력을 받는다.
	// 왼쪽 키를 눌렀을 때 한 번 호출한다.
	if (GetAsyncKeyState(VK_LEFT) & 0x0001)
	{
		TutorialInGameScene::m_CharacterMoveStart = true;

		Left();
		m_MoveState = ePlayerMoveState::Moving;
		m_Origin = GetPosition();
		m_Destination = GetPosition() + Vector3(X_INVERSEVECTOR, 0, 0);
		ApplyStairYPos();
		SetFootStepCollider(m_Direction);
	}
	else if (GetAsyncKeyState(VK_RIGHT) & 0x0001)
	{
		TutorialInGameScene::m_CharacterMoveStart = true;

		Right();
		m_MoveState = ePlayerMoveState::Moving;
		m_Origin = GetPosition();
		m_Destination = GetPosition() + Vector3(X_VECTOR, 0, 0);
		ApplyStairYPos();
		SetFootStepCollider(m_Direction);
	}
	else if (GetAsyncKeyState(VK_UP) & 0x0001)
	{
		TutorialInGameScene::m_CharacterMoveStart = true;

		// 앞을 봐라
		Forward();
		m_MoveState = ePlayerMoveState::Moving;
		m_Origin = GetPosition();
		m_Destination = GetPosition() + Vector3(0, 0, X_VECTOR);
		ApplyStairYPos();
		SetFootStepCollider(m_Direction);
	}
	else if (GetAsyncKeyState(VK_DOWN) & 0x0001)
	{
		TutorialInGameScene::m_CharacterMoveStart = true;

		Backward();
		m_MoveState = ePlayerMoveState::Moving;
		m_Origin = GetPosition();
		m_Destination = GetPosition() + Vector3(0, 0, X_INVERSEVECTOR);
		ApplyStairYPos();
		SetFootStepCollider(m_Direction);
	}
}

/// 각 상태에 따라서 뭔가 움직임을 만들어주는 부분
void PlayerNew::ProcessMove(float dTime)
{
	switch (m_GameState)
	{
		// 이동 처리는 대기 상태에서만
		case ePlayerGameState::StandBy:
		{
			// 이동 중 상태라면 그것을 실행
			if (m_MoveState == ePlayerMoveState::Moving)
			{
				SoundManager::GetInstance()->Play(0, "Character_Move");

				// 이동 처리
				Vector3 _nowPos = Vector3::Zero;

				// per (0 ~ 1), 시간을 누적시켜 퍼센트를 증가시킨다.
				m_Per += dTime * 3;

				// ..... -> 퍼센트가 1(100%)이 넘으면 안된다.
				if (m_Per > 1.0f)
				{
					m_Per = 1.0f;
				}

				// Lerp -> Start Position, Target Position, per
				_nowPos = MathHelper::Lerp<Vector3>(m_Origin, m_Destination, m_Per);

				// 유사 포물선 by HG
				_nowPos.y = CalcJumpYPos(m_Per, m_Origin.y, m_Destination.y + 4.0f, m_Destination.y);

				for (UINT i = 0; i < m_pPlayerList.size(); i++)
				{
					m_pPlayerList[i]->SetPosition(_nowPos.x, _nowPos.y, _nowPos.z);
					m_pPlayerList[i]->GetBoxCollider()->SetPosition(_nowPos.x, _nowPos.y, _nowPos.z);
				}

				// per가 1이 넘었으므로 Lerp 중단한다.
				if (m_Per >= 1.0f)
				{
					// y값을 보정하라는 플래그
					m_YPosDirty = true;

					m_MoveState = ePlayerMoveState::Stopped;
					m_Origin = m_Destination;
					m_Per = 0;
					SoundManager::GetInstance()->Stop(0, "Character_Move");
				}
			}
			else
			{
				// 그냥 그 위치에서 대기 (또는 외부에서 뭔가 이동 처리를 해 준다)
			}
		}
		break;

		case ePlayerGameState::OnHit:
			break;

		case ePlayerGameState::Dying:
		{
			Fall();
		}
		break;

		case ePlayerGameState::Clear:
			break;

		default:
			break;
	}
}

void PlayerNew::Move(Vector3 dXZVec)
{
	Vector3 _nowPosition = m_pPlayerList[0]->GetPosition();
	_nowPosition += dXZVec;

	// 결과 포지션을 가지고서 y값을 만들어줘야 한다.
	for (UINT i = 0; i < m_pPlayerList.size(); i++)
	{
		m_pPlayerList[i]->SetPosition(_nowPosition.x, _nowPosition.y, _nowPosition.z);
	}

	// y값 보정을 하라고 하는 플래그
	m_YPosDirty = true;
}

/// <summary>
/// 떨어지는 상태. 내부적으로 deltaTime을 받으며 (중력에 의해서) 떨어진다.
/// 너무 많이 떨어지면 불쌍하니까 한도를 두자...
/// </summary>
void PlayerNew::Fall()
{
	SoundManager::GetInstance()->Play(0, "Character_Fall");

	m_FallValue = -15.0f;

	m_CheckFallTime += m_pTimer->DeltaTime();

	m_pPlayerList[0]->Fall(m_pTimer->DeltaTime());
	m_pPlayerList[2]->Fall(m_pTimer->DeltaTime());

	if (m_pPlayerList[2]->GetPosition().y < m_FallValue)
	{
		this->ResetBySavePoint();
	}
}

void PlayerNew::Strafe(float start, float end, float value, bool forward)
{
	if (forward == true)
	{
		m_pPlayerList[0]->Strafe(start, end, value);
	}
	else
	{
		m_pPlayerList[0]->StrafeInverse(start, end, value);
	}
}

// 유사 포물선 by HG
float PlayerNew::CalcJumpYPos(float per, float a, float b, float c)
{
	float _resultYPos = 0;

	if (0 <= per && per < 0.5f)
	{
		_resultYPos = MathHelper::Lerp<float>(a, b, per);
	}
	else if (0.5f <= per && per <= 1.0f)
	{
		_resultYPos = MathHelper::Lerp<float>(b, c, per);
	}

	return _resultYPos;
}

void PlayerNew::SetFootStepCollider(eMoveDirection direction)
{
	Vector3 _nowPos = GetPosition();

	switch (direction)
	{
		case eMoveDirection::Forward:
		{
			m_pFootStep->SetPosition(_nowPos.x, _nowPos.y + 6.5f, _nowPos.z + X_VECTOR);
		}
		break;

		case eMoveDirection::Backward:
		{
			m_pFootStep->SetPosition(_nowPos.x, _nowPos.y + Y_EVECTOR, _nowPos.z + X_INVERSEVECTOR);
		}
		break;

		case eMoveDirection::Left:
		{
			m_pFootStep->SetPosition(_nowPos.x + X_INVERSEVECTOR, _nowPos.y + Y_EVECTOR, _nowPos.z);
		}
		break;

		case eMoveDirection::Right:
		{
			m_pFootStep->SetPosition(_nowPos.x + X_VECTOR, _nowPos.y + Y_EVECTOR, _nowPos.z);
		}
		break;

		case eMoveDirection::Neutral:
		{
			m_pFootStep->SetPosition(_nowPos.x, _nowPos.y + Y_EVECTOR, _nowPos.z);
		}
		break;
	}
}

void PlayerNew::ApplyStairYPos()
{
	BoxCollider* _pStair = m_pGameScene->GetFootStepStair();

	if (_pStair != nullptr)
	{
		m_Destination.y = m_pGameScene->GetFootStepStair()->GetPosition().y + Y_EVECTOR;
	}
}

/// <summary>
/// 마지막으로 기록된 세이브포인터에 따라서 위치를 리셋해준다.
/// </summary>
void PlayerNew::ResetBySavePoint()
{
	SoundManager::GetInstance()->Stop(0, "Character_Fall");

	Vector3 _resetPosition = m_pNowSavePoint->m_Position;

	m_pPlayerList[0]->SetPosition(_resetPosition.x, _resetPosition.y, _resetPosition.z);
	m_pPlayerList[0]->GetBoxCollider()->SetPosition(_resetPosition.x, _resetPosition.y + Y_EVECTOR, _resetPosition.z);

	m_GameState = ePlayerGameState::StandBy;
	m_pPlayerList[0]->SetRotation('Y', BACKWARD);
	m_CheckFallTime = 0.0f;		// 떨어진 시간 측정 초기화
	m_Destination = Vector3(0.0f, 0.0f, 0.0f);
	m_FallValue = 0.0f;
}

DirectX::SimpleMath::Vector3 PlayerNew::GetPosition()
{
	return m_pPlayerList[0]->GetPosition();
}

// 포지션을 일괄 전달한다.
void PlayerNew::SetPosition(Vector3 pos)
{
	for (UINT i = 0; i < m_pPlayerList.size(); i++)
	{
		m_pPlayerList[i]->SetPosition(pos.x, pos.y, pos.z);
	}
}

/// <summary>
/// 공중에 떴는데, 죽어야 하는가 아닌가 체크
/// </summary>
void PlayerNew::OnAirCheck()
{
	if (m_MoveState == ePlayerMoveState::Moving)
	{
		// 공중에 점프한 상태이므로 공중이라도 죽음으로 가면 안됨

	}
	else //if (m_MoveState == ePlayerMoveState::Stopped)
	{
		// 공중에서 멈춰 선 것. 발판이 없다는 것이므로 죽어야 한다.
		m_GameState = ePlayerGameState::Dying;
	}
}

void PlayerNew::Forward()
{
	// 위치, 회전 값 적용 (모든 애니메이션별 상태에 대해서)
	for (UINT i = 0; i < m_pPlayerList.size(); i++)
	{
		// 방향은 앞을 보세요
		m_pPlayerList[i]->SetRotation('Y', FORWARD);
	}

	// 앞으로 이동하자
	m_Direction = eMoveDirection::Forward;
}

void PlayerNew::Backward()
{
	// 위치, 회전 값 적용 (모든 애니메이션별 상태에 대해서)
	for (UINT i = 0; i < m_pPlayerList.size(); i++)
	{
		// 방향은 앞을 보세요
		m_pPlayerList[i]->SetRotation('Y', BACKWARD);
	}

	// 앞으로 이동하자
	m_Direction = eMoveDirection::Backward;
}

void PlayerNew::Left()
{
	// 위치, 회전 값 적용 (모든 애니메이션별 상태에 대해서)
	for (UINT i = 0; i < m_pPlayerList.size(); i++)
	{
		// 방향은 앞을 보세요
		m_pPlayerList[i]->SetRotation('Y', LEFT);
	}

	// 앞으로 이동하자
	m_Direction = eMoveDirection::Left;
}

void PlayerNew::Right()
{
	// 위치, 회전 값 적용 (모든 애니메이션별 상태에 대해서)
	for (UINT i = 0; i < m_pPlayerList.size(); i++)
	{
		// 방향은 앞을 보세요
		m_pPlayerList[i]->SetRotation('Y', RIGHT);
	}

	// 앞으로 이동하자
	m_Direction = eMoveDirection::Right;
}

std::wstring PlayerNew::GetMoveState_Debug()
{
	switch (m_MoveState)
	{
		case ePlayerMoveState::Stopped:
			return L"Stopped";

		case ePlayerMoveState::Moving:
			return L"Moving";

		default:
			return L"--";
	}
}

std::wstring PlayerNew::GetGameState_Debug()
{
	switch (m_GameState)
	{
		case ePlayerGameState::StandBy:
			return L"Live";

		case ePlayerGameState::OnHit:
			return L"OnHit";

		case ePlayerGameState::Dying:
			return L"Dying";

		case ePlayerGameState::Clear:
			return L"Clear";

		default:
			return L"--";
	}
}

std::wstring PlayerNew::GetDirection_Debug()
{
	switch (m_Direction)
	{
		case eMoveDirection::Neutral:
			return L"Neutral";

		case eMoveDirection::Forward:
			return L"Forward";

		case eMoveDirection::Left:
			return L"Left";

		case eMoveDirection::Right:
			return L"Right";

		case eMoveDirection::Backward:
			return L"Backward";

		default:
			return L"--";
	}
}

bool PlayerNew::IsYPosNeedRefresh()
{
	return m_YPosDirty;
}

void PlayerNew::YPosCompleted()
{
	m_YPosDirty = false;
}