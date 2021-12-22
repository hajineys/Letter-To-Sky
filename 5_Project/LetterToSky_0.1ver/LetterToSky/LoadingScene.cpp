#include "LoadingScene.h"
#include "SceneManager.h"
#include "GameTimer.h"

// DirectX
#include "DXFactory.h"
#include "IDXObjectBase.h"
#include "DXRenderer.h"

LoadingScene::LoadingScene()
	: m_NextScene(false)
{

}

LoadingScene::~LoadingScene()
{

}

void LoadingScene::Initialize(DXDevice* pDevice, DXFactory* pFactory)
{
	// Objects »ý¼º
	m_pUI.push_back(pFactory->CreateBitmapObject(1920, 1080, L"../Data/Image/Loading_UploadBackground.png"));
	m_pUI[0]->SetBitmapPosition(0, 0);
}

void LoadingScene::Update(HWND hWnd, SceneManager* pSceneManager, GameTimer* pTimer, D3Camera* pCamera)
{
	for (UINT i = 0; i < m_pUI.size(); i++)
	{
		m_pUI[i]->Update(pTimer, pCamera);
	}

	// MousePos¸¦ ¾ò¾î¿È
	GetCursorPos(&m_MousePos);
	ScreenToClient(hWnd, &m_MousePos);

	if (pSceneManager->m_IsMouseUp)
	{
		m_pUI[0]->CheckUIPick(m_MousePos.x, m_MousePos.y);
	}

	if (m_pUI[0]->GetPick() == true)
	{
		m_NextScene = true;
	}

	ChangeSceneState(pSceneManager);
}

void LoadingScene::Render(DXRenderer* pRenderer)
{
	for (UINT i = 0; i < m_pUI.size(); i++)
	{
		m_pUI[i]->Render();
	}
}

void LoadingScene::Finalize()
{
	for (UINT i = 0; i < m_pUI.size(); i++)
	{
		m_pUI[i]->Finalize();
	}

	m_pUI.clear();
}

void LoadingScene::ChangeSceneState(SceneManager* pSceneManager)
{
	// ¾À ÀüÈ¯
	if (m_NextScene)
	{
		pSceneManager->ChangeScene(eSceneState::TITLE);
	}
}