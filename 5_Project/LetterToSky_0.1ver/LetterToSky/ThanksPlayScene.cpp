#include "ThanksPlayScene.h"
#include "SceneManager.h"
#include "GameTimer.h"
#include "SoundManager.h"

// DirectX
#include "DXFactory.h"
#include "IDXObjectBase.h"
#include "DXRenderer.h"

#include "SoundManager.h"

ThanksPlayScene::ThanksPlayScene()
{

}

ThanksPlayScene::~ThanksPlayScene()
{

}

void ThanksPlayScene::Initialize(DXDevice* pDevice, DXFactory* pFactory)
{
	// Objects 생성
	m_pUI.push_back(pFactory->CreateBitmapObject(1920, 1080, L"../Data/Image/ThanksToPlay_Background.png"));
	m_pUI[0]->SetBitmapPosition(0, 0);
}

void ThanksPlayScene::Update(HWND hWnd, SceneManager* pSceneManager, GameTimer* pTimer, D3Camera* pCamera)
{
	for (UINT i = 0; i < m_pUI.size(); i++)
	{
		m_pUI[i]->Update(pTimer, pCamera);
	}

	// MousePos를 얻어옴
	GetCursorPos(&m_MousePos);
	ScreenToClient(hWnd, &m_MousePos);

	ChangeSceneState(pSceneManager);

	// 씬 전환
	if (GetAsyncKeyState(VK_NEXT) & 0x0001)
	{
		pSceneManager->ChangeScene(eSceneState::THANKSPLAY);
	}
}

void ThanksPlayScene::Render(DXRenderer* pRenderer)
{
	for (UINT i = 0; i < m_pUI.size(); i++)
	{
		m_pUI[i]->Render();
	}
}

void ThanksPlayScene::Finalize()
{
	for (UINT i = 0; i < m_pUI.size(); i++)
	{
		m_pUI[i]->Finalize();
	}

	m_pUI.clear();
}

void ThanksPlayScene::ChangeSceneState(SceneManager* pSceneManager)
{
	// 게임 끝
}