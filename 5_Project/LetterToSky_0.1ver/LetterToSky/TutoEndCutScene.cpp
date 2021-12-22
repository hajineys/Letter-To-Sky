#include "TutoEndCutScene.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "GameTimer.h"

// DirectX
#include "DXFactory.h"
#include "IDXObjectBase.h"
#include "DXRenderer.h"

#include "SoundManager.h"

TutoEndCutScene::TutoEndCutScene()
	: m_CharacterOn(true),
	m_TextImageOff(false),
	m_ClearImageDelayTime(0.0f),
	m_NowTextIndex(5)
{

}

TutoEndCutScene::~TutoEndCutScene()
{

}

void TutoEndCutScene::Initialize(DXDevice* pDevice, DXFactory* pFactory)
{
	// Objects »ý¼º
	m_pUI.push_back(pFactory->CreateBitmapObject(1920, 1080, L"../Data/Image/EndCut_Background.png"));
	m_pUI[0]->SetBitmapPosition(0, 0);

	m_pUI.push_back(pFactory->CreateBitmapObject(1920, 1080, L"../Data/Image/AreaChoice_Background_2.png"));
	m_pUI[1]->SetBitmapPosition(0, -2000);

	m_pUI.push_back(pFactory->CreateBitmapObject(1920, 1080, L"../Data/Image/EndCut_01_GirlWithDog.png"));
	m_pUI[2]->SetBitmapPosition(0, -2000);

	m_pUI.push_back(pFactory->CreateBitmapObject(410, 615, L"../Data/Image/IntroCut_Character_R.png"));
	m_pUI[3]->SetBitmapPosition(320, 215);

	m_pUI.push_back(pFactory->CreateBitmapObject(500, 750, L"../Data/Image/EndCut_01_Dog.png"));
	m_pUI[4]->SetBitmapPosition(1070, 210);

	m_pUI.push_back(pFactory->CreateBitmapObject(1816, 206, L"../Data/Image/EndCut_TextBox01.png"));
	m_pUI[5]->SetBitmapPosition(52, 828);

	m_pUI.push_back(pFactory->CreateBitmapObject(1816, 206, L"../Data/Image/EndCut_TextBox02.png"));
	m_pUI[6]->SetBitmapPosition(52, -500);

	m_pUI.push_back(pFactory->CreateBitmapObject(1816, 206, L"../Data/Image/AreaChoice_TextBox08.png"));
	m_pUI[7]->SetBitmapPosition(52, -500);

	m_pUI.push_back(pFactory->CreateBitmapObject(1920, 1080, L"../Data/Image/InGame_Clear.png"));
	m_pUI[8]->SetBitmapPosition(0, -1500);

	m_pUI.push_back(pFactory->CreateBitmapObject(564, 48, L"../Data/Image/Tutorial_Clear_Text.png"));
	m_pUI[9]->SetBitmapPosition(0, -500);

	SoundManager::GetInstance()->Play(0, "BGM_Ending");
}

void TutoEndCutScene::Update(HWND hWnd, SceneManager* pSceneManager, GameTimer* pTimer, D3Camera* pCamera)
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
		m_pUI[m_NowTextIndex]->CheckUIPick(m_MousePos.x, m_MousePos.y);
	}

	if (m_NowTextIndex < 7)
	{
		if (m_pUI[m_NowTextIndex]->GetPick() == true)
		{
			m_pUI[m_NowTextIndex]->SetBitmapPosition(52, -500);
			m_pUI[m_NowTextIndex + 1]->SetBitmapPosition(52, 826);
			m_NowTextIndex++;
		}
	}
	else if (m_NowTextIndex == 7)
	{
		m_pUI[1]->SetBitmapPosition(0, 0);
		m_pUI[2]->SetBitmapPosition(0, 0);

		m_CharacterOn = false;
	}

	if (m_CharacterOn == false)
	{
		m_ClearImageDelayTime += pTimer->DeltaTime();

		if (3.0f < m_ClearImageDelayTime)
		{
			m_pUI[9]->SetBitmapPosition(680, 950);
			m_TextImageOff = true;
			SoundManager::GetInstance()->Play(1, "SendLetter");
			m_pUI[8]->SetBitmapPosition(0, 0);
		}
	}

	if (pSceneManager->m_IsMouseUp)
	{
		m_pUI[9]->CheckUIPick(m_MousePos.x, m_MousePos.y);
	}

	if (m_pUI[9]->GetPick() == true)
	{
		m_NextScene = true;
	}

	ChangeSceneState(pSceneManager);
}

void TutoEndCutScene::Render(DXRenderer* pRenderer)
{
	m_pUI[0]->Render();
	m_pUI[1]->Render();
	m_pUI[2]->Render();

	if (m_CharacterOn == true)
	{
		m_pUI[3]->Render();
		m_pUI[4]->Render();
	}

	if (m_TextImageOff == false)
	{
		m_pUI[7]->Render();
	}

	m_pUI[5]->Render();
	m_pUI[6]->Render();
	m_pUI[8]->Render();
	m_pUI[9]->Render();

	//SimpleMath::Vector4 black(0.0f, 0.0f, 0.0f, 1.0f);
	//
	//pRenderer->DrawText(30, 50, black, (TCHAR*)L"m_NowTextIndex : %d", (TCHAR*)m_NowTextIndex);
}

void TutoEndCutScene::Finalize()
{
	for (UINT i = 0; i < m_pUI.size(); i++)
	{
		m_pUI[i]->Finalize();
	}

	m_pUI.clear();

	SoundManager::GetInstance()->Stop(1, "SendLetter");
}

void TutoEndCutScene::ChangeSceneState(SceneManager* pSceneManager)
{
	// ¾À ÀüÈ¯
	if (m_NextScene)
	{
		SoundManager::GetInstance()->Stop(0, "BGM_Ending");
		pSceneManager->ChangeScene(eSceneState::AREA_MAIN_CHOICE);
	}
}