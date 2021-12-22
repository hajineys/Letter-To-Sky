#include "Stair.h"
#include "DXFactory.h"
#include "GameTimer.h"
#include "PlayerOld.h"
#include "PlayerNew.h"
#include "IDXObjectBase.h"
#include "BoxCollider.h"
#include "MathHelper.h"

Stair::Stair(DXFactory* pFactory, eStairType type, eStairState state)
	: m_pTimer(nullptr),
	m_pStairObject(nullptr),
	m_Type(type),
	m_State(state),
	m_ResetPosition(0.0f, 0.0f, 0.0f),
	m_CheckFallTime(0.0f),
	m_CloudCreateTime(0.0f)
{
	switch (type)
	{
		case eStairType::Start_Base:
			m_pStairObject = pFactory->CreateASEObject((LPSTR)"../Data/ASE/base_start.ASE", L"../Data/Textures/base_start.dds");
			break;

		case eStairType::Cloud_Base:
			m_pStairObject = pFactory->CreateASEObject((LPSTR)"../Data/ASE/cloud_base.ASE", L"../Data/Textures/cloud_base.dds");
			break;

		case eStairType::End_Base:
			m_pStairObject = pFactory->CreateASEObject((LPSTR)"../Data/ASE/base_end.ASE", L"../Data/Textures/base_end.dds");
			break;

		case eStairType::Area01_Normal:
			m_pStairObject = pFactory->CreateASEObject((LPSTR)"../Data/ASE/stair_first.ASE", L"../Data/Textures/stair_first.dds");
			break;

		case eStairType::Area01_Broken:
			m_pStairObject = pFactory->CreateASEObject((LPSTR)"../Data/ASE/stair_first.ASE", L"../Data/Textures/stair_first_fall.dds");
			break;

		case eStairType::Area02_Normal:
			m_pStairObject = pFactory->CreateASEObject((LPSTR)"../Data/ASE/stair_second.ASE", L"../Data/Textures/stair_second.dds");
			break;

		case eStairType::Area02_Broken:
			m_pStairObject = pFactory->CreateASEObject((LPSTR)"../Data/ASE/stair_second.ASE", L"../Data/Textures/stair_second_fall.dds");
			break;

		case eStairType::Area03_Normal:
			m_pStairObject = pFactory->CreateASEObject((LPSTR)"../Data/ASE/stair_third.ASE", L"../Data/Textures/stair_third.dds");
			break;

		case eStairType::Area03_Broken:
			m_pStairObject = pFactory->CreateASEObject((LPSTR)"../Data/ASE/stair_third.ASE", L"../Data/Textures/stair_third_fall.dds");
			break;

		case eStairType::Cloud:
			m_pStairObject = pFactory->CreateASEObject((LPSTR)"../Data/ASE/cloud_small.ASE", L"../Data/Textures/cloud_small.dds");
			break;
	}
}

Stair::~Stair()
{

}

void Stair::Initialize(float posX, float posY, float posZ)
{
	GetResetPosition(posX, posY, posZ);
	m_pStairObject->SetPosition(posX, posY, posZ);

	if (m_Type == eStairType::Start_Base)
	{
		m_pStairObject->GetBoxCollider()->SetPosition(posX, posY - 4.8f, posZ - 10.0f);
		m_pStairObject->SetBoxColliderScale(23.0f, 11.5f, 36.0f);
	}
	else if (m_Type == eStairType::Cloud_Base)
	{
		m_pStairObject->GetBoxCollider()->SetPosition(posX, posY - 1.7f, posZ);
		m_pStairObject->SetBoxColliderScale(26.5f, 4.5f, 20.5f);
	}
	else if (m_Type == eStairType::End_Base)
	{
		m_pStairObject->GetBoxCollider()->SetPosition(posX, posY - 3.3f, posZ - 14.0f);
		m_pStairObject->SetBoxColliderScale(50.0f, 7.5f, 38.0f);
	}
	else if (m_Type == eStairType::Cloud)
	{
		m_pStairObject->GetBoxCollider()->SetPosition(posX, posY + 1.5f, posZ);
		m_pStairObject->SetBoxColliderScale(5.0f, 3.0f, 6.0f);
	}
	else
	{
		m_pStairObject->GetBoxCollider()->SetPosition(posX, posY + 1.35f, posZ);
		m_pStairObject->SetBoxColliderScale(7.5f, 2.8f, 4.5f);
	}
}

void Stair::Update(GameTimer* pTimer, D3Camera* pCamera, PlayerNew* pPlayer)
{
	m_pTimer = pTimer;

	m_pStairObject->Update(pTimer, pCamera);
	m_pStairObject->GetBoxCollider()->Update(pCamera);

	// 타입
	switch (m_Type)
	{
		case eStairType::Start_Base:
		case eStairType::Cloud_Base:
		case eStairType::End_Base:
			break;

		case eStairType::Area01_Normal:
		case eStairType::Area02_Normal:
		case eStairType::Area03_Normal:
			break;

		case eStairType::Area01_Broken:
		case eStairType::Area02_Broken:
		case eStairType::Area03_Broken:
		{
			m_pStairObject->GetBoxCollider()->SetPosition(m_pStairObject->GetPosition().x,
				m_pStairObject->GetPosition().y + 1.35f, m_pStairObject->GetPosition().z);
		}
		break;

		case eStairType::Cloud:
		{
			m_pStairObject->GetBoxCollider()->SetPosition(m_pStairObject->GetPosition().x,
				m_pStairObject->GetPosition().y + 1.5f, m_pStairObject->GetPosition().z);
		}
		break;
	}
}

void Stair::Render()
{
	m_pStairObject->Render();
}

void Stair::Move(Vector3 pos, BoxCollider* pCollider, int number, PlayerNew* pPlayer)
{
	m_CloudCreateTime += m_pTimer->DeltaTime();

	switch (number)
	{
		// 바로 생성
		case 0:
			if (0.0f < m_CloudCreateTime)
			{
				m_pStairObject->SetTranslation(pos.x, pos.y, pos.z);
			}
			break;

			// 2초 뒤 생성
		case 1:
			if (2.0f < m_CloudCreateTime)
			{
				m_pStairObject->SetTranslation(pos.x, pos.y, pos.z);
			}
			break;

			// 4초 뒤 생성
		case 2:
			if (4.0f < m_CloudCreateTime)
			{
				m_pStairObject->SetTranslation(pos.x, pos.y, pos.z);
			}
			break;

			// 6초 뒤 생성
		case 3:
			if (6.0f < m_CloudCreateTime)
			{
				m_pStairObject->SetTranslation(pos.x, pos.y, pos.z);
			}
			break;
	}

	if (m_pStairObject->GetBoxCollider()->CheckCollided(pCollider))
	{
		Reset();
	}

	Vector3 strafePos(m_pStairObject->GetPosition().x, m_pStairObject->GetPosition().y, m_pStairObject->GetPosition().z);

	if (m_pStairObject->GetBoxCollider()->CheckCollided(pPlayer->m_pPlayerList[0]->GetBoxCollider()))
	{
		//pPlayer->m_pPlayerList[0]->SetPosition(strafePos.x, strafePos.y + Y_CHARACTER_OFFSET_CLOUD, strafePos.z);
		pPlayer->m_pPlayerList[0]->SetTranslation(pos.x, pos.y, pos.z);
	}
}

void Stair::Fall(float y, PlayerNew* pPlayer)
{
	if (m_pStairObject->GetBoxCollider()->CheckCollided(pPlayer->m_pPlayerList[0]->GetBoxCollider()))
	{
		m_CheckFallTime += m_pTimer->DeltaTime();

		if (DELAYTIME < m_CheckFallTime)
		{
			m_pStairObject->Fall(m_pTimer->DeltaTime());
			pPlayer->SetFallValue(y);
			pPlayer->SetGameState(ePlayerGameState::Dying);

			// 계단이 2초이상 떨어지면 위치를 리셋시켜준다
			if (m_pStairObject->GetPosition().y < y - 2.0f)
			{
				Reset();
			}
		}
	}
	else
	{
		m_CheckFallTime = 0.0f;
	}
}

void Stair::Strafe(float start, float end, float value, bool forward, PlayerNew* pPlayer)
{
	m_pStairObject->GetBoxCollider()->SetPosition(m_pStairObject->GetPosition().x,
		m_pStairObject->GetPosition().y + 1.35f, m_pStairObject->GetPosition().z);

	Vector3 strafePos(m_pStairObject->GetPosition().x, m_pStairObject->GetPosition().y, m_pStairObject->GetPosition().z);

	if (forward == true)
	{
		m_pStairObject->Strafe(start, end, value);

		if (m_pStairObject->GetBoxCollider()->CheckCollided(pPlayer->m_pPlayerList[0]->GetBoxCollider()))
		{
			pPlayer->m_pPlayerList[0]->SetPosition(strafePos.x, strafePos.y + Y_CHARACTER_OFFSET_STAIR, strafePos.z);
			//pPlayer->Strafe(start, end, value, true);
		}
	}
	else if (forward == false)
	{
		m_pStairObject->StrafeInverse(start, end, value);

		if (m_pStairObject->GetBoxCollider()->CheckCollided(pPlayer->m_pPlayerList[0]->GetBoxCollider()))
		{
			pPlayer->m_pPlayerList[0]->SetPosition(strafePos.x, strafePos.y + Y_CHARACTER_OFFSET_STAIR, strafePos.z);
			//pPlayer->Strafe(start, end, value, false);
		}
	}
}

void Stair::UpDown(float start, float end, float value, PlayerNew* pPlayer)
{
	m_pStairObject->GetBoxCollider()->SetPosition(m_pStairObject->GetPosition().x,
		m_pStairObject->GetPosition().y + 1.35f, m_pStairObject->GetPosition().z);
	
	m_pStairObject->MoveUpDown(start, end, value);

	Vector3 strafePos(m_pStairObject->GetPosition().x, m_pStairObject->GetPosition().y, m_pStairObject->GetPosition().z);

	if (m_pStairObject->GetBoxCollider()->CheckCollided(pPlayer->m_pPlayerList[0]->GetBoxCollider()))
	{
		pPlayer->m_pPlayerList[0]->SetPosition(strafePos.x, strafePos.y + Y_CHARACTER_OFFSET_STAIR, strafePos.z);
	}
}

void Stair::FrontBack(float start, float end, float value, PlayerNew* pPlayer)
{
	m_pStairObject->GetBoxCollider()->SetPosition(m_pStairObject->GetPosition().x,
		m_pStairObject->GetPosition().y + 1.35f, m_pStairObject->GetPosition().z);

	m_pStairObject->MoveFrontBack(start, end, value);

	Vector3 strafePos(m_pStairObject->GetPosition().x, m_pStairObject->GetPosition().y, m_pStairObject->GetPosition().z);

	if (m_pStairObject->GetBoxCollider()->CheckCollided(pPlayer->m_pPlayerList[0]->GetBoxCollider()))
	{
		pPlayer->m_pPlayerList[0]->SetPosition(strafePos.x, strafePos.y + Y_CHARACTER_OFFSET_STAIR, strafePos.z);
	}
}

void Stair::StrafeDiagonal(float startX, float endX, float startY, float endY, float startZ, float endZ, float dTime, PlayerNew* pPlayer)
{
	m_pStairObject->GetBoxCollider()->SetPosition(m_pStairObject->GetPosition().x,
		m_pStairObject->GetPosition().y + 1.35f, m_pStairObject->GetPosition().z);

	m_pStairObject->StrafeDiagonal(startX, endX, startY, endY, startZ, endZ, dTime);

	Vector3 strafePos(m_pStairObject->GetPosition().x, m_pStairObject->GetPosition().y, m_pStairObject->GetPosition().z);

	if (m_pStairObject->GetBoxCollider()->CheckCollided(pPlayer->m_pPlayerList[0]->GetBoxCollider()))
	{
		pPlayer->m_pPlayerList[0]->SetPosition(strafePos.x, strafePos.y + Y_CHARACTER_OFFSET_STAIR, strafePos.z);
	}
}

void Stair::FallStrafe(PlayerNew* pPlayer, float y, float start, float end, float value, bool forward)
{
	m_pStairObject->GetBoxCollider()->SetPosition(m_pStairObject->GetPosition().x,
		m_pStairObject->GetPosition().y + 1.35f, m_pStairObject->GetPosition().z);

	Vector3 strafePos(m_pStairObject->GetPosition().x, m_pStairObject->GetPosition().y, m_pStairObject->GetPosition().z);

	if (forward == true)
	{
		m_pStairObject->Strafe(start, end, value);

		if (m_pStairObject->GetBoxCollider()->CheckCollided(pPlayer->m_pPlayerList[0]->GetBoxCollider()))
		{
			m_CheckFallTime += m_pTimer->DeltaTime();

			pPlayer->m_pPlayerList[0]->SetPosition(strafePos.x, strafePos.y + Y_CHARACTER_OFFSET_STAIR, strafePos.z);
			pPlayer->Strafe(start, end, value, true);

			if (DELAYTIME < m_CheckFallTime)
			{
				m_pStairObject->Fall(m_pTimer->DeltaTime());
				pPlayer->SetFallValue(y);
				pPlayer->SetGameState(ePlayerGameState::Dying);

				// 계단이 2초이상 떨어지면 위치를 리셋시켜준다
				if (m_pStairObject->GetPosition().y < y - 2.0f)
				{
					Reset();
				}
			}
		}

	}
	else if (forward == false)
	{
		m_pStairObject->StrafeInverse(start, end, value);

		if (m_pStairObject->GetBoxCollider()->CheckCollided(pPlayer->m_pPlayerList[0]->GetBoxCollider()))
		{
			m_CheckFallTime += m_pTimer->DeltaTime();

			pPlayer->m_pPlayerList[0]->SetPosition(strafePos.x, strafePos.y + Y_CHARACTER_OFFSET_STAIR, strafePos.z);
			pPlayer->Strafe(start, end, value, false);

			if (DELAYTIME < m_CheckFallTime)
			{
				m_pStairObject->Fall(m_pTimer->DeltaTime());
				pPlayer->SetFallValue(y);
				pPlayer->SetGameState(ePlayerGameState::Dying);

				// 계단이 2초이상 떨어지면 위치를 리셋시켜준다
				if (m_pStairObject->GetPosition().y < y - 2.0f)
				{
					Reset();
				}
			}
		}
	}
	else
	{
		m_CheckFallTime = 0.0f;
	}
}

void Stair::Save()
{

}

void Stair::Clear()
{

}

void Stair::Reset()
{
	m_pStairObject->SetPosition(m_ResetPosition.x, m_ResetPosition.y, m_ResetPosition.z);
	m_pStairObject->GetBoxCollider()->SetPosition(m_ResetPosition.x, m_ResetPosition.y + 1.35f, m_ResetPosition.z);

	m_CheckFallTime = 0.0f;
}

void Stair::Finalize()
{
	delete m_pStairObject;
	m_pStairObject = nullptr;
}

void Stair::GetResetPosition(float posX, float posY, float posZ)
{
	m_ResetPosition.x = posX;
	m_ResetPosition.y = posY;
	m_ResetPosition.z = posZ;
}