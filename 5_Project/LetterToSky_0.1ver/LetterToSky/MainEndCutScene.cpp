#include "MainEndCutScene.h"
#include "SceneManager.h"
#include "GameTimer.h"
#include "SoundManager.h"

// DirectX
#include "DXFactory.h"
#include "IDXObjectBase.h"
#include "DXRenderer.h"

#include "SoundManager.h"

MainEndCutScene::MainEndCutScene()
	: m_NowTextIndex(5),
	m_MainCutImageOn(false),
	m_SendLetterImage(false),
	m_NextScene(false),
	m_ClearImageDelayTime(0.0f)
{

}

MainEndCutScene::~MainEndCutScene()
{

}

void MainEndCutScene::Initialize(DXDevice* pDevice, DXFactory* pFactory)
{
	// Objects »ý¼º
	m_pUI.push_back(pFactory->CreateBitmapObject(1920, 1080, L"../Data/Image/EndCut_Background.png"));
	m_pUI[0]->SetBitmapPosition(0, 0);

	m_pUI.push_back(pFactory->CreateBitmapObject(1920, 1080, L"../Data/Image/AreaChoice_Background_3.png"));
	m_pUI[1]->SetBitmapPosition(0, 0);

	m_pUI.push_back(pFactory->CreateBitmapObject(1920, 1080, L"../Data/Image/EndCut_02_BackGirl.png"));
	m_pUI[2]->SetBitmapPosition(0, 0);

	m_pUI.push_back(pFactory->CreateBitmapObject(410, 615, L"../Data/Image/IntroCut_Character_R.png"));
	m_pUI[3]->SetBitmapPosition(320, 215);

	m_pUI.push_back(pFactory->CreateBitmapObject(500, 700, L"../Data/Image/EndCut_02_Mom.png"));
	m_pUI[4]->SetBitmapPosition(1070, 133);

	m_pUI.push_back(pFactory->CreateBitmapObject(1816, 206, L"../Data/Image/EndCut_TextBox03.png"));
	m_pUI[5]->SetBitmapPosition(52, 828);

	m_pUI.push_back(pFactory->CreateBitmapObject(1816, 206, L"../Data/Image/EndCut_TextBox04.png"));
	m_pUI[6]->SetBitmapPosition(52, -500);

	m_pUI.push_back(pFactory->CreateBitmapObject(1816, 206, L"../Data/Image/EndCut_TextBox05.png"));
	m_pUI[7]->SetBitmapPosition(52, -500);

	m_pUI.push_back(pFactory->CreateBitmapObject(1816, 206, L"../Data/Image/AreaChoice_TextBox09.png"));
	m_pUI[8]->SetBitmapPosition(52, -500);

	m_pUI.push_back(pFactory->CreateBitmapObject(1816, 206, L"../Data/Image/AreaChoice_TextBox10.png"));
	m_pUI[9]->SetBitmapPosition(52, -500);

	m_pUI.push_back(pFactory->CreateBitmapObject(1816, 206, L"../Data/Image/AreaChoice_TextBox11.png"));
	m_pUI[10]->SetBitmapPosition(52, -500);

	m_pUI.push_back(pFactory->CreateBitmapObject(1920, 1080, L"../Data/Image/InGame_Clear.png"));
	m_pUI[11]->SetBitmapPosition(0, -2000);

	m_pUI.push_back(pFactory->CreateBitmapObject(573, 48, L"../Data/Image/MainGame_Clear_Text.png"));
	m_pUI[12]->SetBitmapPosition(0, -1000);

	SoundManager::GetInstance()->Play(0, "BGM_Ending");
}

void MainEndCutScene::Update(HWND hWnd, SceneManager* pSceneManager, GameTimer* pTimer, D3Camera* pCamera)
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

	if (m_pUI[m_NowTextIndex]->GetPick() == true)
	{
		if (m_NowTextIndex < 10)
		{
			m_pUI[m_NowTextIndex]->SetBitmapPosition(52, -500);
			m_pUI[m_NowTextIndex + 1]->SetBitmapPosition(52, 828);
			m_NowTextIndex++;
		}
	}

	if (m_NowTextIndex > 7)
	{
		m_MainCutImageOn = true;
	}

	if (m_NowTextIndex == 10)
	{
		m_ClearImageDelayTime += pTimer->DeltaTime();

		if (2.0f < m_ClearImageDelayTime)
		{
			m_SendLetterImage = true;
			SoundManager::GetInstance()->Play(1, "SendLetter");
			m_pUI[11]->SetBitmapPosition(0, 0);
			m_pUI[12]->SetBitmapPosition(650, 930);
		}
	}

	if (pSceneManager->m_IsMouseUp)
	{
		m_pUI[12]->CheckUIPick(m_MousePos.x, m_MousePos.y);
	}

	if (m_pUI[12]->GetPick() == true)
	{
		m_NextScene = true;
	}

	ChangeSceneState(pSceneManager);
}

void MainEndCutScene::Render(DXRenderer* pRenderer)
{
	m_pUI[0]->Render();

	if (m_MainCutImageOn == true)
	{
		m_pUI[1]->Render();
		m_pUI[2]->Render();
	}
	else
	{
		m_pUI[3]->Render();
		m_pUI[4]->Render();
	}

	m_pUI[5]->Render();
	m_pUI[6]->Render();
	m_pUI[7]->Render();
	m_pUI[8]->Render();
	m_pUI[9]->Render();

	if (m_SendLetterImage == true)
	{
		m_pUI[11]->Render();
	}
	else
	{
		m_pUI[10]->Render();
	}

	m_pUI[12]->Render();

	//SimpleMath::Vector4 black(0.0f, 0.0f, 0.0f, 1.0f);
	//
	//pRenderer->DrawText(30, 50, black, (TCHAR*)L"m_NowTextIndex : %d", (TCHAR*)m_NowTextIndex);
}

void MainEndCutScene::Finalize()
{
	for (UINT i = 0; i < m_pUI.size(); i++)
	{
		m_pUI[i]->Finalize();
	}

	m_pUI.clear();
}

void MainEndCutScene::ChangeSceneState(SceneManager* pSceneManager)
{
	// ¾À ÀüÈ¯
	if (m_NextScene)
	{
		SoundManager::GetInstance()->Stop(1, "SendLetter");
		pSceneManager->ChangeScene(eSceneState::THANKSPLAY);
	}
}