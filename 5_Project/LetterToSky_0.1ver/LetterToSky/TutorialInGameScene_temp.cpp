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

// Object
#include "Stair.h"
#include "Bird.h"
#include "PlayerOld.h"
#include "Player2.h"


TutorialInGameScene::TutorialInGameScene()
	: m_pSea(nullptr),
	m_pPlayer(nullptr), m_pPlayer2(nullptr),
	m_pResetBirds(nullptr),
	m_pResetForwardClouds(nullptr),
	m_pResetBackwardClouds(nullptr),
	m_GameClear(false),
	m_pClearStage(nullptr)
{

}

TutorialInGameScene::~TutorialInGameScene()
{

}

void TutorialInGameScene::Initialize(DXDevice* pDevice, DXFactory* pFactory, ResourceManager* pResourceManager)
{
	//m_pStairList = pResourceManager->m_pTutoStairList;

	// Objects ����
	m_pSea = pFactory->CreateASEObject((LPSTR)"../Data/ASE/sea_dumi.ASE", L"../Data/Textures/sea.dds");
	m_pSea->SetPosition(0.0f, -180.0f, 100.0f);

	m_pStairList.push_back(new Stair(pFactory, eStairType::START_BASE, eStairState::BASE));
	m_pStairList[0]->Initialize(0.0f, 2.7f, -3.0f);

	/// ���� 1
	//////////////////////////////////////////////////////////////////////////
	m_pStairList.push_back(new Stair(pFactory, eStairType::AREA01_NORMAL, eStairState::NORMAL)); // 1
	m_pStairList.push_back(new Stair(pFactory, eStairType::AREA01_NORMAL, eStairState::NORMAL)); // 2
	m_pStairList.push_back(new Stair(pFactory, eStairType::AREA01_NORMAL, eStairState::NORMAL)); // 3
	m_pStairList.push_back(new Stair(pFactory, eStairType::AREA01_NORMAL, eStairState::NORMAL)); // 4
	m_pStairList.push_back(new Stair(pFactory, eStairType::AREA01_NORMAL, eStairState::NORMAL)); // 5
	m_pStairList.push_back(new Stair(pFactory, eStairType::AREA01_NORMAL, eStairState::NORMAL)); // 6
	m_pStairList.push_back(new Stair(pFactory, eStairType::AREA01_NORMAL, eStairState::NORMAL)); // 7
	m_pStairList.push_back(new Stair(pFactory, eStairType::AREA01_BROKEN, eStairState::FALL)); // 8 �ı�
	m_pStairList.push_back(new Stair(pFactory, eStairType::AREA01_NORMAL, eStairState::NORMAL)); // 9
	m_pStairList.push_back(new Stair(pFactory, eStairType::AREA01_NORMAL, eStairState::NORMAL)); // 10
	m_pStairList.push_back(new Stair(pFactory, eStairType::AREA01_NORMAL, eStairState::NORMAL)); // 11
	m_pStairList.push_back(new Stair(pFactory, eStairType::AREA01_NORMAL, eStairState::NORMAL)); // 12
	m_pStairList.push_back(new Stair(pFactory, eStairType::AREA01_NORMAL, eStairState::NORMAL)); // 13
	m_pStairList.push_back(new Stair(pFactory, eStairType::AREA01_BROKEN, eStairState::FALL)); // 14 �ı�
	m_pStairList.push_back(new Stair(pFactory, eStairType::AREA01_BROKEN, eStairState::FALL)); // 15 �ı�
	m_pStairList.push_back(new Stair(pFactory, eStairType::AREA01_NORMAL, eStairState::NORMAL)); // 16
	m_pStairList.push_back(new Stair(pFactory, eStairType::AREA01_NORMAL, eStairState::NORMAL)); // 17
	m_pStairList.push_back(new Stair(pFactory, eStairType::AREA01_NORMAL, eStairState::NORMAL)); // 18
	m_pStairList.push_back(new Stair(pFactory, eStairType::AREA01_NORMAL, eStairState::STRAFE)); // 19 �̵�
	m_pStairList.push_back(new Stair(pFactory, eStairType::AREA01_BROKEN, eStairState::FALL)); // 20 �ı�
	m_pStairList.push_back(new Stair(pFactory, eStairType::AREA01_NORMAL, eStairState::NORMAL)); // 21
	m_pStairList.push_back(new Stair(pFactory, eStairType::AREA01_BROKEN, eStairState::FALL)); // 22 �ı�
	m_pStairList.push_back(new Stair(pFactory, eStairType::AREA01_NORMAL, eStairState::STRAFE)); // 23 �̵�
	m_pStairList.push_back(new Stair(pFactory, eStairType::AREA01_BROKEN, eStairState::FALL)); // 24 �ı�
	m_pStairList.push_back(new Stair(pFactory, eStairType::AREA01_BROKEN, eStairState::FALL)); // 25 �ı�
	m_pStairList.push_back(new Stair(pFactory, eStairType::AREA01_BROKEN, eStairState::FALL)); // 26 �ı�
	m_pStairList.push_back(new Stair(pFactory, eStairType::CLOUD_BASE, eStairState::SAVE)); // 27 ���̺�����Ʈ1

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

	// ��
	//m_pBirdList = pResourceManager->m_pTutoBirdList;
	m_pBirdList.push_back(new Bird(pFactory, eBirdType::BIG));
	m_pBirdList.push_back(new Bird(pFactory, eBirdType::SMALL));
	m_pBirdList.push_back(new Bird(pFactory, eBirdType::BIG));

	m_pBirdList[0]->Initialize(-40.0f, 29.0f, 64.0f); // ���� ��ġ (15.0f, 29.0f, 64.0f)
	m_pBirdList[0]->GetBirdObject()->SetRotation('Y', -90.0f);

	m_pBirdList[1]->Initialize(-40.0f, 32.0f, 72.0f); // ���� ��ġ (15.0f, 32.0f, 64.0f)
	m_pBirdList[1]->GetBirdObject()->SetRotation('Y', -90.0f);

	m_pBirdList[2]->Initialize(-40.0f, 35.0f, 80.0f); // ���� ��ġ (15.0f, 35.0f, 64.0f)
	m_pBirdList[2]->GetBirdObject()->SetRotation('Y', -90.0f);

	// ĳ����
	//m_pPlayer = pResourceManager->m_pTutoPlayer;/// �̰�?
	///m_pPlayer = new Player(pFactory, pDevice);
	///m_pPlayer->Initialize(0.0f, 2.85f, -8.0f);

	m_pPlayer2 = new PlayerNew(pFactory, pDevice);
	m_pPlayer2->Initialize(0.0f, 2.85f, -8.0f);

	// ĳ���Ϳ� ���̺�����Ʈ ����
	AddSavePoint("BASE", m_pPlayer2->GetPosition());
	AddSavePoint("SAVEPOINT01", Vector3(0.0f, 35.8f, 96.0f));
	AddSavePoint("SAVEPOINT02", Vector3(0.0f, 69.0f, 200.0f));

	// ù ��° ���̺�����Ʈ�� ĳ���Ϳ� �־��ش�.
	m_pPlayer2->SetNowSavePoint(m_pVecSavePoints[(int)eResetPoint::BASE]);
	//////////////////////////////////////////////////////////////////////////


	/// ���� 2
	//////////////////////////////////////////////////////////////////////////
	m_pStairList.push_back(new Stair(pFactory, eStairType::AREA02_NORMAL, eStairState::NORMAL)); // 28
	m_pStairList.push_back(new Stair(pFactory, eStairType::AREA02_NORMAL, eStairState::NORMAL)); // 29
	m_pStairList.push_back(new Stair(pFactory, eStairType::AREA02_NORMAL, eStairState::NORMAL)); // 30
	m_pStairList.push_back(new Stair(pFactory, eStairType::AREA02_NORMAL, eStairState::NORMAL)); // 31
	m_pStairList.push_back(new Stair(pFactory, eStairType::AREA02_BROKEN, eStairState::FALL)); // 32 �ı�
	m_pStairList.push_back(new Stair(pFactory, eStairType::AREA02_NORMAL, eStairState::NORMAL)); // 33
	m_pStairList.push_back(new Stair(pFactory, eStairType::AREA02_BROKEN, eStairState::FALL)); // 34 �ı�
	m_pStairList.push_back(new Stair(pFactory, eStairType::AREA02_NORMAL, eStairState::NORMAL)); // 35
	m_pStairList.push_back(new Stair(pFactory, eStairType::AREA02_BROKEN, eStairState::FALL)); // 36 �ı�
	m_pStairList.push_back(new Stair(pFactory, eStairType::AREA02_NORMAL, eStairState::NORMAL)); // 37
	m_pStairList.push_back(new Stair(pFactory, eStairType::AREA02_BROKEN, eStairState::FALL)); // 38 �ı�
	m_pStairList.push_back(new Stair(pFactory, eStairType::AREA02_NORMAL, eStairState::NORMAL)); // 39
	m_pStairList.push_back(new Stair(pFactory, eStairType::AREA02_BROKEN, eStairState::FALL)); // 40 �ı�
	m_pStairList.push_back(new Stair(pFactory, eStairType::AREA02_NORMAL, eStairState::NORMAL)); // 41
	m_pStairList.push_back(new Stair(pFactory, eStairType::AREA02_BROKEN, eStairState::FALL)); // 42 �ı�
	m_pStairList.push_back(new Stair(pFactory, eStairType::CLOUD, eStairState::MOVE)); // 43 ����
	m_pStairList.push_back(new Stair(pFactory, eStairType::CLOUD, eStairState::MOVE)); // 44 ����
	m_pStairList.push_back(new Stair(pFactory, eStairType::CLOUD, eStairState::MOVE)); // 45 ����
	m_pStairList.push_back(new Stair(pFactory, eStairType::CLOUD, eStairState::MOVE)); // 46 ����
	m_pStairList.push_back(new Stair(pFactory, eStairType::CLOUD, eStairState::MOVE)); // 47 ����
	m_pStairList.push_back(new Stair(pFactory, eStairType::CLOUD, eStairState::MOVE)); // 48 ����
	m_pStairList.push_back(new Stair(pFactory, eStairType::CLOUD, eStairState::MOVE)); // 49 ����
	m_pStairList.push_back(new Stair(pFactory, eStairType::CLOUD, eStairState::MOVE)); // 50 ����
	m_pStairList.push_back(new Stair(pFactory, eStairType::CLOUD, eStairState::MOVE)); // 51 ����
	m_pStairList.push_back(new Stair(pFactory, eStairType::CLOUD, eStairState::MOVE)); // 52 ����
	m_pStairList.push_back(new Stair(pFactory, eStairType::CLOUD, eStairState::MOVE)); // 53 ����
	m_pStairList.push_back(new Stair(pFactory, eStairType::CLOUD, eStairState::MOVE)); // 54 ����
	m_pStairList.push_back(new Stair(pFactory, eStairType::CLOUD, eStairState::MOVE)); // 55 ����
	m_pStairList.push_back(new Stair(pFactory, eStairType::CLOUD, eStairState::MOVE)); // 56 ����
	m_pStairList.push_back(new Stair(pFactory, eStairType::AREA02_NORMAL, eStairState::NORMAL)); // 57
	m_pStairList.push_back(new Stair(pFactory, eStairType::AREA02_NORMAL, eStairState::NORMAL)); // 58
	m_pStairList.push_back(new Stair(pFactory, eStairType::AREA02_NORMAL, eStairState::NORMAL)); // 59
	m_pStairList.push_back(new Stair(pFactory, eStairType::CLOUD_BASE, eStairState::SAVE)); // 60 ���̺� ����Ʈ2

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
	m_pStairList[44]->Initialize(-40.0f, 51.0f, 152.0f);
	m_pStairList[45]->Initialize(-56.0f, 51.0f, 152.0f);
	m_pStairList[46]->Initialize(-64.0f, 51.0f, 152.0f);
	m_pStairList[47]->Initialize(40.0f, 54.0f, 160.0f);
	m_pStairList[48]->Initialize(48.0f, 54.0f, 160.0f);
	m_pStairList[49]->Initialize(56.0f, 54.0f, 160.0f);
	m_pStairList[50]->Initialize(-40.0f, 57.0f, 168.0f);
	m_pStairList[51]->Initialize(-48.0f, 57.0f, 168.0f);
	m_pStairList[52]->Initialize(-56.0f, 57.0f, 168.0f);
	m_pStairList[53]->Initialize(-64.0f, 57.0f, 168.0f);
	m_pStairList[54]->Initialize(48.0f, 60.0f, 176.0f);
	m_pStairList[55]->Initialize(56.0f, 60.0f, 176.0f);
	m_pStairList[56]->Initialize(64.0f, 60.0f, 176.0f);

	m_pStairList[57]->Initialize(-8.0f, 63.0f, 184.0f);
	m_pStairList[58]->Initialize(0.0f, 63.0f, 184.0f);
	m_pStairList[59]->Initialize(8.0f, 63.0f, 184.0f);
	m_pStairList[60]->Initialize(0.0f, 68.5f, 200.0f);
	//////////////////////////////////////////////////////////////////////////


	/// ���� 3
	//////////////////////////////////////////////////////////////////////////
	m_pStairList.push_back(new Stair(pFactory, eStairType::AREA03_BROKEN, eStairState::FALL)); // 61 �ı�
	m_pStairList.push_back(new Stair(pFactory, eStairType::AREA03_BROKEN, eStairState::FALL)); // 62 �ı�
	m_pStairList.push_back(new Stair(pFactory, eStairType::AREA03_BROKEN, eStairState::FALL)); // 63 �ı�
	m_pStairList.push_back(new Stair(pFactory, eStairType::AREA03_BROKEN, eStairState::FALL_STRAFE)); // 64 �ı� �̵�
	m_pStairList.push_back(new Stair(pFactory, eStairType::AREA03_BROKEN, eStairState::FALL)); // 65 �ı�
	m_pStairList.push_back(new Stair(pFactory, eStairType::AREA03_NORMAL, eStairState::DIAGONAL)); // 66 �̵�
	m_pStairList.push_back(new Stair(pFactory, eStairType::AREA03_NORMAL, eStairState::NORMAL)); // 67
	m_pStairList.push_back(new Stair(pFactory, eStairType::AREA03_NORMAL, eStairState::DIAGONAL)); // 68 �̵�
	m_pStairList.push_back(new Stair(pFactory, eStairType::AREA03_NORMAL, eStairState::STRAFE)); // 69 �̵�
	m_pStairList.push_back(new Stair(pFactory, eStairType::AREA03_BROKEN, eStairState::FALL)); // 70 �ı�
	m_pStairList.push_back(new Stair(pFactory, eStairType::AREA03_BROKEN, eStairState::FALL_STRAFE)); // 71 �ı� �̵�
	m_pStairList.push_back(new Stair(pFactory, eStairType::AREA03_NORMAL, eStairState::NORMAL)); // 72
	m_pStairList.push_back(new Stair(pFactory, eStairType::AREA03_NORMAL, eStairState::NORMAL)); // 73
	m_pStairList.push_back(new Stair(pFactory, eStairType::AREA03_NORMAL, eStairState::NORMAL)); // 74
	m_pStairList.push_back(new Stair(pFactory, eStairType::END_BASE, eStairState::BASE)); // 75 ���� ����

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
	m_pClearStage->SetPosition(0.0f, 105.0f, 327.5f);
	//////////////////////////////////////////////////////////////////////////

	m_pUI.push_back(pFactory->CreateBitmapObject(150, 920, L"../Data/Image/InGame_Bar.png"));
	m_pUI[0]->SetBitmapPosition(40, 70);

	m_pUI.push_back(pFactory->CreateBitmapObject(207, 178, L"../Data/Image/InGame_CharacterFace.png"));
	m_pUI[1]->SetBitmapPosition(10, 800);
}

void TutorialInGameScene::Update(SceneManager* pSceneManager, GameTimer* pTimer, D3Camera* pCamera)
{
	m_pSea->Update(pTimer, pCamera);


	/// <summary>
	/// Player(Old)
	/// </summary>
	for (UINT i = 0; i < m_pStairList.size(); i++)
	{
		for (UINT j = 0; j < m_pPlayer->m_pPlayerList.size(); j++)
		{
			m_pStairList[i]->Update(pTimer, pCamera, m_pPlayer);
		}

		if (m_pPlayer->GetMoveToLeft()->CheckCollided(m_pStairList[i]->GetStairObject()->GetBoxCollider()))
		{
			m_pPlayer->GetStairMoveToLeft(m_pStairList[i]);
		}

		if (m_pPlayer->GetMoveToRight()->CheckCollided(m_pStairList[i]->GetStairObject()->GetBoxCollider()))
		{
			m_pPlayer->GetStairMoveToRight(m_pStairList[i]);
		}

		if (m_pPlayer->GetMoveToUp()->CheckCollided(m_pStairList[i]->GetStairObject()->GetBoxCollider()))
		{
			m_pPlayer->GetStairMoveToUp(m_pStairList[i]);
		}

		if (m_pPlayer->GetMoveToDown()->CheckCollided(m_pStairList[i]->GetStairObject()->GetBoxCollider()))
		{
			m_pPlayer->GetStairMoveToDown(m_pStairList[i]);
		}
	}

	// ĳ���� ��ġ ����
	m_pPlayer->KeyInput(pTimer->DeltaTime());

	m_pPlayer->Update(pTimer, pCamera);


	/// <summary>
	/// Player(New)
	/// </summary>

	// ��� ��ܿ� ���� �÷��̾� Ŭ������ ��ȣ�ۿ�
	for (UINT i = 0; i < m_pStairList.size(); i++)
	{
		for (UINT j = 0; j < m_pPlayer2->m_pPlayerList.size(); j++)
		{
			m_pStairList[i]->Update2(pTimer, pCamera, m_pPlayer2);
		}
	}

	// ĳ���� ��ġ ����
	m_pPlayer2->KeyInput(pTimer->DeltaTime());
	this->CheckPlayerPos();
	m_pPlayer2->ProcessMove(pTimer->DeltaTime());
	m_pPlayer2->Update(pTimer, pCamera);

	// ���� ��� ��ġ ���½�Ű�� �浹üũ �ڽ�
	m_pResetForwardClouds->Update(pCamera);
	m_pResetBackwardClouds->Update(pCamera);

	// �� ������Ʈ ��ġ ���½�Ű�� �浹üũ �ڽ�
	m_pResetBirds->Update(pCamera);

	for (UINT i = 0; i < m_pBirdList.size(); i++)
	{
		m_pBirdList[i]->Update(pTimer, pCamera);
	}

	for (UINT i = 0; i < m_pUI.size(); i++)
	{
		m_pUI[i]->Update(pTimer, pCamera);
	}

	/// ���� 1
	//////////////////////////////////////////////////////////////////////////
	m_pStairList[19]->GetStairObject()->Strafe(-8.0f, 8.0f, 8.0f * pTimer->DeltaTime());
	m_pStairList[23]->GetStairObject()->StrafeInverse(8.0f, -8.0f, 8.0f * pTimer->DeltaTime());

	m_pResetBirds->Update(pCamera);

	m_pBirdList[0]->GetBirdObject()->SetTranslation(10.0f * pTimer->DeltaTime(), 0.0f, 0.0f); // ���� ��ġ -10.0f
	m_pBirdList[1]->GetBirdObject()->SetTranslation(40.0f * pTimer->DeltaTime(), 0.0f, 0.0f); // ���� ��ġ -20.0f
	m_pBirdList[2]->GetBirdObject()->SetTranslation(15.0f * pTimer->DeltaTime(), 0.0f, 0.0f); // ���� ��ġ -10.0f

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

	/// ///////////////////////////////////////////////////////////////////////
	/// <summary>
	/// �� ���� ���� �÷��̾��� ��ȣ�ۿ�
	/// </summary>
	
	/// <summary>
	/// Player(Old)
	/// </summary>
	for (UINT i = 0; i < m_pPlayer->m_pPlayerList.size(); i++)
	{
		if (m_pPlayer->m_pPlayerList[i]->GetBoxCollider()->CheckCollided(m_pBirdList[0]->GetBirdObject()->GetBoxCollider()))
		{
			m_pPlayer->SetState(ePlayerState::DAMAGE);
			m_pPlayer->Reset();
		}

		if (m_pPlayer->m_pPlayerList[i]->GetBoxCollider()->CheckCollided(m_pBirdList[1]->GetBirdObject()->GetBoxCollider()))
		{
			m_pPlayer->SetState(ePlayerState::DAMAGE);
			m_pPlayer->Reset();
		}

		if (m_pPlayer->m_pPlayerList[i]->GetBoxCollider()->CheckCollided(m_pBirdList[2]->GetBirdObject()->GetBoxCollider()))
		{
			m_pPlayer->SetState(ePlayerState::DAMAGE);
			m_pPlayer->Reset();
		}
	}

	/// <summary>
	/// Player(New)
	/// </summary>
	for (UINT i = 0; i < m_pPlayer2->m_pPlayerList.size(); i++)
	{
		if (m_pPlayer2->m_pPlayerList[i]->GetBoxCollider()->CheckCollided(m_pBirdList[0]->GetBirdObject()->GetBoxCollider()))
		{
			m_pPlayer2->SetGameState(ePlayerGameState::OnHit);
			m_pPlayer2->ResetBySavePoint();
		}

		if (m_pPlayer2->m_pPlayerList[i]->GetBoxCollider()->CheckCollided(m_pBirdList[1]->GetBirdObject()->GetBoxCollider()))
		{
			m_pPlayer2->SetGameState(ePlayerGameState::OnHit);
			m_pPlayer2->ResetBySavePoint();
		}

		if (m_pPlayer2->m_pPlayerList[i]->GetBoxCollider()->CheckCollided(m_pBirdList[2]->GetBirdObject()->GetBoxCollider()))
		{
			m_pPlayer2->SetGameState(ePlayerGameState::OnHit);
			m_pPlayer2->ResetBySavePoint();
		}
	}
	/// ///////////////////////////////////////////////////////////////////////



	/// ///////////////////////////////////////////////////////////////////////
	/// <summary>
	/// Ư�� ������ �����ϸ� ���̺�����Ʈ ����(����) ���..
	/// </summary>
	for (UINT i = 0; i < m_pPlayer->m_pPlayerList.size(); i++)
	{
		// Save Point 01 ������ ���������Ƿ� ĳ������ Reset Position�� �����Ѵ�
		if (m_pStairList[27]->GetStairObject()->GetBoxCollider()->CheckCollided(m_pPlayer->m_pPlayerList[i]->GetBoxCollider()))
		{
			m_pPlayer->SetResetSavePoint01Position(0.0f, 35.8f, 96.0f);
			m_pPlayer->SetResetPoint(eResetPoint::SAVEPOINT01);
		}
	}

	/// 
	for (UINT i = 0; i < m_pPlayer2->m_pPlayerList.size(); i++)
	{
		// Save Point 01 ������ ���������Ƿ� ĳ������ Reset Position�� �����Ѵ�
		if (m_pStairList[27]->GetStairObject()->GetBoxCollider()->CheckCollided(m_pPlayer2->m_pPlayerList[i]->GetBoxCollider()))
		{
			m_pPlayer2->SetNowSavePoint(m_pVecSavePoints[(int)eResetPoint::SAVEPOINT01]);
		}
	}




	/// ���� 2
	//////////////////////////////////////////////////////////////////////////
	m_pStairList[43]->GetStairObject()->SetTranslation(5.0f * pTimer->DeltaTime(), 0.0f, 0.0f);
	m_pStairList[44]->GetStairObject()->SetTranslation(5.0f * pTimer->DeltaTime(), 0.0f, 0.0f);
	m_pStairList[45]->GetStairObject()->SetTranslation(5.0f * pTimer->DeltaTime(), 0.0f, 0.0f);
	m_pStairList[46]->GetStairObject()->SetTranslation(5.0f * pTimer->DeltaTime(), 0.0f, 0.0f);
	m_pStairList[47]->GetStairObject()->SetTranslation(-5.0f * pTimer->DeltaTime(), 0.0f, 0.0f);
	m_pStairList[48]->GetStairObject()->SetTranslation(-5.0f * pTimer->DeltaTime(), 0.0f, 0.0f);
	m_pStairList[49]->GetStairObject()->SetTranslation(-5.0f * pTimer->DeltaTime(), 0.0f, 0.0f);
	m_pStairList[50]->GetStairObject()->SetTranslation(5.0f * pTimer->DeltaTime(), 0.0f, 0.0f);
	m_pStairList[51]->GetStairObject()->SetTranslation(5.0f * pTimer->DeltaTime(), 0.0f, 0.0f);
	m_pStairList[52]->GetStairObject()->SetTranslation(5.0f * pTimer->DeltaTime(), 0.0f, 0.0f);
	m_pStairList[53]->GetStairObject()->SetTranslation(5.0f * pTimer->DeltaTime(), 0.0f, 0.0f);
	m_pStairList[54]->GetStairObject()->SetTranslation(-5.0f * pTimer->DeltaTime(), 0.0f, 0.0f);
	m_pStairList[55]->GetStairObject()->SetTranslation(-5.0f * pTimer->DeltaTime(), 0.0f, 0.0f);
	m_pStairList[56]->GetStairObject()->SetTranslation(-5.0f * pTimer->DeltaTime(), 0.0f, 0.0f);

	if (m_pStairList[43]->GetStairObject()->GetBoxCollider()->CheckCollided(m_pResetForwardClouds))
	{
		m_pStairList[43]->Reset();
	}

	if (m_pStairList[44]->GetStairObject()->GetBoxCollider()->CheckCollided(m_pResetForwardClouds))
	{
		m_pStairList[44]->Reset();
	}

	if (m_pStairList[45]->GetStairObject()->GetBoxCollider()->CheckCollided(m_pResetForwardClouds))
	{
		m_pStairList[45]->Reset();
	}

	if (m_pStairList[46]->GetStairObject()->GetBoxCollider()->CheckCollided(m_pResetForwardClouds))
	{
		m_pStairList[46]->Reset();
	}

	if (m_pStairList[47]->GetStairObject()->GetBoxCollider()->CheckCollided(m_pResetBackwardClouds))
	{
		m_pStairList[47]->Reset();
	}

	if (m_pStairList[48]->GetStairObject()->GetBoxCollider()->CheckCollided(m_pResetBackwardClouds))
	{
		m_pStairList[48]->Reset();
	}

	if (m_pStairList[49]->GetStairObject()->GetBoxCollider()->CheckCollided(m_pResetBackwardClouds))
	{
		m_pStairList[49]->Reset();
	}

	if (m_pStairList[50]->GetStairObject()->GetBoxCollider()->CheckCollided(m_pResetForwardClouds))
	{
		m_pStairList[50]->Reset();
	}

	if (m_pStairList[51]->GetStairObject()->GetBoxCollider()->CheckCollided(m_pResetForwardClouds))
	{
		m_pStairList[51]->Reset();
	}

	if (m_pStairList[52]->GetStairObject()->GetBoxCollider()->CheckCollided(m_pResetForwardClouds))
	{
		m_pStairList[52]->Reset();
	}

	if (m_pStairList[53]->GetStairObject()->GetBoxCollider()->CheckCollided(m_pResetForwardClouds))
	{
		m_pStairList[53]->Reset();
	}

	if (m_pStairList[54]->GetStairObject()->GetBoxCollider()->CheckCollided(m_pResetBackwardClouds))
	{
		m_pStairList[54]->Reset();
	}

	if (m_pStairList[55]->GetStairObject()->GetBoxCollider()->CheckCollided(m_pResetBackwardClouds))
	{
		m_pStairList[55]->Reset();
	}

	if (m_pStairList[56]->GetStairObject()->GetBoxCollider()->CheckCollided(m_pResetBackwardClouds))
	{
		m_pStairList[56]->Reset();
	}


	/// <summary>
	/// �̵��ϴ� ������ Ÿ�� ���ٰ�, ��� ���� �ٱ����� �̵��ϸ� ���µǴ� 
	/// </summary>
	for (UINT i = 0; i < m_pPlayer->m_pPlayerList.size(); i++)
	{
		if (m_pPlayer->m_pPlayerList[i]->GetBoxCollider()->CheckCollided(m_pResetForwardClouds)
			|| m_pPlayer->m_pPlayerList[i]->GetBoxCollider()->CheckCollided(m_pResetBackwardClouds))
		{
			m_pPlayer->Reset();
		}
	}

	for (UINT i = 0; i < m_pPlayer2->m_pPlayerList.size(); i++)
	{
		if (m_pPlayer2->m_pPlayerList[i]->GetBoxCollider()->CheckCollided(m_pResetForwardClouds)
			|| m_pPlayer2->m_pPlayerList[i]->GetBoxCollider()->CheckCollided(m_pResetBackwardClouds))
		{
			m_pPlayer2->ResetBySavePoint();
		}
	}
	//////////////////////////////////////////////////////////////////////////

	// old
	for (UINT i = 0; i < m_pPlayer->m_pPlayerList.size(); i++)
	{
		// Save Point 02 ������ ���������Ƿ� ĳ������ Reset Position�� �����Ѵ�
		if (m_pStairList[50]->GetStairObject()->GetBoxCollider()->CheckCollided(m_pPlayer->m_pPlayerList[i]->GetBoxCollider()))
		{
			m_pPlayer->SetResetSavePoint02Position(0.0f, 69.0f, 200.0f);
			m_pPlayer->SetResetPoint(eResetPoint::SAVEPOINT02);
		}
	}

	// new
	for (UINT i = 0; i < m_pPlayer2->m_pPlayerList.size(); i++)
	{
		// Save Point 02 ������ ���������Ƿ� ĳ������ Reset Position�� �����Ѵ�
		if (m_pStairList[50]->GetStairObject()->GetBoxCollider()->CheckCollided(m_pPlayer2->m_pPlayerList[i]->GetBoxCollider()))
		{
			m_pPlayer2->SetNowSavePoint(m_pVecSavePoints[(int)eResetPoint::SAVEPOINT02]);
		}
	}

	/// ���� 3
	//////////////////////////////////////////////////////////////////////////
	m_pStairList[64]->GetStairObject()->Strafe(-8.0f, 8.0f, 8.0f * pTimer->DeltaTime());

	m_pStairList[66]->GetStairObject()->StrafeDiagonal(-8.0f, -8.0f, 75.0f, 81.0f, 232.0f, 248.0f, 1.2f * pTimer->DeltaTime());
	m_pStairList[68]->GetStairObject()->StrafeDiagonal(8.0f, 8.0f, 84.0f, 78.0f, 256.0f, 240.0f, 1.2f * pTimer->DeltaTime());

	m_pStairList[69]->GetStairObject()->StrafeInverse(8.0f, -8.0f, 16.0f * pTimer->DeltaTime());

	m_pStairList[71]->GetStairObject()->Strafe(-8.0f, 8.0f, 8.0f * pTimer->DeltaTime());
	//////////////////////////////////////////////////////////////////////////

	/// ĳ���Ͱ� � Stair�ϰ� �浹�ϰ����� �ʴٸ� ���������Ѵ�(����)
	for (UINT i = 0; i < m_pPlayer->m_pPlayerList.size(); i++)
	{
		if (m_pPlayer->m_pPlayerList[i]->GetBoxCollider()->CheckCollided(m_pStairList[0]->GetStairObject()->GetBoxCollider()) == false)
		{
			m_pPlayer->m_pPlayerList[i]->SetTranslation(0.0f, -5.0f * pTimer->DeltaTime(), 0.0f);
			m_pPlayer->SetState(ePlayerState::FALL);
		}
	}

	CameraMove(pCamera);

	m_pClearStage->Update(pCamera);

	/// ���� Ŭ���� ����

	// old
	for (UINT i = 0; i < m_pPlayer->m_pPlayerList.size(); i++)
	{
		if (m_pPlayer->m_pPlayerList[i]->GetBoxCollider()->CheckCollided(m_pClearStage))
		{
			m_GameClear = true;
			m_pPlayer->SetState(ePlayerState::CLEAR);
			m_pPlayer->m_pPlayerList[i]->SetRotation('Y', 10.0f * pTimer->DeltaTime());
		}
	}

	// new
	for (UINT i = 0; i < m_pPlayer2->m_pPlayerList.size(); i++)
	{
		if (m_pPlayer2->m_pPlayerList[i]->GetBoxCollider()->CheckCollided(m_pClearStage))
		{
			m_GameClear = true;
			m_pPlayer2->SetGameState(ePlayerGameState::Clear);
			m_pPlayer2->m_pPlayerList[i]->SetRotation('Y', 10.0f * pTimer->DeltaTime());
		}
	}

	ChangeSceneState(pSceneManager);
}

void TutorialInGameScene::Render(DXRenderer* pRenderer)
{
	m_pSea->Render();

	m_pResetForwardClouds->Render();
	m_pResetBackwardClouds->Render();

	for (UINT i = 0; i < m_pStairList.size(); i++)
	{
		m_pStairList[i]->Render();
	}

	m_pResetBirds->Render();

	for (UINT i = 0; i < m_pBirdList.size(); i++)
	{
		m_pBirdList[i]->Render();
	}

	///m_pPlayer->Render();
	m_pPlayer2->Render();

	m_pClearStage->Render();

	for (UINT i = 0; i < m_pUI.size(); i++)
	{
		m_pUI[i]->Render();
	}
}

void TutorialInGameScene::Finalize()
{
	for (UINT i = 0; i < m_pStairList.size(); i++)
	{
		m_pStairList[i]->Finalize();
	}

	m_pStairList.clear();

	for (UINT i = 0; i < m_pBirdList.size(); i++)
	{
		m_pBirdList[i]->Finalize();
	}

	m_pBirdList.clear();

	m_pPlayer->Finalize();
	delete m_pPlayer;
	m_pPlayer = nullptr;

	m_pPlayer2->Finalize();
	delete m_pPlayer2;
	m_pPlayer2 = nullptr;

	for (UINT i = 0; i < m_pUI.size(); i++)
	{
		m_pUI[i]->Finalize();
	}

	m_pUI.clear();

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
	// �� ��ȯ
	if (GetAsyncKeyState(VK_NEXT) & 0x0001)
	{
		pSceneManager->ChangeScene(eSceneState::MAININGAME);
	}

	// ������ Ŭ�����ϸ� ���� ������ �̵�
	if (m_GameClear)
	{
		//pSceneManager->ChangeScene(eSceneState::AREA_CHOICE);
	}
}

void TutorialInGameScene::CameraMove(D3Camera* pCamera)
{
	// old
	for (UINT i = 0; i < m_pPlayer->m_pPlayerList.size(); i++)
	{
		if (pCamera->GetIndex() == 1)
		{
			XMFLOAT3 cameraPos = XMFLOAT3(8.0f, 4.0f, -20.0f);
			cameraPos.x += m_pPlayer->m_pPlayerList[i]->GetPosition().x;
			cameraPos.y += m_pPlayer->m_pPlayerList[i]->GetPosition().y;
			cameraPos.z += m_pPlayer->m_pPlayerList[i]->GetPosition().z;

			XMFLOAT3 target = XMFLOAT3(0.0f, 2.5f, 0.0f);
			target.x += m_pPlayer->m_pPlayerList[i]->GetPosition().x;
			target.y += m_pPlayer->m_pPlayerList[i]->GetPosition().y;
			target.z += m_pPlayer->m_pPlayerList[i]->GetPosition().z;

			pCamera->LookAt(cameraPos, target, XMFLOAT3(0, 1.0f, 0));
		}
	}

	// new
	for (UINT i = 0; i < m_pPlayer2->m_pPlayerList.size(); i++)
	{
		if (pCamera->GetIndex() == 1)
		{
			XMFLOAT3 cameraPos = XMFLOAT3(8.0f, 4.0f, -20.0f);
			cameraPos.x += m_pPlayer2->m_pPlayerList[i]->GetPosition().x;
			cameraPos.y += m_pPlayer2->m_pPlayerList[i]->GetPosition().y;
			cameraPos.z += m_pPlayer2->m_pPlayerList[i]->GetPosition().z;

			XMFLOAT3 target = XMFLOAT3(0.0f, 2.5f, 0.0f);
			target.x += m_pPlayer2->m_pPlayerList[i]->GetPosition().x;
			target.y += m_pPlayer2->m_pPlayerList[i]->GetPosition().y;
			target.z += m_pPlayer2->m_pPlayerList[i]->GetPosition().z;

			pCamera->LookAt(cameraPos, target, XMFLOAT3(0, 1.0f, 0));
		}
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
/// ��ü ��ܸ� ���鼭
/// x/z ��ġ�� ������ �浹����
/// 
/// ��������� ĳ���Ͱ� '���' ���� �� ��ġ�� �˷��ش�.
/// 
/// �浹���� ��ġ�� ������ ���ٰ� ����
/// </summary>
/// <param name="position">��������� �޾� �� ��ġ (y���� �߿�)</param>
/// <param name="x">ĳ������ x��ǥ��</param>
/// <param name="z">ĳ������ z��ǥ��</param>
/// <returns></returns>
bool TutorialInGameScene::GetCollisionPos(Vector3* position)
{
	// ����� �������� �� ������
	//position->x = x;
	//position->z = z;

	DirectX::BoundingBox _boundingBox;
	for (UINT i = 0; i < m_pStairList.size(); i++)
	{
		_boundingBox = m_pStairList[i]->GetStairObject()->GetBoxCollider()->GetBBForCollisionDetection();

		Vector4 _origin(position->x, 1000.0f, position->z, 1.0f);
		Vector4 _direction(0, -1.0f, 0, 1.0f);
		float _resultDist = 1000.0f;

		// ������ �浹�� �ߴٸ�, �� ��ġ�� �����Ѵ�.
		if (_boundingBox.Intersects(_origin, _direction, _resultDist) == true)
		{
			//position->y = _origin.y + _resultDist;
			position->y = m_pStairList[i]->GetStairObject()->GetPosition().y + 3.0f;
			return true;
		}
	}

	// �浹�� ���ߴٴ� ����, ����� �� �ִٴ� ��
	return false;
}

void TutorialInGameScene::CheckPlayerPos()
{
	/// y���� ������. ������ �ʿ伺�� ���� �� �Ź� �ҷ���
	if (m_pPlayer2->IsYPosNeedRefresh() == true)
	{
		Vector3 _newPos = m_pPlayer2->GetPosition();

		if (GetCollisionPos(&_newPos) == true)
		{
			// �ٴڰ��� �浹������ ������ ����, �� ��ġ�� ������ָ� �ǰ� (MK1)
			m_pPlayer2->SetPosition(_newPos);
		}
		else
		{
			// �ƴ� ����, ����� ������ �׾�� ��
			m_pPlayer2->SetGameState(ePlayerGameState::Dying);
		}

		m_pPlayer2->YPosCompleted();
	}
}
