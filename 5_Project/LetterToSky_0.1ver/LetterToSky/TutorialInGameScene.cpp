#include "TutorialInGameScene.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "GameTimer.h"

// DirectX
#include "DXFactory.h"
#include "IDXObjectBase.h"
#include "D3Camera.h"
#include "DXRenderer.h"
#include "Transform.h"
#include "BoxCollider.h"
#include "../DXTK/Inc/SimpleMath.h"
using namespace DirectX;

// Object
#include "Stair.h"
#include "Bird.h"
#include "PlayerNew.h"
#include "ConstValue.h"

#include "SoundManager.h"

bool TutorialInGameScene::m_CharacterMoveStart = false;

TutorialInGameScene::TutorialInGameScene()
	: m_pPlayerNew(nullptr),
	m_NowTextIndex(7),
	m_pResetBirds(nullptr),
	m_pResetForwardClouds(nullptr),
	m_pResetBackwardClouds(nullptr),
	m_ClearDelayTime(0.0f),
	m_GameClearImageOn(false),
	m_GameClear(false),
	m_pClearStage(nullptr),
	CharacterY(0.0f),
	m_TimeLimit(0.0f),
	m_TimeOut(false)
{
	LetterBasic = true;
	Letter8 = false;
	Letter6 = false;
	Letter4 = false;
	Letter2 = false;
}

TutorialInGameScene::~TutorialInGameScene()
{

}

void TutorialInGameScene::Initialize(DXDevice* pDevice, DXFactory* pFactory, ResourceManager* pResourceManager)
{
	m_pBackground = pResourceManager->m_pTutoBackgroundList;

	// Objects 생성
	m_pBackground[0]->SetPosition(0.0f, -180.0f, 100.0f);
	m_pBackground[1]->SetPosition(30.0f, 30.0f, 90.0f);
	m_pBackground[2]->SetPosition(-20.0f, 40.0f, 130.0f);
	m_pBackground[3]->SetPosition(30.0f, 20.0f, 120.0f);
	m_pBackground[3]->SetScale(3.0f, 3.0f, 3.0f);
	m_pBackground[3]->SetRotation('Y', 180.0f);
	m_pBackground[4]->SetPosition(-70.0f, 100.0f, 430.0f);
	m_pBackground[5]->SetPosition(20.0f, 120.0f, 400.0f);
	m_pBackground[5]->SetScale(3.0f, 3.0f, 3.0f);
	m_pBackground[5]->SetRotation('Y', 180.0f);
	m_pBackground[6]->SetPosition(-50.0f, 90.0f, 390.0f);
	m_pBackground[6]->SetScale(2.0f, 2.0f, 2.0f);
	m_pBackground[6]->SetRotation('Y', 180.0f);
	m_pBackground[7]->SetPosition(-60.0f, 110.0f, 370.0f);
	m_pBackground[7]->SetScale(1.0f, 1.0f, 1.0f);
	m_pBackground[8]->SetPosition(-30.0f, 0.0f, 10.0f);
	m_pBackground[8]->SetScale(1.0f, 1.0f, 1.0f);
	m_pBackground[8]->SetRotation('Y', -30.0f);
	m_pBackground[9]->SetPosition(-20.0f, 100.0f, 270.0f);
	m_pBackground[9]->SetScale(1.0f, 1.0f, 1.0f);
	m_pBackground[9]->SetRotation('Y', -30.0f);
	m_pBackground[10]->SetPosition(20.0f, 80.0f, 280.0f);
	m_pBackground[10]->SetScale(3.0f, 3.0f, 3.0f);
	m_pBackground[10]->SetRotation('Y', 0.0f);
	m_pBackground[11]->SetPosition(-20.0f, 80.0f, 260.0f);
	m_pBackground[11]->SetScale(3.0f, 3.0f, 3.0f);
	m_pBackground[11]->SetRotation('Y', 0.0f);
	m_pBackground[12]->SetPosition(-30.0f, 85.0f, 260.0f);
	m_pBackground[12]->SetScale(5.0f, 5.0f, 5.0f);
	m_pBackground[12]->SetRotation('Y', 0.0f);
	m_pBackground[13]->SetPosition(30.0f, 130.0f, 400.0f);
	m_pBackground[13]->SetScale(1.0f, 1.0f, 1.0f);
	m_pBackground[13]->SetRotation('Y', 0.0f);
	m_pBackground[14]->SetPosition(-130.0f, 130.0f, 450.0f);
	m_pBackground[14]->SetScale(2.0f, 2.0f, 2.0f);
	m_pBackground[14]->SetRotation('Y', -70.0f);
	m_pBackground[15]->SetPosition(35.0f, 90.0f, 460.0f);
	m_pBackground[15]->SetScale(5.0f, 5.0f, 5.0f);
	m_pBackground[15]->SetRotation('Y', 0.0f);

	// 캐릭터
	m_pPlayerNew = pResourceManager->m_pTutoPlayer;
	m_pPlayerNew->Initialize(0.0f, 2.85f, -8.0f, this);
	m_pPlayerNew->m_pPlayerList[0]->SetRotation('Y', BACKWARD);
	m_pPlayerNew->SetTextState(eTextState::TextOn);

	// 캐릭터와 세이브포인트 관련
	AddSavePoint("Base", m_pPlayerNew->GetPosition());
	AddSavePoint("SavePoint01", Vector3(0.0f, 36.5f, 96.0f));
	AddSavePoint("SavePoint02", Vector3(0.0f, 69.5f, 200.0f));
	AddSavePoint("Cheat", Vector3(0.0f, 101.7f, 305.0f));

	// 첫 번째 세이브포인트를 캐릭터에 넣어준다.
	m_pPlayerNew->SetNowSavePoint(m_pVecSavePoints[(int)eSavePoint::Base]);

	m_pStairList = pResourceManager->m_pTutoStairList;
	m_pStairList[0]->Initialize(0.0f, 2.7f, -3.0f);

	/// 지역 1
	//////////////////////////////////////////////////////////////////////////
	m_pStairList[1]->Initialize(-8.0f, 3.0f, 8.0f);
	m_pStairList[2]->Initialize(0.0f, 3.0f, 8.0f);
	m_pStairList[3]->Initialize(8.0f, 3.0f, 8.0f);
	m_pStairList[4]->Initialize(-8.0f, 6.0f, 16.0f);
	m_pStairList[5]->Initialize(0.0f, 6.0f, 16.0f);
	m_pStairList[6]->Initialize(8.0f, 6.0f, 16.0f);
	m_pStairList[7]->Initialize(-8.0f, 9.0f, 24.0f);
	m_pStairList[8]->Initialize(0.0f, 9.0f, 24.0f);
	m_pStairList[9]->Initialize(8.0f, 9.0f, 24.0f);
	m_pStairList[10]->Initialize(-8.0f, 12.0f, 32.0f);
	m_pStairList[11]->Initialize(0.0f, 12.0f, 32.0f);
	m_pStairList[12]->Initialize(8.0f, 12.0f, 32.0f);
	m_pStairList[13]->Initialize(-8.0f, 15.0f, 40.0f);
	m_pStairList[14]->Initialize(0.0f, 15.0f, 40.0f);
	m_pStairList[15]->Initialize(8.0f, 15.0f, 40.0f);
	m_pStairList[16]->Initialize(-8.0f, 18.0f, 48.0f);
	m_pStairList[17]->Initialize(0.0f, 18.0f, 48.0f);
	m_pStairList[18]->Initialize(8.0f, 18.0f, 48.0f);
	m_pStairList[19]->Initialize(0.0f, 21.0f, 56.0f);
	m_pStairList[20]->Initialize(-8.0f, 24.0f, 64.0f);
	m_pStairList[21]->Initialize(0.0f, 24.0f, 64.0f);
	m_pStairList[22]->Initialize(8.0f, 24.0f, 64.0f);
	m_pStairList[23]->Initialize(0.0f, 27.0f, 72.0f);
	m_pStairList[24]->Initialize(-8.0f, 30.0f, 80.0f);
	m_pStairList[25]->Initialize(0.0f, 30.0f, 80.0f);
	m_pStairList[26]->Initialize(8.0f, 30.0f, 80.0f);
	m_pStairList[27]->Initialize(0.0f, 35.5f, 96.0f);

	m_pResetBirds = new BoxCollider(pDevice);
	m_pResetBirds->Initialize(5.0f, 20.0f, 20.f);
	m_pResetBirds->SetPosition(40.0f, 35.0f, 72.0f);

	// 새
	m_pBirdList = pResourceManager->m_pTutoBirdList;
	m_pBirdList[0]->Initialize(-40.0f, 29.0f, 64.0f); // 이전 수치 (15.0f, 29.0f, 64.0f)
	m_pBirdList[1]->Initialize(-40.0f, 32.0f, 72.0f); // 이전 수치 (15.0f, 32.0f, 64.0f)
	m_pBirdList[2]->Initialize(-40.0f, 35.0f, 80.0f); // 이전 수치 (15.0f, 35.0f, 64.0f)
	//////////////////////////////////////////////////////////////////////////


	/// 지역 2
	//////////////////////////////////////////////////////////////////////////
	m_pResetForwardClouds = new BoxCollider(pDevice);
	m_pResetForwardClouds->Initialize(5.0f, 20.0f, 30.f);
	m_pResetForwardClouds->SetPosition(20.0f, 54.0f, 165.0f);

	m_pResetBackwardClouds = new BoxCollider(pDevice);
	m_pResetBackwardClouds->Initialize(5.0f, 20.0f, 30.f);
	m_pResetBackwardClouds->SetPosition(-20.0f, 54.0f, 165.0f);

	m_pStairList[28]->Initialize(-8.0f, 36.0f, 112.0f);
	m_pStairList[29]->Initialize(0.0f, 36.0f, 112.0f);
	m_pStairList[30]->Initialize(8.0f, 36.0f, 112.0f);
	m_pStairList[31]->Initialize(-8.0f, 39.0f, 120.0f);
	m_pStairList[32]->Initialize(0.0f, 39.0f, 120.0f);
	m_pStairList[33]->Initialize(8.0f, 39.0f, 120.0f);
	m_pStairList[34]->Initialize(-8.0f, 42.0f, 128.0f);
	m_pStairList[35]->Initialize(0.0f, 42.0f, 128.0f);
	m_pStairList[36]->Initialize(8.0f, 42.0f, 128.0f);
	m_pStairList[37]->Initialize(-8.0f, 45.0f, 136.0f);
	m_pStairList[38]->Initialize(0.0f, 45.0f, 136.0f);
	m_pStairList[39]->Initialize(8.0f, 45.0f, 136.0f);
	m_pStairList[40]->Initialize(-8.0f, 48.0f, 144.0f);
	m_pStairList[41]->Initialize(0.0f, 48.0f, 144.0f);
	m_pStairList[42]->Initialize(8.0f, 48.0f, 144.0f);
	m_pStairList[43]->Initialize(-32.0f, 51.0f, 152.0f);
	m_pStairList[44]->Initialize(-32.0f, 51.0f, 152.0f);
	m_pStairList[45]->Initialize(-32.0f, 51.0f, 152.0f);
	m_pStairList[46]->Initialize(-32.0f, 51.0f, 152.0f);
	m_pStairList[47]->Initialize(32.0f, 54.0f, 160.0f);
	m_pStairList[48]->Initialize(32.0f, 54.0f, 160.0f);
	m_pStairList[49]->Initialize(32.0f, 54.0f, 160.0f);
	m_pStairList[50]->Initialize(-32.0f, 57.0f, 168.0f);
	m_pStairList[51]->Initialize(-32.0f, 57.0f, 168.0f);
	m_pStairList[52]->Initialize(-32.0f, 57.0f, 168.0f);
	m_pStairList[53]->Initialize(-32.0f, 57.0f, 168.0f);
	m_pStairList[54]->Initialize(32.0f, 60.0f, 176.0f);
	m_pStairList[55]->Initialize(32.0f, 60.0f, 176.0f);
	m_pStairList[56]->Initialize(32.0f, 60.0f, 176.0f);
	m_pStairList[57]->Initialize(-8.0f, 63.0f, 184.0f);
	m_pStairList[58]->Initialize(0.0f, 63.0f, 184.0f);
	m_pStairList[59]->Initialize(8.0f, 63.0f, 184.0f);
	m_pStairList[60]->Initialize(0.0f, 68.5f, 200.0f);
	//////////////////////////////////////////////////////////////////////////


	/// 지역 3
	//////////////////////////////////////////////////////////////////////////
	m_pStairList[61]->Initialize(-8.0f, 69.0f, 216.0f);
	m_pStairList[62]->Initialize(0.0f, 69.0f, 216.0f);
	m_pStairList[63]->Initialize(8.0f, 69.0f, 216.0f);
	m_pStairList[64]->Initialize(0.0f, 72.0f, 224.0f);
	m_pStairList[65]->Initialize(0.0f, 75.0f, 232.0f);
	m_pStairList[66]->Initialize(-8.0f, 78.0f, 240.0f);
	m_pStairList[67]->Initialize(0.0f, 81.0f, 248.0f);
	m_pStairList[68]->Initialize(8.0f, 81.0f, 248.0f);
	m_pStairList[69]->Initialize(0.0f, 87.0f, 264.0f);
	m_pStairList[70]->Initialize(0.0f, 90.0f, 272.0f);
	m_pStairList[71]->Initialize(0.0f, 93.0f, 280.0f);
	m_pStairList[72]->Initialize(-8.0f, 96.0f, 288.0f);
	m_pStairList[73]->Initialize(0.0f, 96.0f, 288.0f);
	m_pStairList[74]->Initialize(8.0f, 96.0f, 288.0f);
	m_pStairList[75]->Initialize(0.0f, 101.0f, 328.0f);

	m_pClearStage = new BoxCollider(pDevice);
	m_pClearStage->Initialize(4.0f, 7.0f, 4.0f);
	m_pClearStage->SetPosition(0.0f, 105.0f, 332.0f);
	//////////////////////////////////////////////////////////////////////////

	m_pUI = pResourceManager->m_pTutoUI;
	m_pUI[0]->SetBitmapPosition(40, 70);
	m_pUI[1]->SetBitmapPosition(55, 850);
	m_pUI[2]->SetBitmapPosition(1620, 815);
	m_pUI[3]->SetBitmapPosition(1620, 815);
	m_pUI[4]->SetBitmapPosition(1405, 50);	// Cursor UI
	m_pUI[5]->SetBitmapPosition(0, -500);	// Cursor UI
	m_pUI[6]->SetBitmapPosition(0, -500);	// Cursor UI
	m_pUI[7]->SetBitmapPosition(52, 826);	// Text UI 01
	m_pUI[8]->SetBitmapPosition(52, -500);	// Text UI 02
	m_pUI[9]->SetBitmapPosition(52, -500);	// Text UI 03
	m_pUI[10]->SetBitmapPosition(52, -500);	// Text UI 04

	m_pUI[11]->SetBitmapPosition(1620, -1000);
	m_pUI[12]->SetBitmapPosition(1620, -1000);
	m_pUI[13]->SetBitmapPosition(1620, -1000);
	m_pUI[14]->SetBitmapPosition(1620, -1000);

	m_pUI[15]->SetBitmapPosition(0, 0);
	m_pUI[16]->SetBitmapPosition(770, 700);

	m_pUI[17]->SetBitmapPosition(52, -500);	// Text UI 05

	/// <summary>
	/// 지연 카메라의 초깃값
	/// </summary>
	XMFLOAT3 cameraPos = XMFLOAT3(8.0f, 4.0f, -20.0f);
	cameraPos.x += m_pPlayerNew->m_pPlayerList[0]->GetPosition().x;
	cameraPos.y += m_pPlayerNew->m_pPlayerList[0]->GetPosition().y;
	cameraPos.z += m_pPlayerNew->m_pPlayerList[0]->GetPosition().z;

	m_DelayCamPos = cameraPos;

	SoundManager::GetInstance()->Play(3, "BGM_InGame");
}

void TutorialInGameScene::Update(HWND hWnd, SceneManager* pSceneManager, GameTimer* pTimer, D3Camera* pCamera)
{
	// MousePos를 얻어옴
	GetCursorPos(&m_MousePos);
	ScreenToClient(hWnd, &m_MousePos);

	if (pSceneManager->m_IsMouseUp)
	{
		m_pUI[7]->CheckUIPick(m_MousePos.x, m_MousePos.y);
	}

	if (m_pUI[7]->GetPick() == true)
	{
		SoundManager::GetInstance()->Play(1, "Button");
		m_pUI[7]->SetBitmapPosition(52, -500);
		m_pUI[8]->SetBitmapPosition(290, 416);
		m_pUI[4]->SetBitmapPosition(0, -500);
		m_pUI[5]->SetBitmapPosition(160, 448);
		m_NowTextIndex = 8;
		SoundManager::GetInstance()->Stop(1, "Button");
	}

	if (pSceneManager->m_IsMouseUp)
	{
		m_pUI[8]->CheckUIPick(m_MousePos.x, m_MousePos.y);
	}

	if (m_pUI[8]->GetPick() == true)
	{
		SoundManager::GetInstance()->Play(1, "Button");
		m_pUI[8]->SetBitmapPosition(52, -500);
		m_pUI[9]->SetBitmapPosition(52, 826);
		m_pUI[5]->SetBitmapPosition(0, -500);
		m_pUI[6]->SetBitmapPosition(1521, 870);
		m_NowTextIndex = 9;
		SoundManager::GetInstance()->Stop(1, "Button");
	}

	if (pSceneManager->m_IsMouseUp)
	{
		m_pUI[9]->CheckUIPick(m_MousePos.x, m_MousePos.y);
	}

	if (m_pUI[9]->GetPick() == true)
	{
		SoundManager::GetInstance()->Play(1, "Button");
		m_pUI[9]->SetBitmapPosition(52, -500);
		m_pUI[10]->SetBitmapPosition(52, 826);
		m_pUI[6]->SetBitmapPosition(0, -500);
		m_NowTextIndex = 10;
		SoundManager::GetInstance()->Stop(1, "Button");
	}

	if (pSceneManager->m_IsMouseUp)
	{
		m_pUI[10]->CheckUIPick(m_MousePos.x, m_MousePos.y);
	}

	if (m_NowTextIndex == 10)
	{
		m_pPlayerNew->SetTextState(eTextState::TextOff);
	}

	for (UINT i = 0; i < m_pBackground.size(); i++)
	{
		m_pBackground[i]->Update(pTimer, pCamera);
	}

	/// <summary>
	/// Player(New)
	/// </summary>
	// 모든 계단에 대한 플레이어 클래스의 상호작용
	for (UINT i = 0; i < m_pStairList.size(); i++)
	{
		for (UINT j = 0; j < m_pPlayerNew->m_pPlayerList.size(); j++)
		{
			m_pStairList[i]->Update(pTimer, pCamera, m_pPlayerNew);
		}
	}

	// 캐릭터 위치 조정
	m_pPlayerNew->KeyInput(pTimer->DeltaTime());
	m_pPlayerNew->ProcessMove(pTimer->DeltaTime());
	this->CheckPlayerPos();
	m_pPlayerNew->Update(pTimer, pCamera);

	// 구름 기믹 위치 리셋시키는 충돌체크 박스
	m_pResetForwardClouds->Update(pCamera);
	m_pResetBackwardClouds->Update(pCamera);

	// 새 오브젝트 위치 리셋시키는 충돌체크 박스
	m_pResetBirds->Update(pCamera);

	for (UINT i = 0; i < m_pBirdList.size(); i++)
	{
		m_pBirdList[i]->Update(pTimer, pCamera);
	}

	m_TimeLimit += pTimer->DeltaTime();

	// 5분이 지나면
	if (300.0f < m_TimeLimit)	// 임시 테스트 10초 -> 기존 300.0f
	{
		m_TimeImageOn = true;

		if (pSceneManager->m_IsMouseUp)
		{
			m_pUI[16]->CheckUIPick(m_MousePos.x, m_MousePos.y);
		}

		if (m_pUI[16]->GetPick() == true)
		{
			m_TimeOut = true;
		}
	}

	for (UINT i = 0; i < m_pUI.size(); i++)
	{
		m_pUI[i]->Update(pTimer, pCamera);
	}

	CharacterY = (int)m_pPlayerNew->m_pPlayerList[0]->GetPosition().y;

	switch (CharacterY)
	{
		case 0:
			m_pUI[1]->SetBitmapPosition(55, 850);
			break;

		case 10:
			m_pUI[1]->SetBitmapPosition(55, 800);
			break;

		case 20:
			m_pUI[1]->SetBitmapPosition(55, 750);
			break;

		case 36:
			m_pUI[1]->SetBitmapPosition(55, 650);
			break;

		case 45:
			m_pUI[1]->SetBitmapPosition(55, 580);
			break;

		case 55:
			m_pUI[1]->SetBitmapPosition(55, 530);
			break;

		case 65:
			m_pUI[1]->SetBitmapPosition(55, 450);
			break;

		case 70:
			m_pUI[1]->SetBitmapPosition(55, 400);
			break;

		case 80:
			m_pUI[1]->SetBitmapPosition(55, 300);
			break;

		case 90:
			m_pUI[1]->SetBitmapPosition(55, 200);
			break;

		case 101:
			m_pUI[1]->SetBitmapPosition(55, 120);
			break;
	}

	/// 지역 1
	//////////////////////////////////////////////////////////////////////////
	m_pStairList[8]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[14]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[15]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[19]->Strafe(-8.0f, 8.0f, 8.0f * pTimer->DeltaTime(), true, m_pPlayerNew);
	m_pStairList[20]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[22]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[23]->Strafe(8.0f, -8.0f, 8.0f * pTimer->DeltaTime(), false, m_pPlayerNew);
	m_pStairList[24]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[25]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[26]->Fall(-15.0f, m_pPlayerNew);

	m_pBirdList[0]->GetBirdObject()->SetTranslation(10.0f * pTimer->DeltaTime(), 0.0f, 0.0f);
	m_pBirdList[1]->GetBirdObject()->SetTranslation(30.0f * pTimer->DeltaTime(), 0.0f, 0.0f);
	m_pBirdList[2]->GetBirdObject()->SetTranslation(15.0f * pTimer->DeltaTime(), 0.0f, 0.0f);

	if (m_pBirdList[0]->GetBirdObject()->GetBoxCollider()->CheckCollided(m_pResetBirds))
	{
		m_pBirdList[0]->Reset();
	}

	if (m_pBirdList[1]->GetBirdObject()->GetBoxCollider()->CheckCollided(m_pResetBirds))
	{
		m_pBirdList[1]->Reset();
	}

	if (m_pBirdList[2]->GetBirdObject()->GetBoxCollider()->CheckCollided(m_pResetBirds))
	{
		m_pBirdList[2]->Reset();
	}

	/// <summary>
	/// 새 등의 적과 플레이어의 상호작용
	/// Player(New)
	/// </summary>
	for (UINT i = 0; i < m_pPlayerNew->m_pPlayerList.size(); i++)
	{
		if (m_pPlayerNew->m_pPlayerList[i]->GetBoxCollider()->CheckCollided(m_pBirdList[0]->GetBirdObject()->GetBoxCollider()))
		{
			m_pPlayerNew->SetGameState(ePlayerGameState::OnHit);
			m_pPlayerNew->ResetBySavePoint();
		}

		if (m_pPlayerNew->m_pPlayerList[i]->GetBoxCollider()->CheckCollided(m_pBirdList[1]->GetBirdObject()->GetBoxCollider()))
		{
			m_pPlayerNew->SetGameState(ePlayerGameState::OnHit);
			m_pPlayerNew->ResetBySavePoint();
		}

		if (m_pPlayerNew->m_pPlayerList[i]->GetBoxCollider()->CheckCollided(m_pBirdList[2]->GetBirdObject()->GetBoxCollider()))
		{
			m_pPlayerNew->SetGameState(ePlayerGameState::OnHit);
			m_pPlayerNew->ResetBySavePoint();
		}
	}
	//////////////////////////////////////////////////////////////////////////


	/// ///////////////////////////////////////////////////////////////////////
	/// <summary>
	/// 특정 지점에 도달하면 세이브포인트 저장(변경) 등등..
	/// </summary>
	// Save Point 01 구름에 도달했으므로 캐릭터의 Reset Position을 변경한다
	if (m_pStairList[27]->GetStairObject()->GetBoxCollider()->CheckCollided(m_pPlayerNew->m_pPlayerList[0]->GetBoxCollider()))
	{
		m_pPlayerNew->SetNowSavePoint(m_pVecSavePoints[(int)eSavePoint::SavePoint01]);
	}


	/// 지역 2
	//////////////////////////////////////////////////////////////////////////
	m_pStairList[32]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[34]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[36]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[38]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[40]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[42]->Fall(-15.0f, m_pPlayerNew);

	Vector3 MoveVector(5.0f * pTimer->DeltaTime(), 0.0f, 0.0f);
	Vector3 MoveInverseVector(-5.0f * pTimer->DeltaTime(), 0.0f, 0.0f);

	m_pStairList[43]->Move(MoveVector, m_pResetForwardClouds, 0, m_pPlayerNew);
	m_pStairList[44]->Move(MoveVector, m_pResetForwardClouds, 1, m_pPlayerNew);
	m_pStairList[45]->Move(MoveVector, m_pResetForwardClouds, 2, m_pPlayerNew);
	m_pStairList[46]->Move(MoveVector, m_pResetForwardClouds, 3, m_pPlayerNew);
	m_pStairList[47]->Move(MoveInverseVector, m_pResetBackwardClouds, 0, m_pPlayerNew);
	m_pStairList[48]->Move(MoveInverseVector, m_pResetBackwardClouds, 1, m_pPlayerNew);
	m_pStairList[49]->Move(MoveInverseVector, m_pResetBackwardClouds, 2, m_pPlayerNew);
	m_pStairList[50]->Move(MoveVector, m_pResetForwardClouds, 0, m_pPlayerNew);
	m_pStairList[51]->Move(MoveVector, m_pResetForwardClouds, 1, m_pPlayerNew);
	m_pStairList[52]->Move(MoveVector, m_pResetForwardClouds, 2, m_pPlayerNew);
	m_pStairList[53]->Move(MoveVector, m_pResetForwardClouds, 3, m_pPlayerNew);
	m_pStairList[54]->Move(MoveInverseVector, m_pResetBackwardClouds, 0, m_pPlayerNew);
	m_pStairList[55]->Move(MoveInverseVector, m_pResetBackwardClouds, 1, m_pPlayerNew);
	m_pStairList[56]->Move(MoveInverseVector, m_pResetBackwardClouds, 2, m_pPlayerNew);

	/// <summary>
	/// 이동하는 구름을 타고 가다가, 계단 범위 바깥으로 이동하면 리셋되는 
	/// </summary>

	Vector3 fallForwardPosition = Vector3(m_pResetForwardClouds->GetPosition().x,
		m_pResetForwardClouds->GetPosition().y, m_pResetForwardClouds->GetPosition().z);

	Vector3 fallBackwardPosition = Vector3(m_pResetBackwardClouds->GetPosition().x,
		m_pResetBackwardClouds->GetPosition().y, m_pResetBackwardClouds->GetPosition().z);

	if (m_pPlayerNew->m_pPlayerList[0]->GetBoxCollider()->CheckCollided(m_pResetForwardClouds))
	{
		m_pPlayerNew->SetGameState(ePlayerGameState::Dying);
		m_pPlayerNew->m_pPlayerList[2]->SetPosition(fallBackwardPosition.x,
			fallBackwardPosition.y, fallBackwardPosition.z);

		if (m_pPlayerNew->m_pPlayerList[2]->GetPosition().y < -20.0f)
		{
			m_pPlayerNew->ResetBySavePoint();
		}
	}

	if (m_pPlayerNew->m_pPlayerList[0]->GetBoxCollider()->CheckCollided(m_pResetBackwardClouds))
	{
		m_pPlayerNew->SetGameState(ePlayerGameState::Dying);
		m_pPlayerNew->m_pPlayerList[2]->SetPosition(fallForwardPosition.x,
			fallForwardPosition.y, fallForwardPosition.z);

		if (m_pPlayerNew->m_pPlayerList[2]->GetPosition().y < -20.0f)
		{
			m_pPlayerNew->ResetBySavePoint();
		}
	}
	//////////////////////////////////////////////////////////////////////////


	// new
	// Save Point 02 구름에 도달했으므로 캐릭터의 Reset Position을 변경한다
	if (m_pStairList[60]->GetStairObject()->GetBoxCollider()->CheckCollided(m_pPlayerNew->m_pPlayerList[0]->GetBoxCollider()))
	{
		m_pPlayerNew->SetNowSavePoint(m_pVecSavePoints[(int)eSavePoint::SavePoint02]);
	}


	/// 지역 3
	//////////////////////////////////////////////////////////////////////////
	m_pStairList[64]->FallStrafe(m_pPlayerNew, 15.0f, -8.0f, 8.0f, 5.0f * pTimer->DeltaTime(), true);

	m_pStairList[66]->StrafeDiagonal(-8.0f, -8.0f, 75.0f, 81.0f, 232.0f, 248.0f, 0.5f * pTimer->DeltaTime(), m_pPlayerNew);
	m_pStairList[68]->StrafeDiagonal(8.0f, 8.0f, 78.0f, 84.0f, 240.0f, 256.0f, 0.5f * pTimer->DeltaTime(), m_pPlayerNew);

	m_pStairList[69]->Strafe(8.0f, -8.0f, 10.0f * pTimer->DeltaTime(), false, m_pPlayerNew);
	m_pStairList[71]->FallStrafe(m_pPlayerNew, 30.0f, -8.0f, 8.0f, 5.0f * pTimer->DeltaTime(), true);
	//////////////////////////////////////////////////////////////////////////

	CameraMove(pCamera, pTimer->DeltaTime());

	m_pClearStage->Update(pCamera);

	/// 게임 클리어 조건
	if (m_pPlayerNew->m_pPlayerList[0]->GetBoxCollider()->CheckCollided(m_pClearStage))
	{
		SoundManager::GetInstance()->Play(2, "GameClear");

		m_pUI[17]->SetBitmapPosition(52, 826);	// 임시(완료대사)
		m_GameClearImageOn = true;
		m_pPlayerNew->m_pPlayerList[4]->SetRotation('Y', BACKWARD);
		m_pPlayerNew->SetGameState(ePlayerGameState::Clear);

		m_ClearDelayTime += pTimer->DeltaTime();

		if (4.0f < m_ClearDelayTime)
		{
			m_GameClear = true;
		}
	}

	if (GetAsyncKeyState(VK_F7) & 0x0001)
	{
		m_pPlayerNew->SetNowSavePoint(m_pVecSavePoints[(int)eSavePoint::Cheat]);
	}

	ChangeSceneState(pSceneManager);
}

void TutorialInGameScene::Render(DXRenderer* pRenderer)
{
	for (UINT i = 0; i < m_pBackground.size(); i++)
	{
		m_pBackground[i]->Render();
	}

	//m_pResetForwardClouds->Render();
	//m_pResetBackwardClouds->Render();

	for (UINT i = 0; i < m_pStairList.size(); i++)
	{
		m_pStairList[i]->Render();
	}

	//m_pResetBirds->Render();

	for (UINT i = 0; i < m_pBirdList.size(); i++)
	{
		m_pBirdList[i]->Render();
	}

	m_pPlayerNew->Render();

	//m_pClearStage->Render();

	m_pUI[0]->Render();
	m_pUI[1]->Render();
	m_pUI[2]->Render();

	m_pUI[3]->Render();
	m_pUI[11]->Render();
	m_pUI[12]->Render();
	m_pUI[13]->Render();
	m_pUI[14]->Render();

	if (m_CharacterMoveStart == false)
	{
		m_pUI[10]->Render();
	}

	m_pUI[4]->Render();
	m_pUI[5]->Render();
	m_pUI[6]->Render();
	m_pUI[7]->Render();
	m_pUI[8]->Render();
	m_pUI[9]->Render();


	if (m_TimeImageOn == true)
	{
		m_pUI[15]->Render();
		m_pUI[16]->Render();
	}

	if (m_GameClearImageOn == true)
	{
		m_pUI[17]->Render();
	}

	//SimpleMath::Vector4 green(0.0f, 1.0f, 0.0f, 1.0f);
	//SimpleMath::Vector4 black(0.0f, 0.0f, 0.0f, 1.0f);
	//
	//pRenderer->DrawText(60, 50, black, (TCHAR*)L"GameState : %s", (TCHAR*)m_pPlayerNew->GetGameState_Debug().c_str());
	//pRenderer->DrawText(60, 70, black, (TCHAR*)L"Direction : %s", (TCHAR*)m_pPlayerNew->GetDirection_Debug().c_str());
	//pRenderer->DrawText(60, 90, black, (TCHAR*)L"m_NowTextIndex : %d", (TCHAR*)m_NowTextIndex);
	//
	//pRenderer->DrawText(60, 130, black, (TCHAR*)L"Charater Pos X : %f", m_pPlayerNew->GetPosition().x);
	//pRenderer->DrawText(60, 150, black, (TCHAR*)L"Charater Pos Y : %f", m_pPlayerNew->GetPosition().y);
	//pRenderer->DrawText(60, 170, black, (TCHAR*)L"Charater Pos Z : %f", m_pPlayerNew->GetPosition().z);
	//
	//pRenderer->DrawText(60, 190, black, (TCHAR*)L"TimeLimit : %f", m_TimeLimit);
}

void TutorialInGameScene::Finalize()
{
	for (UINT i = 0; i < m_pUI.size(); i++)
	{
		m_pUI[i]->Finalize();
	}

	m_pUI.clear();

	for (UINT i = 0; i < m_pBirdList.size(); i++)
	{
		m_pBirdList[i]->Finalize();
	}

	m_pBirdList.clear();

	for (UINT i = 0; i < m_pStairList.size(); i++)
	{
		m_pStairList[i]->Finalize();
	}

	m_pStairList.clear();

	for (UINT i = 0; i < m_pBackground.size(); i++)
	{
		m_pBackground[i]->Finalize();
	}

	m_pBackground.clear();

	m_pPlayerNew->Finalize();
	delete m_pPlayerNew;
	m_pPlayerNew = nullptr;

	delete m_pResetBirds;
	m_pResetBirds = nullptr;

	delete m_pResetForwardClouds;
	m_pResetForwardClouds = nullptr;

	delete m_pResetBackwardClouds;
	m_pResetBackwardClouds = nullptr;

	delete m_pClearStage;
	m_pClearStage = nullptr;
}

void TutorialInGameScene::ChangeSceneState(SceneManager* pSceneManager)
{
	// 씬 전환
	// 게임을 클리어하면 엔딩 컷 씬으로 이동
	if (m_GameClear)
	{
		SoundManager::GetInstance()->Stop(2, "GameClear");
		SoundManager::GetInstance()->Pause(3, "BGM_InGame");
		pSceneManager->ChangeScene(eSceneState::TUTO_END_CUT);
	}

	// 타임아웃 되서 다시 타이틀로
	if (m_TimeOut)
	{
		SoundManager::GetInstance()->Stop(2, "GameClear");
		SoundManager::GetInstance()->Pause(3, "BGM_InGame");
		pSceneManager->ChangeScene(eSceneState::TITLE);
	}
}

void TutorialInGameScene::CameraMove(D3Camera* pCamera, float dTime)
{
	// new
	if (pCamera->GetIndex() == 1)
	{
		XMFLOAT3 cameraPos = XMFLOAT3(8.0f, 4.0f, -20.0f);
		cameraPos.x += m_pPlayerNew->m_pPlayerList[0]->GetPosition().x;
		cameraPos.y += m_pPlayerNew->m_pPlayerList[0]->GetPosition().y;
		cameraPos.z += m_pPlayerNew->m_pPlayerList[0]->GetPosition().z;

		XMFLOAT3 target = XMFLOAT3(0.0f, 2.5f, 0.0f);
		target.x += m_pPlayerNew->m_pPlayerList[0]->GetPosition().x;
		target.y += m_pPlayerNew->m_pPlayerList[0]->GetPosition().y;
		target.z += m_pPlayerNew->m_pPlayerList[0]->GetPosition().z;

		Vector3 _distVec = cameraPos - m_DelayCamPos;
		m_DelayCamPos += _distVec * dTime * 1.0f; // 초기값 2.0f

		pCamera->LookAt(m_DelayCamPos, target, XMFLOAT3(0, 1.0f, 0));
	}
}

void TutorialInGameScene::AddSavePoint(std::string name, Vector3 pos)
{
	SavePoint* _newSavePoint = new SavePoint();
	_newSavePoint->m_Name = name;
	_newSavePoint->m_Position = pos;

	m_pVecSavePoints.push_back(_newSavePoint);
}

/// <summary>
/// 전체 계단만 돌면서
/// x/z 위치를 가지고서 충돌감지
/// 
/// 결과적으로 캐릭터가 '딛고' 서야 할 위치를 알려준다.
/// 
/// 충돌감지 위치가 없으면 없다고 리턴
/// </summary>
/// <param name="position">결과적으로 받아 갈 위치 (y값이 중요)</param>
/// <param name="x">캐릭터의 x좌표값</param>
/// <param name="z">캐릭터의 z좌표값</param>
/// <returns></returns>
bool TutorialInGameScene::GetCollisionPos(Vector3* position)
{
	DirectX::BoundingBox _boundingBox;
	for (UINT i = 0; i < m_pStairList.size(); i++)
	{
		_boundingBox = m_pStairList[i]->GetStairObject()->GetBoxCollider()->GetBBForCollisionDetection();

		Vector4 _origin(position->x, 1000.0f, position->z, 1.0f);
		Vector4 _direction(0, -1.0f, 0, 1.0f);
		float _resultDist = 1000.0f;

		// 직선과 충돌을 했다면, 그 위치를 리턴한다.
		if (_boundingBox.Intersects(_origin, _direction, _resultDist) == true)
		{
			//position->y = _origin.y + _resultDist;
			if (m_pStairList[i]->GetType() == eStairType::Start_Base)
			{
				position->y = m_pStairList[i]->GetStairObject()->GetPosition().y + Y_CHARACTER_OFFSET_STARTBASE;
			}
			else if (m_pStairList[i]->GetType() == eStairType::Cloud_Base)
			{
				position->y = m_pStairList[i]->GetStairObject()->GetPosition().y + Y_CHARACTER_OFFSET_CLOUDBASE;
			}
			else if (m_pStairList[i]->GetType() == eStairType::End_Base)
			{
				position->y = m_pStairList[i]->GetStairObject()->GetPosition().y + Y_CHARACTER_OFFSET_ENDBASE;
			}
			else if (m_pStairList[i]->GetType() == eStairType::Cloud)
			{
				position->y = m_pStairList[i]->GetStairObject()->GetPosition().y + Y_CHARACTER_OFFSET_CLOUD;
			}
			else
			{
				position->y = m_pStairList[i]->GetStairObject()->GetPosition().y + Y_CHARACTER_OFFSET_STAIR;
			}

			return true;
		}
	}

	// 충돌을 안했다는 것은, 허공에 떠 있다는 것
	return false;
}

void TutorialInGameScene::CheckPlayerPos()
{
	/// y값을 보정함. 갱신의 필요성이 있을 때 매번 불려짐
	if (m_pPlayerNew->IsYPosNeedRefresh() == true)
	{
		Vector3 _newPos = m_pPlayerNew->GetPosition();

		if (GetCollisionPos(&_newPos) == true)
		{
			// 바닥과의 충돌감지를 성공한 경우는, 그 위치로 만들어주면 되고 (MK1)
			m_pPlayerNew->SetPosition(_newPos);
		}
		else
		{
			// 아닌 경우는, 허공에 떴으니 죽음 체크를 해야 함
			m_pPlayerNew->OnAirCheck();
		}

		m_pPlayerNew->YPosCompleted();
	}
}

BoxCollider* TutorialInGameScene::GetFootStepStair()
{
	// StairList를 돌면서 
	for (UINT i = 0; i < m_pStairList.size(); i++)
	{
		// FootStep과 충돌한 Stair의 BoxCollider를 가져온다
		if (m_pPlayerNew->GetFootStep()->CheckCollided(m_pStairList[i]->GetStairObject()->GetBoxCollider()))
		{
			return m_pStairList[i]->GetStairObject()->GetBoxCollider();
		}
	}

	return nullptr;
}