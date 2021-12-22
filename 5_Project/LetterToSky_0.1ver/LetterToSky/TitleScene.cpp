#include "TitleScene.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "GameTimer.h"

// DirectX
#include "DXFactory.h"
#include "IDXObjectBase.h"
#include "DXRenderer.h"

#include "SoundManager.h"

bool TitleScene::m_OnceCreateAll = false;

TitleScene::TitleScene()
	: m_NextScene(false)
{

}

TitleScene::~TitleScene()
{

}

void TitleScene::Initialize(DXDevice* pDevice, DXFactory* pFactory, ResourceManager* pResourceManager)
{
	if (m_OnceCreateAll == false)
	{
		pResourceManager->CreateTutoInGameAssets();
		pResourceManager->CreateMainInGameAssets();
		m_OnceCreateAll = true;
	}

	// Objects 생성
	m_pUI.push_back(pFactory->CreateBitmapObject(1920, 1080, L"../Data/Image/Title_Background.png"));
	m_pUI[0]->SetBitmapPosition(0, 0);

	m_pUI.push_back(pFactory->CreateBitmapObject(915, 401, L"../Data/Image/Title_TitleText.png"));
	m_pUI[1]->SetBitmapPosition(103, 123);

	m_pUI.push_back(pFactory->CreateBitmapObject(378, 128, L"../Data/Image/Title_Start.png"));
	m_pUI[2]->SetBitmapPosition(290, 590);

	m_pUI.push_back(pFactory->CreateBitmapObject(378, 128, L"../Data/Image/Title_Exit.png"));
	m_pUI[3]->SetBitmapPosition(290, 790);

	SoundManager::GetInstance()->Play(0, "BGM_Title");
}

void TitleScene::Update(HWND hWnd, SceneManager* pSceneManager, GameTimer* pTimer, D3Camera* pCamera)
{
	for (UINT i = 0; i < m_pUI.size(); i++)
	{
		m_pUI[i]->Update(pTimer, pCamera);
	}

	// MousePos를 얻어옴
	GetCursorPos(&m_MousePos);
	ScreenToClient(hWnd, &m_MousePos);

	if (pSceneManager->m_IsMouseUp)
	{
		m_pUI[2]->CheckUIPick(m_MousePos.x, m_MousePos.y);
		m_pUI[3]->CheckUIPick(m_MousePos.x, m_MousePos.y);
	}

	if (m_pUI[2]->GetPick() == true && pSceneManager->m_IsMouseUp == true)
	{
		m_NextScene = true;
	}

	if (m_pUI[3]->GetPick() == true && pSceneManager->m_IsMouseUp == true)
	{
		SendMessage(hWnd, WM_DESTROY, 0, 0);
	}

	// if (버튼을 클릭했을 때)
	// Play("buttonSound");
	// else 버튼을 클릭하지 않았다
	// Stop("buttonSound");

	ChangeSceneState(pSceneManager);
}

void TitleScene::Render(DXRenderer* pRenderer)
{
	for (UINT i = 0; i < m_pUI.size(); i++)
	{
		m_pUI[i]->Render();
	}

	//SimpleMath::Vector4 black(0.0f, 0.0f, 0.0f, 1.0f);
	//
	//pRenderer->DrawText(m_MousePos.x, m_MousePos.y, black, (TCHAR*)L"%d, %d", m_MousePos.x, m_MousePos.y);
}

void TitleScene::Finalize()
{
	for (UINT i = 0; i < m_pUI.size(); i++)
	{
		m_pUI[i]->Finalize();
	}

	m_pUI.clear();

}

void TitleScene::ChangeSceneState(SceneManager* pSceneManager)
{
	// 씬 전환
	if (m_NextScene == true)
	{
		SoundManager::GetInstance()->Play(1, "Button");
		SoundManager::GetInstance()->Stop(0, "BGM_Title");
		pSceneManager->ChangeScene(eSceneState::INTRO_CUT);
	}

	if (GetAsyncKeyState(VK_F7) & 0x0001)
	{
		pSceneManager->ChangeScene(eSceneState::TUTORIALINGAME);
	}

	if (GetAsyncKeyState(VK_F8) & 0x0001)
	{
		pSceneManager->ChangeScene(eSceneState::MAININGAME);
	}
}