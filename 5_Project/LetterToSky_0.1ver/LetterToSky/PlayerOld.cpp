#include "PlayerOld.h"
#include "DXFactory.h"
#include "GameTimer.h"
#include "IDXObjectBase.h"
#include "BoxCollider.h"
#include "Transform.h"
#include "MathHelper.h"
#include "Stair.h"
//#include "ConstValue.h"

PlayerOld::PlayerOld(DXFactory* pFactory, DXDevice* pDevice)
	: m_pTimer(nullptr),
	m_State(ePlayerState::IDLE),
	m_ResetPoint(eResetPoint::BASE),
	m_ResetPosition(0.0f, 0.0f, 0.0f),
	m_ResetSavePoint01Position(0.0f, 0.0f, 0.0f),
	m_ResetSavePoint02Position(0.0f, 0.0f, 0.0f),
	m_pMoveToLeft(nullptr),
	m_pMoveToRight(nullptr),
	m_pMoveToUp(nullptr),
	m_pMoveToDown(nullptr),
	m_MoveToLeft(0.0f, 0.0f, 0.0f),
	m_MoveToRight(0.0f, 0.0f, 0.0f),
	m_MoveToUp(0.0f, 0.0f, 0.0f),
	m_MoveToDown(0.0f, 0.0f, 0.0f)
{
	m_pPlayerList.push_back(pFactory->CreateASEObject((LPSTR)"../Data/ASE/Angel_mail_girl_idle.ASE", L"../Data/Textures/Angel_mail_girl_T.dds"));
	m_pPlayerList.push_back(pFactory->CreateASEObject((LPSTR)"../Data/ASE/Angel_mail_girl_jump.ASE", L"../Data/Textures/Angel_mail_girl_T.dds"));
	m_pPlayerList.push_back(pFactory->CreateASEObject((LPSTR)"../Data/ASE/Angel_mail_girl_fall.ASE", L"../Data/Textures/Angel_mail_girl_T.dds"));
	m_pPlayerList.push_back(pFactory->CreateASEObject((LPSTR)"../Data/ASE/Angel_mail_girl_damage.ASE", L"../Data/Textures/Angel_mail_girl_T.dds"));
	m_pPlayerList.push_back(pFactory->CreateASEObject((LPSTR)"../Data/ASE/Angel_mail_girl_clear.ASE", L"../Data/Textures/Angel_mail_girl_T.dds"));

	m_pMoveToLeft = new BoxCollider(pDevice);
	m_pMoveToLeft->Initialize(4.0f, 11.0f, 4.0f);

	m_pMoveToRight = new BoxCollider(pDevice);
	m_pMoveToRight->Initialize(4.0f, 11.0f, 4.0f);

	m_pMoveToUp = new BoxCollider(pDevice);
	m_pMoveToUp->Initialize(4.0f, 11.0f, 4.0f);

	m_pMoveToDown = new BoxCollider(pDevice);
	m_pMoveToDown->Initialize(4.0f, 11.0f, 4.0f);
}

PlayerOld::~PlayerOld()
{

}

void PlayerOld::Initialize(float posX, float posY, float posZ)
{
	SetResetPosition(posX, posY, posZ);

	m_pMoveToLeft->SetPosition(posX - 8.0f, posY + 1.5f, posZ);
	//m_pMoveToLeft->SetPosition(posX - ConstValue::XAxisBoxColliderPosMargin, posY + 1.5f, posZ);

	///m_pMoveToLeft->SetPosition(posX + ConstValue1::LeftBoxColliderPos, posY + 1.5f, posZ);
	///m_pMoveToLeft->SetPosition(posX + ConstValue2::GetInstance()->LeftBoxColliderPos, posY + 1.5f, posZ);

	m_pMoveToLeft->SetPosition(posX + 8.0f, posY + 1.5f, posZ);
	//m_pMoveToRight->SetPosition(posX + ConstValue::XAxisBoxColliderPosMargin, posY + 1.5f, posZ);

	m_pMoveToUp->SetPosition(posX, posY + 1.5f, posZ + 7.7f);
	m_pMoveToDown->SetPosition(posX, posY + 1.5f, posZ - 7.7f);

	for (UINT i = 0; i < m_pPlayerList.size(); i++)
	{
		m_pPlayerList[i]->SetPosition(posX, posY, posZ);
		m_pPlayerList[i]->SetBoxColliderScale(4.0f, 8.0f, 4.0f);
	}

	m_State = ePlayerState::IDLE;
}

void PlayerOld::Update(GameTimer* pTimer, D3Camera* pCamera)
{
	m_pTimer = pTimer;
	m_pMoveToLeft->Update(pCamera);
	m_pMoveToRight->Update(pCamera);
	m_pMoveToUp->Update(pCamera);
	m_pMoveToDown->Update(pCamera);

	for (UINT i = 0; i < m_pPlayerList.size(); i++)
	{
		m_pPlayerList[i]->Update(pTimer, pCamera);
		m_pPlayerList[i]->GetBoxCollider()->Update(pCamera);

		m_pPlayerList[i]->GetBoxCollider()->SetPosition(m_pPlayerList[i]->GetPosition().x,
			m_pPlayerList[i]->GetPosition().y + 3.5f, m_pPlayerList[i]->GetPosition().z);

		m_pMoveToLeft->SetPosition(m_pPlayerList[i]->GetPosition().x - 8.0f,
			m_pPlayerList[i]->GetPosition().y + 1.5f, m_pPlayerList[i]->GetPosition().z);
		m_pMoveToRight->SetPosition(m_pPlayerList[i]->GetPosition().x + 8.0f,
			m_pPlayerList[i]->GetPosition().y + 1.5f, m_pPlayerList[i]->GetPosition().z);
		m_pMoveToUp->SetPosition(m_pPlayerList[i]->GetPosition().x,
			m_pPlayerList[i]->GetPosition().y + 1.5f, m_pPlayerList[i]->GetPosition().z + 7.7f);
		m_pMoveToDown->SetPosition(m_pPlayerList[i]->GetPosition().x,
			m_pPlayerList[i]->GetPosition().y + 1.5f, m_pPlayerList[i]->GetPosition().z - 7.7f);

		if (m_State == ePlayerState::FALL)
		{
			m_pPlayerList[(int)ePlayerState::FALL]->SetRotation('Y', 180.0f);
		}
	}
}

#pragma region _위의 업데이트를 개선한 예시. 지역변수를 잘 쓰자.
void PlayerOld::Update2(GameTimer* pTimer, D3Camera* pCamera)
{
	m_pTimer = pTimer;
	m_pMoveToLeft->Update(pCamera);
	m_pMoveToRight->Update(pCamera);
	m_pMoveToUp->Update(pCamera);
	m_pMoveToDown->Update(pCamera);

	for (UINT i = 0; i < m_pPlayerList.size(); i++)
	{
		m_pPlayerList[i]->Update(pTimer, pCamera);
		m_pPlayerList[i]->GetBoxCollider()->Update(pCamera);

		// 지역변수를 사용하여 코드를 한눈에 볼수있게 정리하자
		Vector3 _nowPlayerPos = m_pPlayerList[i]->GetPosition();

		m_pPlayerList[i]->GetBoxCollider()->SetPosition(_nowPlayerPos.x, _nowPlayerPos.y + 3.5f, _nowPlayerPos.z);
		m_pMoveToLeft->SetPosition(_nowPlayerPos.x - 8.0f, _nowPlayerPos.y + 1.5f, _nowPlayerPos.z);
		m_pMoveToRight->SetPosition(_nowPlayerPos.x + 8.0f, _nowPlayerPos.y + 1.5f, _nowPlayerPos.z);
		m_pMoveToUp->SetPosition(_nowPlayerPos.x, _nowPlayerPos.y + 1.5f, _nowPlayerPos.z + 7.7f);
		m_pMoveToDown->SetPosition(_nowPlayerPos.x, _nowPlayerPos.y + 1.5f, _nowPlayerPos.z - 7.7f);

		if (m_State == ePlayerState::FALL)
		{
			m_pPlayerList[(int)ePlayerState::FALL]->SetRotation('Y', 180.0f);
		}
	}
}
#pragma endregion


void PlayerOld::Render()
{
	m_pMoveToLeft->Render();
	m_pMoveToRight->Render();
	m_pMoveToUp->Render();
	m_pMoveToDown->Render();

	switch (m_State)
	{
	case ePlayerState::IDLE:
		m_pPlayerList[(int)ePlayerState::IDLE]->Render();
		break;

	case ePlayerState::JUMP:
		m_pPlayerList[(int)ePlayerState::JUMP]->Render();
		break;

	case ePlayerState::FALL:
		m_pPlayerList[(int)ePlayerState::FALL]->Render();
		break;

	case ePlayerState::DAMAGE:
		m_pPlayerList[(int)ePlayerState::DAMAGE]->Render();
		break;

	case ePlayerState::CLEAR:
		m_pPlayerList[(int)ePlayerState::CLEAR]->Render();
		break;
	}
}


/// <summary>
/// 1. 키 입력을 받는다.
/// 2. 캐릭터의 포지션 셋팅도 한다.
/// 3. 캐릭터의 회전값도 만들어서 적용시켜준다.
/// 4. 그에 따른 충돌감지용 컬라이더도 (위치를) 만들어준다.
/// </summary>
void PlayerOld::KeyInput(float dTime)
{
	static float per = 0.0f;
	static bool isLerping = false;

	static Vector3 startPos;
	static Vector3 targetPos;

	static Vector3 nowPos;	// 현재 진행도에 따른 현재 포지션...-> per 0% -> nowPos = startPos
							// per -> 100% (1.0) -> nowPos = targetPos

	// 왼쪽 키를 누르면, Lerp를 시작한다.
	// 왼쪽 키를 눌렀을 때 딱 한번 들어온다.
	if (GetAsyncKeyState(VK_LEFT) & 0x0001 && !isLerping)
	{
		isLerping = true;

		for (UINT i = 0; i < m_pPlayerList.size(); i++)
		{
			// 포지션 세팅, (초기화)
			startPos = m_pPlayerList[i]->GetPosition();

			targetPos = m_MoveToLeft;
			targetPos.y = m_MoveToLeft.y + 2.8f;
			m_pPlayerList[i]->SetRotation('Y', -90.0f);
		}

		per = 0.0f;
	}

	if (GetAsyncKeyState(VK_RIGHT) & 0x0001 && !isLerping)
	{
		isLerping = true;

		for (UINT i = 0; i < m_pPlayerList.size(); i++)
		{
			// 포지션 세팅, (초기화)
			startPos = m_pPlayerList[i]->GetPosition();

			targetPos = m_MoveToRight;
			targetPos.y = m_MoveToRight.y + 2.8f;
			m_pPlayerList[i]->SetRotation('Y', 90.0f);
		}

		per = 0.0f;
	}

	if (GetAsyncKeyState(VK_UP) & 0x0001 && !isLerping)
	{
		isLerping = true;

		for (UINT i = 0; i < m_pPlayerList.size(); i++)
		{
			// 포지션 세팅, (초기화)
			startPos = m_pPlayerList[i]->GetPosition();

			targetPos = m_MoveToUp;
			targetPos.y = m_MoveToUp.y + 2.8f;
			m_pPlayerList[i]->SetRotation('Y', 0.0f);
		}

		per = 0.0f;
	}

	if (GetAsyncKeyState(VK_DOWN) & 0x0001 && !isLerping)
	{
		isLerping = true;

		for (UINT i = 0; i < m_pPlayerList.size(); i++)
		{
			// 포지션 세팅, (초기화)
			startPos = m_pPlayerList[i]->GetPosition();

			targetPos = m_MoveToDown;
			targetPos.y = m_MoveToDown.y + 2.8f;
			m_pPlayerList[i]->SetRotation('Y', 180.0f);
		}

		per = 0.0f;
	}

	// 매 프레임 들어올 것이다.
	if (isLerping)
	{
		m_State = ePlayerState::JUMP;
		///Lerp();

		// per (0 ~ 1), 시간을 누적시켜 퍼센트를 증가시킨다.
		per += dTime * 5;

		// Lerp -> Start Position, Target Position, per
		nowPos = MathHelper::Lerp<Vector3>(startPos, targetPos, per);

		for (UINT i = 0; i < m_pPlayerList.size(); i++)
		{
			m_pPlayerList[i]->SetPosition(nowPos.x, nowPos.y, nowPos.z);
			m_pPlayerList[i]->GetBoxCollider()->SetPosition(nowPos.x, nowPos.y + 3.5f, nowPos.z);
		}

		m_pMoveToLeft->SetPosition(nowPos.x - 8.0f, nowPos.y + 1.5f, nowPos.z);
		m_pMoveToRight->SetPosition(nowPos.x + 8.0f, nowPos.y + 1.5f, nowPos.z);
		m_pMoveToUp->SetPosition(nowPos.x, nowPos.y + 1.5f, nowPos.z + 7.7f);
		m_pMoveToDown->SetPosition(nowPos.x, nowPos.y + 1.5f, nowPos.z - 7.7f);

		// ..... -> 퍼센트가 1(100%)이 넘으면 안된다.
		if (per > 1.0f)
		{
			per = 1.0f;
		}

		// per가 1이 넘었으므로 Lerp 중단한다.
		if (per >= 1.0f)
		{
			isLerping = false;
			m_State = ePlayerState::IDLE;
		}
	}
}

void PlayerOld::PlayerFSM()
{
	// Player의 위치에 따라 BoxCollider의 위치를 다시 Update해준다

	// Update문에서 Player의 FSM을 계속 체크한다

	switch (m_State)
	{
	case ePlayerState::IDLE:
		// 키 입력(왼쪽/오른쪽/위/아래)
		// 이동할 지점에 계단이 있는 지 없는 지 판단
		// 있다면 Jump/없다면 Fall
		break;

	case ePlayerState::JUMP:
		// 이동
		// 앞서 Idle상태에서 계단이 있다고 판단 후 Jump했으나 아무것도 없다면 다시 Fall로 상태 변경
		// 부서진 계단으로 이동했다면 0.5초뒤 Fall로 상태변경
		// Strafe등 이동형 계단으로 점프했다면 그 계단의 위치에 맞춰 포지션 변경
		break;

	case ePlayerState::FALL:
		// m_PlayerList[Fall]->Render
		// 방향은 Y, 180도(캐릭터 얼굴 보여줘야함)
		// 떨어진다, 키 입력 불가
		// 일정 Value만큼(?) 떨어지면 리셋(SavePoint지점으로)
		break;

	case ePlayerState::DAMAGE:
		// 방해물체와 거리 측정
		// 방해물체와 부딫혔다고 판단이 되면(서로의 거리값이 0) 리셋
		break;

	case ePlayerState::CLEAR:
		// 2초 뒤 Clear 지점에 도착했다고 판단
		// 인게임 씬 종료 -> 다음 씬으로 씬 체인지
		break;
	}
}

void PlayerOld::Fall()
{
	m_State = ePlayerState::FALL;

	for (UINT i = 0; i < m_pPlayerList.size(); i++)
	{
		m_pPlayerList[i]->Fall(m_pTimer->DeltaTime());
	}
}

void PlayerOld::Reset()
{
	switch (m_ResetPoint)
	{
	case eResetPoint::BASE:
		for (UINT i = 0; i < m_pPlayerList.size(); i++)
		{
			m_pPlayerList[i]->SetPosition(m_ResetPosition.x, m_ResetPosition.y, m_ResetPosition.z);
			m_pPlayerList[i]->GetBoxCollider()->SetPosition(m_ResetPosition.x, m_ResetPosition.y, m_ResetPosition.z);
			m_pMoveToLeft->SetPosition(m_ResetPosition.x - 8.0f, m_ResetPosition.y + 1.5f, m_ResetPosition.z);
			m_pMoveToRight->SetPosition(m_ResetPosition.x + 8.0f, m_ResetPosition.y + 1.5f, m_ResetPosition.z);
			m_pMoveToUp->SetPosition(m_ResetPosition.x, m_ResetPosition.y + 1.5f, m_ResetPosition.z + 7.7f);
			m_pMoveToDown->SetPosition(m_ResetPosition.x, m_ResetPosition.y + 1.5f, m_ResetPosition.z - 7.7f);
		}
		break;

	case eResetPoint::SAVEPOINT01:
		for (UINT i = 0; i < m_pPlayerList.size(); i++)
		{
			m_pPlayerList[i]->SetPosition(m_ResetSavePoint01Position.x, m_ResetSavePoint01Position.y, m_ResetSavePoint01Position.z);
			m_pPlayerList[i]->GetBoxCollider()->SetPosition(m_ResetSavePoint01Position.x, m_ResetSavePoint01Position.y, m_ResetSavePoint01Position.z);
			m_pMoveToLeft->SetPosition(m_ResetPosition.x - 8.0f, m_ResetPosition.y + 1.5f, m_ResetPosition.z);
			m_pMoveToRight->SetPosition(m_ResetPosition.x + 8.0f, m_ResetPosition.y + 1.5f, m_ResetPosition.z);
			m_pMoveToUp->SetPosition(m_ResetPosition.x, m_ResetPosition.y + 1.5f, m_ResetPosition.z + 7.7f);
			m_pMoveToDown->SetPosition(m_ResetPosition.x, m_ResetPosition.y + 1.5f, m_ResetPosition.z - 7.7f);
		}
		break;

	case eResetPoint::SAVEPOINT02:
		for (UINT i = 0; i < m_pPlayerList.size(); i++)
		{
			m_pPlayerList[i]->SetPosition(m_ResetSavePoint02Position.x, m_ResetSavePoint02Position.y, m_ResetSavePoint02Position.z);
			m_pPlayerList[i]->GetBoxCollider()->SetPosition(m_ResetSavePoint02Position.x, m_ResetSavePoint02Position.y, m_ResetSavePoint02Position.z);
			m_pMoveToLeft->SetPosition(m_ResetPosition.x - 8.0f, m_ResetPosition.y + 1.5f, m_ResetPosition.z);
			m_pMoveToRight->SetPosition(m_ResetPosition.x + 8.0f, m_ResetPosition.y + 1.5f, m_ResetPosition.z);
			m_pMoveToUp->SetPosition(m_ResetPosition.x, m_ResetPosition.y + 1.5f, m_ResetPosition.z + 7.7f);
			m_pMoveToDown->SetPosition(m_ResetPosition.x, m_ResetPosition.y + 1.5f, m_ResetPosition.z - 7.7f);
		}
		break;
	}

	m_State = ePlayerState::IDLE;
}

void PlayerOld::GetStairMoveToLeft(Stair* pStair)
{
	m_MoveToLeft = pStair->GetStairObject()->GetPosition();
}

void PlayerOld::GetStairMoveToRight(Stair* pStair)
{
	m_MoveToRight = pStair->GetStairObject()->GetPosition();
}

void PlayerOld::GetStairMoveToUp(Stair* pStair)
{
	m_MoveToUp = pStair->GetStairObject()->GetPosition();
}

void PlayerOld::GetStairMoveToDown(Stair* pStair)
{
	m_MoveToDown = pStair->GetStairObject()->GetPosition();
}

void PlayerOld::Finalize()
{
	m_pPlayerList.clear();
}

void PlayerOld::SetResetPosition(float posX, float posY, float posZ)
{
	m_ResetPosition.x = posX;
	m_ResetPosition.y = posY;
	m_ResetPosition.z = posZ;
}

void PlayerOld::SetResetSavePoint01Position(float posX, float posY, float posZ)
{
	m_ResetSavePoint01Position.x = posX;
	m_ResetSavePoint01Position.y = posY;
	m_ResetSavePoint01Position.z = posZ;
}

void PlayerOld::SetResetSavePoint02Position(float posX, float posY, float posZ)
{
	m_ResetSavePoint02Position.x = posX;
	m_ResetSavePoint02Position.y = posY;
	m_ResetSavePoint02Position.z = posZ;
}