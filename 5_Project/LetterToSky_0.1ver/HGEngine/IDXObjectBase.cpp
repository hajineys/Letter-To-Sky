#include "IDXObjectBase.h"
#include "DXDevice.h"
#include "DXRenderStates.h"
#include "D3Camera.h"
#include "BoxCollider.h"

IDXObjectBase::IDXObjectBase(DXDevice* pDevice)
	: md3dDevice(pDevice->GetD3dDevice()), md3dImmediateContext(pDevice->GetD3dImmediateContext()), mRenderstate(DXRenderStates::m_pSolidRS),
	mTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST),
	mView(), mProj(),
	mEyePosW(0.0f, 0.0f, 0.0f),
	m_pBoxCollider(nullptr),
	//m_RenderingPriority(0)
	m_IsPicked(false), m_IsCollided(false),
	m_DebugBoxOn(false)
{
	mMaterial.Ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	mMaterial.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	mMaterial.Specular = XMFLOAT4(0.6f, 0.6f, 0.6f, 16.0f);

	m_pBoxCollider = new BoxCollider(pDevice);
}

IDXObjectBase::~IDXObjectBase()
{

}

void IDXObjectBase::Initialize()
{

}

void IDXObjectBase::Update(GameTimer* pTimer /*= nullptr*/, D3Camera* pCamera /*= nullptr*/)
{
	UpdateWorldTM();

	mView = pCamera->View();
	mProj = pCamera->Proj();

	mEyePosW = pCamera->GetPosition();

	if (GetAsyncKeyState(VK_F5) & 0x0001)
	{
		m_DebugBoxOn ^= true;
	}
}

void IDXObjectBase::Render()
{
	if (m_DebugBoxOn)
	{
		m_pBoxCollider->Render();
	}
}

void IDXObjectBase::SetBoxColliderScale(float width /*= 5.0f*/, float height /*= 5.0f*/, float depth /*= 5.0f*/)
{
	m_pBoxCollider->Initialize(width, height, depth);
}

void IDXObjectBase::Finalize()
{
	delete m_pBoxCollider;
	m_pBoxCollider = nullptr;
}
