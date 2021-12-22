#include "IntroCutScene.h"
#include "SceneManager.h"
#include "Transform.h"
#include "GameTimer.h"

// DirectX
#include "DXFactory.h"
#include "IDXObjectBase.h"
#include "DXRenderer.h"

#include "SoundManager.h"

IntroCutScene::IntroCutScene()
	: m_PostOfficeOn(false),
	m_NowTextIndex(4),
	m_NextScene(false)
{

}

IntroCutScene::~IntroCutScene()
{

}

void IntroCutScene::Initialize(DXDevice* pDevice, DXFactory* pFactory)
{
	// Objects 생성
	m_pUI.push_back(pFactory->CreateBitmapObject(1920, 1080, L"../Data/Image/IntroCut_SkyCountry.png"));
	m_pUI[0]->SetBitmapPosition(0, 0);

	m_pUI.push_back(pFactory->CreateBitmapObject(410, 615, L"../Data/Image/IntroCut_Character.png"));
	m_pUI[1]->SetBitmapPosition(760, 215);

	m_pUI.push_back(pFactory->CreateBitmapObject(410, 615, L"../Data/Image/IntroCut_Character_happy.png"));
	m_pUI[2]->SetBitmapPosition(760, 215);

	m_pUI.push_back(pFactory->CreateBitmapObject(1920, 1080, L"../Data/Image/IntroCut_Post_Office.png"));
	m_pUI[3]->SetBitmapPosition(0, 0);

	m_pUI.push_back(pFactory->CreateBitmapObject(1816, 206, L"../Data/Image/IntroCut_TextBox01.png"));
	m_pUI[4]->SetBitmapPosition(52, 826);

	m_pUI.push_back(pFactory->CreateBitmapObject(1816, 206, L"../Data/Image/IntroCut_TextBox02.png"));
	m_pUI[5]->SetBitmapPosition(52, -500);

	m_pUI.push_back(pFactory->CreateBitmapObject(1816, 206, L"../Data/Image/IntroCut_TextBox03.png"));
	m_pUI[6]->SetBitmapPosition(52, -500);

	m_pUI.push_back(pFactory->CreateBitmapObject(1816, 206, L"../Data/Image/IntroCut_TextBox04.png"));
	m_pUI[7]->SetBitmapPosition(52, -500);

	m_pUI.push_back(pFactory->CreateBitmapObject(1816, 206, L"../Data/Image/IntroCut_TextBox05.png"));
	m_pUI[8]->SetBitmapPosition(52, -500);

	m_pUI.push_back(pFactory->CreateBitmapObject(1816, 206, L"../Data/Image/IntroCut_TextBox06.png"));
	m_pUI[9]->SetBitmapPosition(52, -500);

	m_pUI.push_back(pFactory->CreateBitmapObject(1816, 206, L"../Data/Image/IntroCut_TextBox07.png"));
	m_pUI[10]->SetBitmapPosition(52, -500);

	// 오류 방지용
	m_pUI.push_back(pFactory->CreateBitmapObject(1816, 206, L"../Data/Image/IntroCut_TextBox07.png"));
	m_pUI[11]->SetBitmapPosition(52, -500);

	SoundManager::GetInstance()->Play(0, "BGM_IntroCut");
}

void IntroCutScene::Update(HWND hWnd, SceneManager* pSceneManager, GameTimer* pTimer, D3Camera* pCamera)
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
		m_pUI[4]->CheckUIPick(m_MousePos.x, m_MousePos.y);
		m_pUI[m_NowTextIndex]->CheckUIPick(m_MousePos.x, m_MousePos.y);
	}

	if (m_pUI[4]->GetPick() == true)
	{
		m_PostOfficeOn = true;
	}

	if (m_pUI[m_NowTextIndex]->GetPick() == true)
	{
		m_pUI[m_NowTextIndex]->SetBitmapPosition(52, -500);
		m_pUI[m_NowTextIndex + 1]->SetBitmapPosition(52, 826);

		m_NowTextIndex++;
	}

	if (m_NowTextIndex == 5)
	{
		m_pUI[1]->SetBitmapPosition(950, 215);
	}

	if (m_NowTextIndex == 8)
	{
		m_pUI[1]->SetBitmapPosition(760, 215);
		m_PostOfficeOn = false;
	}

	if (m_NowTextIndex > 10)
	{
		m_NextScene = true;
	}

	ChangeSceneState(pSceneManager);
}

void IntroCutScene::Render(DXRenderer* pRenderer)
{
	m_pUI[0]->Render();
	m_pUI[1]->Render();

	if (m_PostOfficeOn == true)
	{
		m_pUI[3]->Render();
	}

	if (m_NowTextIndex == 10)
	{
		m_pUI[2]->Render();
	}

	m_pUI[4]->Render();
	m_pUI[5]->Render();
	m_pUI[6]->Render();
	m_pUI[7]->Render();
	m_pUI[8]->Render();
	m_pUI[9]->Render();
	m_pUI[10]->Render();

	//SimpleMath::Vector4 black(0.0f, 0.0f, 0.0f, 1.0f);
	//
	//pRenderer->DrawText(30, 50, black, (TCHAR*)L"m_NowTextIndex : %d", (TCHAR*)m_NowTextIndex);
}

void IntroCutScene::Finalize()
{
	for (UINT i = 0; i < m_pUI.size(); i++)
	{
		m_pUI[i]->Finalize();
	}

	m_pUI.clear();
}

void IntroCutScene::ChangeSceneState(SceneManager* pSceneManager)
{
	// 씬 전환
	if (m_NextScene)
	{
		SoundManager::GetInstance()->Stop(1, "Button");
		pSceneManager->ChangeScene(eSceneState::AREA_TUTO_CHOICE);
	}
}