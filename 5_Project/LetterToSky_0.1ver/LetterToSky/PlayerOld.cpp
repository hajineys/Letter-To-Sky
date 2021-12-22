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

#pragma region _���� ������Ʈ�� ������ ����. ���������� �� ����.
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

		// ���������� ����Ͽ� �ڵ带 �Ѵ��� �����ְ� ��������
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
/// 1. Ű �Է��� �޴´�.
/// 2. ĳ������ ������ ���õ� �Ѵ�.
/// 3. ĳ������ ȸ������ ���� ��������ش�.
/// 4. �׿� ���� �浹������ �ö��̴��� (��ġ��) ������ش�.
/// </summary>
void PlayerOld::KeyInput(float dTime)
{
	static float per = 0.0f;
	static bool isLerping = false;

	static Vector3 startPos;
	static Vector3 targetPos;

	static Vector3 nowPos;	// ���� ���൵�� ���� ���� ������...-> per 0% -> nowPos = startPos
							// per -> 100% (1.0) -> nowPos = targetPos

	// ���� Ű�� ������, Lerp�� �����Ѵ�.
	// ���� Ű�� ������ �� �� �ѹ� ���´�.
	if (GetAsyncKeyState(VK_LEFT) & 0x0001 && !isLerping)
	{
		isLerping = true;

		for (UINT i = 0; i < m_pPlayerList.size(); i++)
		{
			// ������ ����, (�ʱ�ȭ)
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
			// ������ ����, (�ʱ�ȭ)
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
			// ������ ����, (�ʱ�ȭ)
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
			// ������ ����, (�ʱ�ȭ)
			startPos = m_pPlayerList[i]->GetPosition();

			targetPos = m_MoveToDown;
			targetPos.y = m_MoveToDown.y + 2.8f;
			m_pPlayerList[i]->SetRotation('Y', 180.0f);
		}

		per = 0.0f;
	}

	// �� ������ ���� ���̴�.
	if (isLerping)
	{
		m_State = ePlayerState::JUMP;
		///Lerp();

		// per (0 ~ 1), �ð��� �������� �ۼ�Ʈ�� ������Ų��.
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

		// ..... -> �ۼ�Ʈ�� 1(100%)�� ������ �ȵȴ�.
		if (per > 1.0f)
		{
			per = 1.0f;
		}

		// per�� 1�� �Ѿ����Ƿ� Lerp �ߴ��Ѵ�.
		if (per >= 1.0f)
		{
			isLerping = false;
			m_State = ePlayerState::IDLE;
		}
	}
}

void PlayerOld::PlayerFSM()
{
	// Player�� ��ġ�� ���� BoxCollider�� ��ġ�� �ٽ� Update���ش�

	// Update������ Player�� FSM�� ��� üũ�Ѵ�

	switch (m_State)
	{
	case ePlayerState::IDLE:
		// Ű �Է�(����/������/��/�Ʒ�)
		// �̵��� ������ ����� �ִ� �� ���� �� �Ǵ�
		// �ִٸ� Jump/���ٸ� Fall
		break;

	case ePlayerState::JUMP:
		// �̵�
		// �ռ� Idle���¿��� ����� �ִٰ� �Ǵ� �� Jump������ �ƹ��͵� ���ٸ� �ٽ� Fall�� ���� ����
		// �μ��� ������� �̵��ߴٸ� 0.5�ʵ� Fall�� ���º���
		// Strafe�� �̵��� ������� �����ߴٸ� �� ����� ��ġ�� ���� ������ ����
		break;

	case ePlayerState::FALL:
		// m_PlayerList[Fall]->Render
		// ������ Y, 180��(ĳ���� �� ���������)
		// ��������, Ű �Է� �Ұ�
		// ���� Value��ŭ(?) �������� ����(SavePoint��������)
		break;

	case ePlayerState::DAMAGE:
		// ���ع�ü�� �Ÿ� ����
		// ���ع�ü�� �΋H���ٰ� �Ǵ��� �Ǹ�(������ �Ÿ����� 0) ����
		break;

	case ePlayerState::CLEAR:
		// 2�� �� Clear ������ �����ߴٰ� �Ǵ�
		// �ΰ��� �� ���� -> ���� ������ �� ü����
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