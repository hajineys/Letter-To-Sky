#include "Bird.h"
#include "DXFactory.h"
#include "IDXObjectBase.h"
#include "BoxCollider.h"
#include "MathHelper.h"

Bird::Bird(DXFactory* pFactory, eBirdType type)
	: m_pBirdObject(nullptr),
	m_Type(type),
	m_ResetPosition(0.0f, 0.0f, 0.0f)
{
	switch (type)
	{
	case eBirdType::BIG:
		m_pBirdObject = pFactory->CreateASEObject((LPSTR)"../Data/ASE/big_bird.ASE", L"../Data/Textures/big_bird.dds");
		break;

	case eBirdType::SMALL:
		m_pBirdObject = pFactory->CreateASEObject((LPSTR)"../Data/ASE/small_bird.ASE", L"../Data/Textures/small_bird.dds");
		break;
	}
}

Bird::~Bird()
{

}

void Bird::Initialize(float posX, float posY, float posZ)
{
	SetResetPosition(posX, posY, posZ);

	m_pBirdObject->SetPosition(posX, posY, posZ);
	m_pBirdObject->GetBoxCollider()->SetPosition(posX, posY, posZ);

	if (m_Type == eBirdType::BIG)
	{
		m_pBirdObject->SetBoxColliderScale(3.0f, 6.0f, 3.0f);
	}
	else if (m_Type == eBirdType::SMALL)
	{
		m_pBirdObject->SetBoxColliderScale(3.0f, 6.0f, 3.0f);
	}
}

void Bird::Update(GameTimer* pTimer, D3Camera* pCamera)
{
	m_pBirdObject->Update(pTimer, pCamera);
	m_pBirdObject->GetBoxCollider()->Update(pCamera);

	m_pBirdObject->GetBoxCollider()->SetPosition(m_pBirdObject->GetPosition().x,
		m_pBirdObject->GetPosition().y, m_pBirdObject->GetPosition().z);
}

void Bird::Render()
{
	m_pBirdObject->Render();
}

void Bird::Reset()
{
	m_pBirdObject->SetPosition(m_ResetPosition.x, m_ResetPosition.y, m_ResetPosition.z);
	m_pBirdObject->GetBoxCollider()->SetPosition(m_ResetPosition.x, m_ResetPosition.y, m_ResetPosition.z);
}

void Bird::Finalize()
{
	delete m_pBirdObject;
	m_pBirdObject = nullptr;
}

void Bird::SetResetPosition(float posX, float posY, float posZ)
{
	m_ResetPosition.x = posX;
	m_ResetPosition.y = posY;
	m_ResetPosition.z = posZ;
}