#include "ResourceManager.h"
#include "DXDevice.h"
#include "DXFactory.h"

// Object
#include "Stair.h"
#include "Bird.h"
#include "PlayerNew.h"
#include "IDXObjectBase.h"

ResourceManager::ResourceManager(DXDevice* pDevice, DXFactory* pFactory)
	: m_pDXDevice(pDevice),
	m_pDXFactory(pFactory),
	m_pTutoPlayer(nullptr),
	m_pMainPlayer(nullptr)
{

}

ResourceManager::~ResourceManager()
{
	
}

void ResourceManager::CreateTutoInGameAssets()
{
	m_pTutoBackgroundList.push_back(m_pDXFactory->CreateASEObject((LPSTR)"../Data/ASE/sea_dumi.ASE", L"../Data/Textures/sea.dds"));
	m_pTutoBackgroundList.push_back(m_pDXFactory->CreateASEObject((LPSTR)"../Data/ASE/cloud_big.ASE", L"../Data/Textures/cloud_big.dds"));
	m_pTutoBackgroundList.push_back(m_pDXFactory->CreateASEObject((LPSTR)"../Data/ASE/cloud_big.ASE", L"../Data/Textures/cloud_big.dds"));
	m_pTutoBackgroundList.push_back(m_pDXFactory->CreateASEObject((LPSTR)"../Data/ASE/cloud_big.ASE", L"../Data/Textures/cloud_big.dds"));
	m_pTutoBackgroundList.push_back(m_pDXFactory->CreateASEObject((LPSTR)"../Data/ASE/namu.ASE", L"../Data/Textures/namu.dds"));
	m_pTutoBackgroundList.push_back(m_pDXFactory->CreateASEObject((LPSTR)"../Data/ASE/cloud_big.ASE", L"../Data/Textures/cloud_big.dds"));
	m_pTutoBackgroundList.push_back(m_pDXFactory->CreateASEObject((LPSTR)"../Data/ASE/cloud_big.ASE", L"../Data/Textures/cloud_big.dds"));
	m_pTutoBackgroundList.push_back(m_pDXFactory->CreateASEObject((LPSTR)"../Data/ASE/cloud_big.ASE", L"../Data/Textures/cloud_big.dds"));
	m_pTutoBackgroundList.push_back(m_pDXFactory->CreateASEObject((LPSTR)"../Data/ASE/lighthouse.ASE", L"../Data/Textures/lighthouse.dds"));
	m_pTutoBackgroundList.push_back(m_pDXFactory->CreateASEObject((LPSTR)"../Data/ASE/cloud_small.ASE", L"../Data/Textures/cloud_small.dds"));
	m_pTutoBackgroundList.push_back(m_pDXFactory->CreateASEObject((LPSTR)"../Data/ASE/cloud_small.ASE", L"../Data/Textures/cloud_small.dds"));
	m_pTutoBackgroundList.push_back(m_pDXFactory->CreateASEObject((LPSTR)"../Data/ASE/cloud_big.ASE", L"../Data/Textures/cloud_big.dds"));
	m_pTutoBackgroundList.push_back(m_pDXFactory->CreateASEObject((LPSTR)"../Data/ASE/cloud_small.ASE", L"../Data/Textures/cloud_small.dds"));
	m_pTutoBackgroundList.push_back(m_pDXFactory->CreateASEObject((LPSTR)"../Data/ASE/sin_1.ASE", L"../Data/Textures/sin_1.dds"));
	m_pTutoBackgroundList.push_back(m_pDXFactory->CreateASEObject((LPSTR)"../Data/ASE/sin_2.ASE", L"../Data/Textures/sin_2.dds"));
	m_pTutoBackgroundList.push_back(m_pDXFactory->CreateASEObject((LPSTR)"../Data/ASE/cloud_small.ASE", L"../Data/Textures/cloud_small.dds"));

	m_pTutoPlayer = new PlayerNew(m_pDXDevice, m_pDXFactory);

	m_pTutoStairList.push_back(new Stair(m_pDXFactory, eStairType::Start_Base, eStairState::Base));
	m_pTutoStairList.push_back(new Stair(m_pDXFactory, eStairType::Area01_Normal, eStairState::Normal)); // 1
	m_pTutoStairList.push_back(new Stair(m_pDXFactory, eStairType::Area01_Normal, eStairState::Normal)); // 2
	m_pTutoStairList.push_back(new Stair(m_pDXFactory, eStairType::Area01_Normal, eStairState::Normal)); // 3
	m_pTutoStairList.push_back(new Stair(m_pDXFactory, eStairType::Area01_Normal, eStairState::Normal)); // 4
	m_pTutoStairList.push_back(new Stair(m_pDXFactory, eStairType::Area01_Normal, eStairState::Normal)); // 5
	m_pTutoStairList.push_back(new Stair(m_pDXFactory, eStairType::Area01_Normal, eStairState::Normal)); // 6
	m_pTutoStairList.push_back(new Stair(m_pDXFactory, eStairType::Area01_Normal, eStairState::Normal)); // 7
	m_pTutoStairList.push_back(new Stair(m_pDXFactory, eStairType::Area01_Broken, eStairState::Fall)); // 8 �ı�
	m_pTutoStairList.push_back(new Stair(m_pDXFactory, eStairType::Area01_Normal, eStairState::Normal)); // 9
	m_pTutoStairList.push_back(new Stair(m_pDXFactory, eStairType::Area01_Normal, eStairState::Normal)); // 10
	m_pTutoStairList.push_back(new Stair(m_pDXFactory, eStairType::Area01_Normal, eStairState::Normal)); // 11
	m_pTutoStairList.push_back(new Stair(m_pDXFactory, eStairType::Area01_Normal, eStairState::Normal)); // 12
	m_pTutoStairList.push_back(new Stair(m_pDXFactory, eStairType::Area01_Normal, eStairState::Normal)); // 13
	m_pTutoStairList.push_back(new Stair(m_pDXFactory, eStairType::Area01_Broken, eStairState::Fall)); // 14 �ı�
	m_pTutoStairList.push_back(new Stair(m_pDXFactory, eStairType::Area01_Broken, eStairState::Fall)); // 15 �ı�
	m_pTutoStairList.push_back(new Stair(m_pDXFactory, eStairType::Area01_Normal, eStairState::Normal)); // 16
	m_pTutoStairList.push_back(new Stair(m_pDXFactory, eStairType::Area01_Normal, eStairState::Normal)); // 17
	m_pTutoStairList.push_back(new Stair(m_pDXFactory, eStairType::Area01_Normal, eStairState::Normal)); // 18
	m_pTutoStairList.push_back(new Stair(m_pDXFactory, eStairType::Area01_Normal, eStairState::Strafe)); // 19 �̵�
	m_pTutoStairList.push_back(new Stair(m_pDXFactory, eStairType::Area01_Broken, eStairState::Fall)); // 20 �ı�
	m_pTutoStairList.push_back(new Stair(m_pDXFactory, eStairType::Area01_Normal, eStairState::Normal)); // 21
	m_pTutoStairList.push_back(new Stair(m_pDXFactory, eStairType::Area01_Broken, eStairState::Fall)); // 22 �ı�
	m_pTutoStairList.push_back(new Stair(m_pDXFactory, eStairType::Area01_Normal, eStairState::Strafe)); // 23 �̵�
	m_pTutoStairList.push_back(new Stair(m_pDXFactory, eStairType::Area01_Broken, eStairState::Fall)); // 24 �ı�
	m_pTutoStairList.push_back(new Stair(m_pDXFactory, eStairType::Area01_Broken, eStairState::Fall)); // 25 �ı�
	m_pTutoStairList.push_back(new Stair(m_pDXFactory, eStairType::Area01_Broken, eStairState::Fall)); // 26 �ı�
	m_pTutoStairList.push_back(new Stair(m_pDXFactory, eStairType::Cloud_Base, eStairState::Base)); // 27 ���̺�����Ʈ1
	m_pTutoStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Normal, eStairState::Normal)); // 28
	m_pTutoStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Normal, eStairState::Normal)); // 29
	m_pTutoStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Normal, eStairState::Normal)); // 30
	m_pTutoStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Normal, eStairState::Normal)); // 31
	m_pTutoStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Broken, eStairState::Fall)); // 32 �ı�
	m_pTutoStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Normal, eStairState::Normal)); // 33
	m_pTutoStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Broken, eStairState::Fall)); // 34 �ı�
	m_pTutoStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Normal, eStairState::Normal)); // 35
	m_pTutoStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Broken, eStairState::Fall)); // 36 �ı�
	m_pTutoStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Normal, eStairState::Normal)); // 37
	m_pTutoStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Broken, eStairState::Fall)); // 38 �ı�
	m_pTutoStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Normal, eStairState::Normal)); // 39
	m_pTutoStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Broken, eStairState::Fall)); // 40 �ı�
	m_pTutoStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Normal, eStairState::Normal)); // 41
	m_pTutoStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Broken, eStairState::Fall)); // 42 �ı�
	m_pTutoStairList.push_back(new Stair(m_pDXFactory, eStairType::Cloud, eStairState::Move)); // 43 ����
	m_pTutoStairList.push_back(new Stair(m_pDXFactory, eStairType::Cloud, eStairState::Move)); // 44 ����
	m_pTutoStairList.push_back(new Stair(m_pDXFactory, eStairType::Cloud, eStairState::Move)); // 45 ����
	m_pTutoStairList.push_back(new Stair(m_pDXFactory, eStairType::Cloud, eStairState::Move)); // 46 ����
	m_pTutoStairList.push_back(new Stair(m_pDXFactory, eStairType::Cloud, eStairState::Move)); // 47 ����
	m_pTutoStairList.push_back(new Stair(m_pDXFactory, eStairType::Cloud, eStairState::Move)); // 48 ����
	m_pTutoStairList.push_back(new Stair(m_pDXFactory, eStairType::Cloud, eStairState::Move)); // 49 ����
	m_pTutoStairList.push_back(new Stair(m_pDXFactory, eStairType::Cloud, eStairState::Move)); // 50 ����
	m_pTutoStairList.push_back(new Stair(m_pDXFactory, eStairType::Cloud, eStairState::Move)); // 51 ����
	m_pTutoStairList.push_back(new Stair(m_pDXFactory, eStairType::Cloud, eStairState::Move)); // 52 ����
	m_pTutoStairList.push_back(new Stair(m_pDXFactory, eStairType::Cloud, eStairState::Move)); // 53 ����
	m_pTutoStairList.push_back(new Stair(m_pDXFactory, eStairType::Cloud, eStairState::Move)); // 54 ����
	m_pTutoStairList.push_back(new Stair(m_pDXFactory, eStairType::Cloud, eStairState::Move)); // 55 ����
	m_pTutoStairList.push_back(new Stair(m_pDXFactory, eStairType::Cloud, eStairState::Move)); // 56 ����
	m_pTutoStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Normal, eStairState::Normal)); // 57
	m_pTutoStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Normal, eStairState::Normal)); // 58
	m_pTutoStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Normal, eStairState::Normal)); // 59
	m_pTutoStairList.push_back(new Stair(m_pDXFactory, eStairType::Cloud_Base, eStairState::Base)); // 60 ���̺� ����Ʈ2
	m_pTutoStairList.push_back(new Stair(m_pDXFactory, eStairType::Area03_Normal, eStairState::Normal)); // 61
	m_pTutoStairList.push_back(new Stair(m_pDXFactory, eStairType::Area03_Normal, eStairState::Normal)); // 62
	m_pTutoStairList.push_back(new Stair(m_pDXFactory, eStairType::Area03_Normal, eStairState::Normal)); // 63
	m_pTutoStairList.push_back(new Stair(m_pDXFactory, eStairType::Area03_Broken, eStairState::Fall_Strafe)); // 64 �ı� �̵�
	m_pTutoStairList.push_back(new Stair(m_pDXFactory, eStairType::Area03_Normal, eStairState::Normal)); // 65
	m_pTutoStairList.push_back(new Stair(m_pDXFactory, eStairType::Area03_Normal, eStairState::Diagonal)); // 66 �̵�
	m_pTutoStairList.push_back(new Stair(m_pDXFactory, eStairType::Area03_Normal, eStairState::Normal)); // 67
	m_pTutoStairList.push_back(new Stair(m_pDXFactory, eStairType::Area03_Normal, eStairState::Diagonal)); // 68 �̵�
	m_pTutoStairList.push_back(new Stair(m_pDXFactory, eStairType::Area03_Normal, eStairState::Strafe)); // 69 �̵�
	m_pTutoStairList.push_back(new Stair(m_pDXFactory, eStairType::Area03_Normal, eStairState::Normal)); // 70
	m_pTutoStairList.push_back(new Stair(m_pDXFactory, eStairType::Area03_Broken, eStairState::Fall_Strafe)); // 71 �ı� �̵�
	m_pTutoStairList.push_back(new Stair(m_pDXFactory, eStairType::Area03_Normal, eStairState::Normal)); // 72
	m_pTutoStairList.push_back(new Stair(m_pDXFactory, eStairType::Area03_Normal, eStairState::Normal)); // 73
	m_pTutoStairList.push_back(new Stair(m_pDXFactory, eStairType::Area03_Normal, eStairState::Normal)); // 74
	m_pTutoStairList.push_back(new Stair(m_pDXFactory, eStairType::End_Base, eStairState::Base)); // 75 ���� ����

	m_pTutoBirdList.push_back(new Bird(m_pDXFactory, eBirdType::BIG));
	m_pTutoBirdList.push_back(new Bird(m_pDXFactory, eBirdType::SMALL));
	m_pTutoBirdList.push_back(new Bird(m_pDXFactory, eBirdType::BIG));

	m_pTutoUI.push_back(m_pDXFactory->CreateBitmapObject(150, 920, L"../Data/Image/InGame_Bar.png"));					// 0
	m_pTutoUI.push_back(m_pDXFactory->CreateBitmapObject(120, 101, L"../Data/Image/InGame_CharacterFace.png"));			// 1
	m_pTutoUI.push_back(m_pDXFactory->CreateBitmapObject(290, 220, L"../Data/Image/InGame_TimeLimit_Background.png"));	// 2
	m_pTutoUI.push_back(m_pDXFactory->CreateBitmapObject(290, 220, L"../Data/Image/InGame_TimeLimit_Icon.png"));		// 3
	m_pTutoUI.push_back(m_pDXFactory->CreateBitmapObject(103, 127, L"../Data/Image/TutorialGame_TutoCursor_2.png"));	// 4
	m_pTutoUI.push_back(m_pDXFactory->CreateBitmapObject(103, 127, L"../Data/Image/TutorialGame_TutoCursor_3.png"));	// 5
	m_pTutoUI.push_back(m_pDXFactory->CreateBitmapObject(103, 127, L"../Data/Image/TutorialGame_TutoCursor_1.png"));	// 6
	m_pTutoUI.push_back(m_pDXFactory->CreateBitmapObject(1816, 206, L"../Data/Image/TutorialGame_TextBox01.png"));		// 7
	m_pTutoUI.push_back(m_pDXFactory->CreateBitmapObject(1454, 206, L"../Data/Image/TutorialGame_TextBox02.png"));		// 8
	m_pTutoUI.push_back(m_pDXFactory->CreateBitmapObject(1455, 206, L"../Data/Image/TutorialGame_TextBox03.png"));		// 9
	m_pTutoUI.push_back(m_pDXFactory->CreateBitmapObject(1816, 206, L"../Data/Image/TutorialGame_TextBox04.png"));		// 10

	m_pTutoUI.push_back(m_pDXFactory->CreateBitmapObject(290, 220, L"../Data/Image/InGame_TimeLimit_Icon_80.png"));		// 11
	m_pTutoUI.push_back(m_pDXFactory->CreateBitmapObject(290, 220, L"../Data/Image/InGame_TimeLimit_Icon_60.png"));		// 12
	m_pTutoUI.push_back(m_pDXFactory->CreateBitmapObject(290, 220, L"../Data/Image/InGame_TimeLimit_Icon_40.png"));		// 13
	m_pTutoUI.push_back(m_pDXFactory->CreateBitmapObject(290, 220, L"../Data/Image/InGame_TimeLimit_Icon_20.png"));		// 14

	m_pTutoUI.push_back(m_pDXFactory->CreateBitmapObject(1920, 1080, L"../Data/Image/InGame_fail.png"));				// 15
	m_pTutoUI.push_back(m_pDXFactory->CreateBitmapObject(378, 128, L"../Data/Image/Title_Retry.png"));					// 16

	m_pTutoUI.push_back(m_pDXFactory->CreateBitmapObject(1816, 206, L"../Data/Image/Ingame_Clear_Text.png"));			// 17
}

void ResourceManager::CreateMainInGameAssets()
{
	m_pMainBackgroundList.push_back(m_pDXFactory->CreateASEObject((LPSTR)"../Data/ASE/sea_dumi.ASE", L"../Data/Textures/sea.dds"));
	m_pMainBackgroundList.push_back(m_pDXFactory->CreateASEObject((LPSTR)"../Data/ASE/sea_dumi.ASE", L"../Data/Textures/sea.dds"));
	m_pMainBackgroundList.push_back(m_pDXFactory->CreateASEObject((LPSTR)"../Data/ASE/sky_mail.ASE", L"../Data/Textures/sky_mail.dds"));
	m_pMainBackgroundList.push_back(m_pDXFactory->CreateASEObject((LPSTR)"../Data/ASE/cloud_base.ASE", L"../Data/Textures/cloud_base.dds"));
	m_pMainBackgroundList.push_back(m_pDXFactory->CreateASEObject((LPSTR)"../Data/ASE/cloud_big.ASE", L"../Data/Textures/cloud_big.dds"));
	m_pMainBackgroundList.push_back(m_pDXFactory->CreateASEObject((LPSTR)"../Data/ASE/cloud_big.ASE", L"../Data/Textures/cloud_big.dds"));
	m_pMainBackgroundList.push_back(m_pDXFactory->CreateASEObject((LPSTR)"../Data/ASE/cloud_big.ASE", L"../Data/Textures/cloud_big.dds"));
	m_pMainBackgroundList.push_back(m_pDXFactory->CreateASEObject((LPSTR)"../Data/ASE/cloud_big.ASE", L"../Data/Textures/cloud_big.dds"));
	m_pMainBackgroundList.push_back(m_pDXFactory->CreateASEObject((LPSTR)"../Data/ASE/cloud_small.ASE", L"../Data/Textures/cloud_small.dds"));
	m_pMainBackgroundList.push_back(m_pDXFactory->CreateASEObject((LPSTR)"../Data/ASE/house.ASE", L"../Data/Textures/house.dds"));
	m_pMainBackgroundList.push_back(m_pDXFactory->CreateASEObject((LPSTR)"../Data/ASE/sin_1.ASE", L"../Data/Textures/sin_1.dds"));
	m_pMainBackgroundList.push_back(m_pDXFactory->CreateASEObject((LPSTR)"../Data/ASE/house.ASE", L"../Data/Textures/house.dds"));
	m_pMainBackgroundList.push_back(m_pDXFactory->CreateASEObject((LPSTR)"../Data/ASE/sin_2.ASE", L"../Data/Textures/sin_2.dds"));

	m_pMainPlayer = new PlayerNew(m_pDXDevice, m_pDXFactory);

	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Start_Base, eStairState::Base));	// StartBase
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area01_Normal, eStairState::Normal)); // 1
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area01_Normal, eStairState::Normal)); // 2
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area01_Normal, eStairState::Normal)); // 3
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area01_Normal, eStairState::Normal)); // 4
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area01_Normal, eStairState::Normal)); // 5
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area01_Normal, eStairState::Normal)); // 6
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area01_Normal, eStairState::Normal)); // 7
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area01_Normal, eStairState::Normal)); // 8
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area01_Normal, eStairState::Normal)); // 9
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area01_Normal, eStairState::Normal)); // 10
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area01_Normal, eStairState::Normal)); // 11
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area01_Normal, eStairState::Normal)); // 12
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area01_Normal, eStairState::Normal)); // 13
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area01_Normal, eStairState::Normal)); // 14
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area01_Normal, eStairState::Normal)); // 15
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area01_Normal, eStairState::Normal)); // 16
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area01_Normal, eStairState::Normal)); // 17
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area01_Normal, eStairState::Normal)); // 18
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area01_Normal, eStairState::Normal)); // 19
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area01_Normal, eStairState::Normal)); // 20
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area01_Normal, eStairState::Normal)); // 21
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area01_Normal, eStairState::UpDown)); // 22 �̵�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area01_Broken, eStairState::Fall)); // 23 �ı�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area01_Normal, eStairState::Normal)); // 24
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area01_Normal, eStairState::Normal)); // 25
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area01_Normal, eStairState::Normal)); // 26
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area01_Normal, eStairState::Normal)); // 27
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area01_Normal, eStairState::Normal)); // 28
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area01_Broken, eStairState::Fall)); // 29 �ı�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area01_Normal, eStairState::Normal)); // 30
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area01_Normal, eStairState::Normal)); // 31
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area01_Normal, eStairState::Normal)); // 32
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area01_Broken, eStairState::Fall)); // 33 �ı�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area01_Normal, eStairState::Normal)); // 34
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area01_Normal, eStairState::Strafe)); // 35 �̵�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area01_Broken, eStairState::Fall)); // 36 �ı�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area01_Broken, eStairState::Fall)); // 37 �ı�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area01_Normal, eStairState::Normal)); // 38
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area01_Normal, eStairState::Strafe)); // 39 �̵�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area01_Broken, eStairState::Fall)); // 40 �ı�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area01_Normal, eStairState::Normal)); // 41
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area01_Normal, eStairState::Normal)); // 42
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area01_Normal, eStairState::Normal)); // 43
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area01_Broken, eStairState::Fall)); // 44 �ı�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area01_Broken, eStairState::Fall)); // 45 �ı�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area01_Broken, eStairState::Fall)); // 46 �ı�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area01_Broken, eStairState::Fall)); // 47 �ı�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area01_Broken, eStairState::Fall)); // 48 �ı�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Cloud_Base, eStairState::Base)); // 49 ���̺� ����Ʈ

	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Broken, eStairState::Fall)); // 50 �ı�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Normal, eStairState::Strafe)); // 51 �̵�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Broken, eStairState::Fall)); // 52 �ı�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Broken, eStairState::Fall)); // 53 �ı�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Broken, eStairState::Fall)); // 54 �ı�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Normal, eStairState::Normal)); // 55
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Broken, eStairState::Fall)); // 56 �ı�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Broken, eStairState::Fall)); // 57 �ı�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Broken, eStairState::Fall)); // 58 �ı�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Normal, eStairState::FrontBack)); // 59 �̵�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Normal, eStairState::UpDown)); // 60 �̵�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Normal, eStairState::FrontBack)); // 61 �̵�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Normal, eStairState::Normal)); // 62 
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Normal, eStairState::Normal)); // 63 
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Normal, eStairState::Normal)); // 64 
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Broken, eStairState::Fall)); // 65 �ı�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Broken, eStairState::Fall)); // 66 �ı�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Broken, eStairState::Fall)); // 67 �ı�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Broken, eStairState::Fall)); // 68 �ı�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Normal, eStairState::Normal)); // 69
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Normal, eStairState::Normal)); // 70 
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Normal, eStairState::Normal)); // 71
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Normal, eStairState::Normal)); // 72
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Broken, eStairState::Fall)); // 73 �ı�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Normal, eStairState::Normal)); // 74
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Normal, eStairState::Normal)); // 75 
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Normal, eStairState::Normal)); // 76
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Normal, eStairState::Normal)); // 77
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Normal, eStairState::Normal)); // 78
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Broken, eStairState::Fall)); // 79 �ı�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Broken, eStairState::Fall)); // 80 �ı�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Normal, eStairState::Normal)); // 81
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Normal, eStairState::Normal)); // 82
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Normal, eStairState::Normal)); // 83
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Normal, eStairState::Normal)); // 84
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Normal, eStairState::Normal)); // 85
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Normal, eStairState::Normal)); // 86
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Normal, eStairState::Normal)); // 87
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Broken, eStairState::Fall)); // 88 �ı�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Broken, eStairState::Fall)); // 89 �ı�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Broken, eStairState::Fall)); // 90 �ı�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Broken, eStairState::Fall)); // 91 �ı�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Broken, eStairState::Fall)); // 92 �ı�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Broken, eStairState::Fall)); // 93 �ı�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Broken, eStairState::Fall)); // 94 �ı�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Broken, eStairState::Fall)); // 95 �ı�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Normal, eStairState::Normal)); // 96
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Normal, eStairState::Normal)); // 97
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Broken, eStairState::Fall)); // 98 �ı�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Broken, eStairState::Fall)); // 99 �ı�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Broken, eStairState::Fall)); // 100 �ı�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Normal, eStairState::Normal)); // 101
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Normal, eStairState::Normal)); // 102
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Normal, eStairState::Normal)); // 103
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Normal, eStairState::Normal)); // 104
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Normal, eStairState::Normal)); // 105
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Broken, eStairState::Fall)); // 106 �ı�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Broken, eStairState::Fall)); // 107 �ı�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Broken, eStairState::Fall)); // 108 �ı�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Cloud, eStairState::Move)); // 109 ����
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Cloud, eStairState::Move)); // 110 ����
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Cloud, eStairState::Move)); // 111 ����
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Cloud, eStairState::Move)); // 112 ����
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Broken, eStairState::Fall)); // 113 �ı�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Broken, eStairState::Fall)); // 114 �ı�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Broken, eStairState::Fall)); // 115 �ı�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Broken, eStairState::Fall)); // 116 �ı�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Broken, eStairState::Fall)); // 117 �ı�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Broken, eStairState::Fall)); // 118 �ı�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Cloud, eStairState::Move)); // 119 ����
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Cloud, eStairState::Move)); // 120 ����
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Cloud, eStairState::Move)); // 121 ����
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Broken, eStairState::Fall)); // 122 �ı�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Broken, eStairState::Fall)); // 123 �ı�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Broken, eStairState::Fall)); // 124 �ı�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Cloud, eStairState::Move)); // 125 ����
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Cloud, eStairState::Move)); // 126 ����
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Cloud, eStairState::Move)); // 127 ����
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Broken, eStairState::Fall)); // 128 �ı�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Broken, eStairState::Fall)); // 129 �ı�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Broken, eStairState::Fall)); // 130 �ı�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Broken, eStairState::Fall)); // 131 �ı�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Broken, eStairState::Fall)); // 132 �ı�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Broken, eStairState::Fall)); // 133 �ı�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Broken, eStairState::Fall)); // 134 �ı�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Broken, eStairState::Fall)); // 135 �ı�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Normal, eStairState::Normal)); // 136
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Normal, eStairState::Normal)); // 137
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Normal, eStairState::Normal)); // 138
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Broken, eStairState::Fall)); // 139 �ı�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area02_Broken, eStairState::Fall)); // 140 �ı�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Cloud_Base, eStairState::Base)); // 141 ���̺� ����Ʈ

	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area03_Broken, eStairState::Fall)); // 142 �ı�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area03_Broken, eStairState::Fall)); // 143 �ı�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area03_Broken, eStairState::Fall)); // 144 �ı�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area03_Normal, eStairState::Normal)); // 145 
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area03_Broken, eStairState::Fall)); // 146 �ı�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area03_Normal, eStairState::UpDown)); // 147 �̵�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area03_Normal, eStairState::Normal)); // 148 
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area03_Broken, eStairState::Fall)); // 149 �ı�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area03_Normal, eStairState::Strafe)); // 150 �̵�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area03_Normal, eStairState::Normal)); // 151
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area03_Normal, eStairState::Normal)); // 152
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area03_Normal, eStairState::Normal)); // 153
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area03_Normal, eStairState::Normal)); // 154
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area03_Broken, eStairState::Fall)); // 155 �ı�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area03_Broken, eStairState::Fall)); // 156 �ı�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area03_Broken, eStairState::Fall)); // 157 �ı�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area03_Broken, eStairState::Fall)); // 158 �ı�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area03_Broken, eStairState::Fall)); // 159 �ı�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area03_Normal, eStairState::UpDown)); // 160 �̵�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area03_Normal, eStairState::Normal)); // 161
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area03_Normal, eStairState::Normal)); // 162
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area03_Normal, eStairState::Diagonal)); // 163 �̵�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area03_Normal, eStairState::Normal)); // 164
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area03_Broken, eStairState::Fall)); // 165 �ı�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area03_Normal, eStairState::Diagonal)); // 166 �̵�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area03_Normal, eStairState::Normal)); // 167
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area03_Normal, eStairState::Diagonal)); // 168 �̵�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area03_Broken, eStairState::Fall)); // 169 �ı�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area03_Broken, eStairState::Fall)); // 170 �ı�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area03_Broken, eStairState::Fall)); // 171 �ı�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area03_Broken, eStairState::Fall)); // 172 �ı�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area03_Broken, eStairState::Fall)); // 173 �ı�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area03_Normal, eStairState::Strafe)); // 174 �̵�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area03_Normal, eStairState::Strafe)); // 175 �̵�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area03_Broken, eStairState::Fall)); // 176 �ı�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area03_Normal, eStairState::Normal)); // 177
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area03_Broken, eStairState::Fall)); // 178 �ı�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area03_Broken, eStairState::Fall)); // 179 �ı�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area03_Broken, eStairState::Fall)); // 180 �ı�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area03_Broken, eStairState::Fall)); // 181 �ı�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area03_Normal, eStairState::Normal)); // 182
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area03_Broken, eStairState::Fall)); // 183 �ı�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area03_Normal, eStairState::Normal)); // 184
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area03_Normal, eStairState::Normal)); // 185
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area03_Normal, eStairState::Normal)); // 186
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area03_Broken, eStairState::Fall)); // 187 �ı�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area03_Broken, eStairState::Fall)); // 188 �ı�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area03_Broken, eStairState::Fall)); // 189 �ı�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::Area03_Broken, eStairState::Fall)); // 190 �ı�
	m_pMainStairList.push_back(new Stair(m_pDXFactory, eStairType::End_Base, eStairState::Base)); // 191

	m_pMainBirdList.push_back(new Bird(m_pDXFactory, eBirdType::BIG));
	m_pMainBirdList.push_back(new Bird(m_pDXFactory, eBirdType::SMALL));
	m_pMainBirdList.push_back(new Bird(m_pDXFactory, eBirdType::BIG));

	m_pMainUI.push_back(m_pDXFactory->CreateBitmapObject(150, 920, L"../Data/Image/InGame_Bar.png"));
	m_pMainUI.push_back(m_pDXFactory->CreateBitmapObject(120, 101, L"../Data/Image/InGame_CharacterFace.png"));
	m_pMainUI.push_back(m_pDXFactory->CreateBitmapObject(290, 220, L"../Data/Image/InGame_TimeLimit_Background.png"));
	m_pMainUI.push_back(m_pDXFactory->CreateBitmapObject(290, 220, L"../Data/Image/InGame_TimeLimit_Icon.png"));
	m_pMainUI.push_back(m_pDXFactory->CreateBitmapObject(1816, 206, L"../Data/Image/Ingame_Clear_Text.png"));
	m_pMainUI.push_back(m_pDXFactory->CreateBitmapObject(1920, 1080, L"../Data/Image/InGame_fail.png"));
	m_pMainUI.push_back(m_pDXFactory->CreateBitmapObject(378, 128, L"../Data/Image/Title_Retry.png"));
}

void ResourceManager::Finalize()
{
	// Tutorial
	for (UINT i = 0; i < m_pTutoUI.size(); i++)
	{
		m_pTutoUI[i]->Finalize();
	}

	m_pTutoUI.clear();

	for (UINT i = 0; i < m_pTutoBirdList.size(); i++)
	{
		m_pTutoBirdList[i]->Finalize();
	}

	m_pTutoBirdList.clear();

	for (UINT i = 0; i < m_pTutoStairList.size(); i++)
	{
		m_pTutoStairList[i]->Finalize();
	}

	m_pTutoStairList.clear();

	m_pTutoPlayer->Finalize();
	delete m_pTutoPlayer;
	m_pTutoPlayer = nullptr;

	for (UINT i = 0; i < m_pTutoBackgroundList.size(); i++)
	{
		m_pTutoBackgroundList[i]->Finalize();
	}

	m_pTutoBackgroundList.clear();

	// Main
	for (UINT i = 0; i < m_pMainUI.size(); i++)
	{
		m_pMainUI[i]->Finalize();
	}

	m_pMainUI.clear();

	for (UINT i = 0; i < m_pMainBirdList.size(); i++)
	{
		m_pMainBirdList[i]->Finalize();
	}

	m_pMainBirdList.clear();

	for (UINT i = 0; i < m_pMainStairList.size(); i++)
	{
		m_pMainStairList[i]->Finalize();
	}

	m_pMainStairList.clear();

	m_pMainPlayer->Finalize();
	delete m_pMainPlayer;
	m_pMainPlayer = nullptr;

	for (UINT i = 0; i < m_pMainBackgroundList.size(); i++)
	{
		m_pMainBackgroundList[i]->Finalize();
	}

	m_pMainBackgroundList.clear();
}