#include "AreaTutoChoiceScene.h"
#include "SceneManager.h"
#include "Transform.h"
#include "GameTimer.h"

// DirectX
#include "DXFactory.h"
#include "IDXObjectBase.h"
#include "DXRenderer.h"

#include "SoundManager.h"

AreaTutoChoiceScene::AreaTutoChoiceScene()
	: m_CharacterOn(true),
	m_NowTextIndex(5),
	m_CollidedLetter(false),
	m_TutoCutImage(false),
	m_UI07Click(0),
	m_NextScene(nullptr)
{

}

AreaTutoChoiceScene::~AreaTutoChoiceScene()
{

}

void AreaTutoChoiceScene::Initialize(DXDevice* pDevice, DXFactory* pFactory)
{
	// Objects »ý¼º
	m_pUI.push_back(pFactory->CreateBitmapObject(1920, 1080, L"../Data/Image/AreaChoice_Background.png"));
	m_pUI[0]->SetBitmapPosition(0, 0);

	// ÆíÁö Off
	m_pUI.push_back(pFactory->CreateBitmapObject(150, 150, L"../Data/Image/AreaChoice_LetterOff.png"));
	m_pUI[1]->SetBitmapPosition(815, 75);

	// ÆíÁö On
	m_pUI.push_back(pFactory->CreateBitmapObject(150, 150, L"../Data/Image/AreaChoice_LetterOn.png"));
	m_pUI[2]->SetBitmapPosition(815, 75);

	// Ä³¸¯ÅÍ
	m_pUI.push_back(pFactory->CreateBitmapObject(410, 615, L"../Data/Image/IntroCut_Character.png"));
	m_pUI[3]->SetBitmapPosition(1300, 215);

	// ÄÆ¾À
	m_pUI.push_back(pFactory->CreateBitmapObject(1920, 1080, L"../Data/Image/AreaChoice_01_SadGirl.png"));
	m_pUI[4]->SetBitmapPosition(0, 0);

	m_pUI.push_back(pFactory->CreateBitmapObject(1816, 206, L"../Data/Image/AreaChoice_TextBox01.png"));
	m_pUI[5]->SetBitmapPosition(52, 828);

	m_pUI.push_back(pFactory->CreateBitmapObject(1816, 206, L"../Data/Image/AreaChoice_TextBox02.png"));
	m_pUI[6]->SetBitmapPosition(52, -500);

	m_pUI.push_back(pFactory->CreateBitmapObject(1816, 206, L"../Data/Image/AreaChoice_TextBox03.png"));
	m_pUI[7]->SetBitmapPosition(52, -500);

	m_pUI.push_back(pFactory->CreateBitmapObject(1816, 206, L"../Data/Image/AreaChoice_TextBox04.png"));
	m_pUI[8]->SetBitmapPosition(52, -500);
}

void AreaTutoChoiceScene::Update(HWND hWnd, SceneManager* pSceneManager, GameTimer* pTimer, D3Camera* pCamera)
{
	for (UINT i = 0; i < m_pUI.size(); i++)
	{
		m_pUI[i]->Update(pTimer, pCamera);
	}

	// MousePos¸¦ ¾ò¾î¿È
	GetCursorPos(&m_MousePos);
	ScreenToClient(hWnd, &m_MousePos);

	if (m_NowTextIndex == 5)
	{
		if (pSceneManager->m_IsMouseUp)
		{
			m_pUI[5]->CheckUIPick(m_MousePos.x, m_MousePos.y);
		}

		if (m_pUI[5]->GetPick() == true)
		{
			m_pUI[5]->SetBitmapPosition(52, -500);
			m_pUI[6]->SetBitmapPosition(52, 828);
			m_NowTextIndex++;
		}
	}

	m_pUI[1]->CheckUICollided(m_MousePos.x, m_MousePos.y);

	if (pSceneManager->m_IsMouseUp)
	{
		m_pUI[2]->CheckUIPick(m_MousePos.x, m_MousePos.y);
	}

	if (m_NowTextIndex == 6)
	{
		if (m_pUI[1]->GetCollided() == true)
		{
			m_CollidedLetter = true;
		}
		else
		{
			m_CollidedLetter = false;
		}

		if (m_pUI[2]->GetPick() == true)
		{
			SoundManager::GetInstance()->Play(1, "Button");
			m_pUI[6]->SetBitmapPosition(52, -500);
			m_pUI[7]->SetBitmapPosition(52, 828);
			m_NowTextIndex = 7;
		}
	}

	if (m_NowTextIndex == 7)
	{
		m_TutoCutImage = true;
		m_CharacterOn = false;

		if (pSceneManager->m_IsMouseUp)
		{
			m_pUI[7]->CheckUIPick(m_MousePos.x, m_MousePos.y);
		}

		if (m_pUI[7]->GetPick() == true)
		{
			m_pUI[7]->SetBitmapPosition(52, -500);
			m_pUI[8]->SetBitmapPosition(52, 828);
			m_NowTextIndex = 8;
		}
	}

	if (pSceneManager->m_IsMouseUp && m_NowTextIndex > 7)
	{
		m_pUI[8]->CheckUIPick(m_MousePos.x, m_MousePos.y);
		m_UI07Click++;
	}

	if (m_pUI[8]->GetPick() == true)
	{
		if (m_UI07Click > 1)
		{
			m_NextScene = true;
		}
	}

	ChangeSceneState(pSceneManager);
}

void AreaTutoChoiceScene::Render(DXRenderer* pRenderer)
{
	m_pUI[0]->Render();

	if (m_CollidedLetter == false)
	{
		m_pUI[1]->Render();
	}
	else
	{
		m_pUI[2]->Render();
	}

	if (m_CharacterOn)
	{
		m_pUI[3]->Render();
	}

	if (m_TutoCutImage == true)
	{
		m_pUI[4]->Render();
	}

	m_pUI[5]->Render();
	m_pUI[6]->Render();
	m_pUI[7]->Render();
	m_pUI[8]->Render();

	//SimpleMath::Vector4 black(0.0f, 0.0f, 0.0f, 1.0f);
	//
	//pRenderer->DrawText(30, 50, black, (TCHAR*)L"m_NowTextIndex : %d", (TCHAR*)m_NowTextIndex);
	//pRenderer->DrawText(30, 90, black, (TCHAR*)L"%d", (TCHAR*)m_UI07Click);
}

void AreaTutoChoiceScene::Finalize()
{
	for (UINT i = 0; i < m_pUI.size(); i++)
	{
		m_pUI[i]->Finalize();
	}

	m_pUI.clear();
}

void AreaTutoChoiceScene::ChangeSceneState(SceneManager* pSceneManager)
{
	// ¾À ÀüÈ¯
	if (m_NextScene)
	{
		SoundManager::GetInstance()->Stop(1, "Button");
		SoundManager::GetInstance()->Stop(0, "BGM_IntroCut");
		pSceneManager->ChangeScene(eSceneState::TUTORIALINGAME);
	}
}