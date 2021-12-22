#include "MainInGameScene.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "GameTimer.h"

// DirectX
#include "IDXObjectBase.h"
#include "DXFactory.h"
#include "D3Camera.h"
#include "DXRenderer.h"
#include "Transform.h"
#include "BoxCollider.h"

// Object
#include "Stair.h"
#include "Bird.h"
#include "PlayerNew.h"

#include "SoundManager.h"

MainInGameScene::MainInGameScene()
	: m_pPlayerNew(nullptr),
	m_pResetBirds(nullptr),
	m_pResetForwardClouds(nullptr),
	m_pResetBackwardClouds(nullptr),
	m_ClearDelayTime(0.0f),
	CharacterY(0.0f),
	m_GameClear(false),
	m_pClearStage(nullptr),
	m_TimeLimit(0.0f),
	m_TimeOut(false),
	m_TimeNextScene(0.0f)
{

}

MainInGameScene::~MainInGameScene()
{

}

void MainInGameScene::Initialize(DXDevice* pDevice, DXFactory* pFactory, ResourceManager* pResourceManager)
{
	m_pBackground = pResourceManager->m_pMainBackgroundList;

	m_pBackground[0]->SetPosition(0.0f, -180.0f, 100.0f);
	m_pBackground[1]->SetPosition(0.0f, -180.0f, 100.0f);
	m_pBackground[2]->SetPosition(0.0f, 120.0f, 430.0f);
	m_pBackground[2]->SetScale(10.0f, 10.0f, 10.0f);
	m_pBackground[2]->SetRotation('Y', 90.0f);
	m_pBackground[3]->SetPosition(0.0f, 115.0f, 430.0f);
	m_pBackground[3]->SetScale(1.0f, 1.0f, 1.0f);
	m_pBackground[3]->SetRotation('Y', 0.0f);
	m_pBackground[4]->SetPosition(20.0f, 100.0f, 380.0f);
	m_pBackground[4]->SetScale(3.0f, 3.0f, 3.0f);
	m_pBackground[4]->SetRotation('Y', 0.0f);
	m_pBackground[5]->SetPosition(45.0f, 112.0f, 386.0f);
	m_pBackground[5]->SetScale(1.0f, 1.0f, 1.0f);
	m_pBackground[5]->SetRotation('Y', -180.0f);
	m_pBackground[6]->SetPosition(-30.0f, 80.0f, 360.0f);
	m_pBackground[6]->SetScale(1.0f, 1.0f, 1.0f);
	m_pBackground[6]->SetRotation('Y', -180.0f);
	m_pBackground[7]->SetPosition(-50.0f, 105.0f, 370.0f);
	m_pBackground[7]->SetScale(5.0f, 5.0f, 5.0f);
	m_pBackground[7]->SetRotation('Y', 0.0f);
	m_pBackground[8]->SetPosition(-60.0f, 115.0f, 460.0f);
	m_pBackground[8]->SetScale(5.0f, 5.0f, 5.0f);
	m_pBackground[8]->SetRotation('Y', 0.0f);
	m_pBackground[9]->SetPosition(-60.0f, 100.0f, 570.0f);
	m_pBackground[9]->SetScale(4.0f, 4.0f, 4.0f);
	m_pBackground[9]->SetRotation('Y', -40.0f);
	m_pBackground[10]->SetPosition(-100.0f, 110.0f, 400.0f);
	m_pBackground[10]->SetScale(1.0f, 1.0f, 1.0f);
	m_pBackground[10]->SetRotation('Y', -40.0f);
	m_pBackground[11]->SetPosition(34.0f, 110.0f, 460.0f);
	m_pBackground[11]->SetScale(1.0f, 1.0f, 1.0f);
	m_pBackground[11]->SetRotation('Y', 40.0f);
	m_pBackground[12]->SetPosition(41.0f, 110.0f, 420.0f);
	m_pBackground[12]->SetScale(1.0f, 1.0f, 1.0f);
	m_pBackground[12]->SetRotation('Y', 0.0f);

	// 캐릭터
	m_pPlayerNew = pResourceManager->m_pMainPlayer;
	m_pPlayerNew->Initialize(0.0f, 2.85f, -8.0f, this);
	m_pPlayerNew->SetTextState(eTextState::TextOff);

	// 캐릭터와 세이브포인트 관련
	AddSavePoint("Base", m_pPlayerNew->GetPosition());
	AddSavePoint("SavePoint01", Vector3(0.0f, 36.5f, 96.0f));
	AddSavePoint("SavePoint02", Vector3(0.0f, 69.5f, 200.0f));
	AddSavePoint("Cheat", Vector3(0.0f, 101.7f, 306.0f));

	// 첫 번째 세이브포인트를 캐릭터에 넣어준다.
	m_pPlayerNew->SetNowSavePoint(m_pVecSavePoints[(int)eSavePoint::Base]);

	// Objects 생성
	m_pStairList = pResourceManager->m_pMainStairList;
	m_pStairList[0]->Initialize(0.0f, 2.7f, -3.0f);

	/// 지역 1
	//////////////////////////////////////////////////////////////////////////
	m_pStairList[1]->Initialize(-8.0f, 3.0f, 8.0f);
	m_pStairList[2]->Initialize(0.0f, 3.0f, 8.0f);
	m_pStairList[3]->Initialize(8.0f, 3.0f, 8.0f);
	m_pStairList[4]->Initialize(-8.0f, 6.0f, 16.0f);
	m_pStairList[5]->Initialize(8.0f, 6.0f, 16.0f);
	m_pStairList[6]->Initialize(-40.0f, 18.0f, 24.0f);
	m_pStairList[7]->Initialize(-32.0f, 15.0f, 24.0f);
	m_pStairList[8]->Initialize(-24.0f, 12.0f, 24.0f);
	m_pStairList[9]->Initialize(-8.0f, 9.0f, 24.0f);
	m_pStairList[10]->Initialize(8.0f, 9.0f, 24.0f);
	m_pStairList[11]->Initialize(-40.0f, 21.0f, 32.0f);
	m_pStairList[12]->Initialize(-24.0f, 15.0f, 32.0f);
	m_pStairList[13]->Initialize(-8.0f, 12.0f, 32.0f);
	m_pStairList[14]->Initialize(8.0f, 12.0f, 32.0f);
	m_pStairList[15]->Initialize(-40.0f, 24.0f, 40.0f);
	m_pStairList[16]->Initialize(-32.0f, 15.0f, 40.0f);
	m_pStairList[17]->Initialize(-24.0f, 18.0f, 40.0f);
	m_pStairList[18]->Initialize(-16.0f, 18.0f, 40.0f);
	m_pStairList[19]->Initialize(-8.0f, 15.0f, 40.0f);
	m_pStairList[20]->Initialize(8.0f, 15.0f, 40.0f);
	m_pStairList[21]->Initialize(16.0f, 18.0f, 40.0f);
	m_pStairList[22]->Initialize(24.0f, 18.0f, 40.0f);
	m_pStairList[23]->Initialize(32.0f, 24.0f, 40.0f);
	m_pStairList[24]->Initialize(-40.0f, 24.0f, 48.0f);
	m_pStairList[25]->Initialize(-32.0f, 18.0f, 48.0f);
	m_pStairList[26]->Initialize(-16.0f, 15.0f, 48.0f);
	m_pStairList[27]->Initialize(-8.0f, 18.0f, 48.0f);
	m_pStairList[28]->Initialize(8.0f, 18.0f, 48.0f);
	m_pStairList[29]->Initialize(32.0f, 24.0f, 48.0f);
	m_pStairList[30]->Initialize(-40.0f, 24.0f, 56.0f);
	m_pStairList[31]->Initialize(-32.0f, 24.0f, 56.0f);
	m_pStairList[32]->Initialize(-24.0f, 24.0f, 56.0f);
	m_pStairList[33]->Initialize(32.0f, 24.0f, 56.0f);
	m_pStairList[34]->Initialize(-40.0f, 24.0f, 64.0f);
	m_pStairList[35]->Initialize(-8.0f, 24.0f, 64.0f);
	m_pStairList[36]->Initialize(24.0f, 24.0f, 64.0f);
	m_pStairList[37]->Initialize(32.0f, 24.0f, 64.0f);
	m_pStairList[38]->Initialize(-40.0f, 27.0f, 72.0f);
	m_pStairList[39]->Initialize(-8.0f, 27.0f, 72.0f);
	m_pStairList[40]->Initialize(24.0f, 27.0f, 72.0f);
	m_pStairList[41]->Initialize(-32.0f, 30.0f, 80.0f);
	m_pStairList[42]->Initialize(-24.0f, 30.0f, 80.0f);
	m_pStairList[43]->Initialize(-16.0f, 30.0f, 80.0f);
	m_pStairList[44]->Initialize(-8.0f, 30.0f, 80.0f);
	m_pStairList[45]->Initialize(0.0f, 30.0f, 80.0f);
	m_pStairList[46]->Initialize(8.0f, 30.0f, 80.0f);
	m_pStairList[47]->Initialize(16.0f, 30.0f, 80.0f);
	m_pStairList[48]->Initialize(24.0f, 30.0f, 80.0f);
	m_pStairList[49]->Initialize(0.0f, 35.5f, 96.0f); // 세이브 포인트

	m_pResetBirds = new BoxCollider(pDevice);
	m_pResetBirds->Initialize(5.0f, 20.0f, 20.f);
	m_pResetBirds->SetPosition(48.0f, 27.0f, 72.0f);

	// 새
	m_pBirdList = pResourceManager->m_pMainBirdList;
	m_pBirdList[0]->Initialize(-64.0f, 29.0f, 64.0f);
	m_pBirdList[1]->Initialize(-64.0f, 32.0f, 72.0f);
	m_pBirdList[2]->Initialize(-64.0f, 35.0f, 80.0f);
	//////////////////////////////////////////////////////////////////////////

	/// 지역 2
	//////////////////////////////////////////////////////////////////////////
	m_pResetForwardClouds = new BoxCollider(pDevice);
	m_pResetForwardClouds->Initialize(5.0f, 20.0f, 30.f);
	m_pResetForwardClouds->SetPosition(20.0f, 54.0f, 165.0f);

	m_pResetBackwardClouds = new BoxCollider(pDevice);
	m_pResetBackwardClouds->Initialize(5.0f, 20.0f, 30.f);
	m_pResetBackwardClouds->SetPosition(-20.0f, 54.0f, 165.0f);

	m_pStairList[50]->Initialize(24.0f, 30.0f, 88.0f);
	m_pStairList[51]->Initialize(-32.0f, 48.0f, 96.0f);
	m_pStairList[52]->Initialize(24.0f, 33.0f, 96.0f);
	m_pStairList[53]->Initialize(40.0f, 33.0f, 96.0f);
	m_pStairList[54]->Initialize(48.0f, 30.0f, 96.0f);
	m_pStairList[55]->Initialize(-24.0f, 45.0f, 104.0f);
	m_pStairList[56]->Initialize(24.0f, 36.0f, 104.0f);
	m_pStairList[57]->Initialize(40.0f, 36.0f, 104.0f);
	m_pStairList[58]->Initialize(48.0f, 30.0f, 104.0f);
	m_pStairList[59]->Initialize(-48.0f, 48.0f, 112.0f);
	m_pStairList[60]->Initialize(-40.0f, 36.0f, 112.0f);
	m_pStairList[61]->Initialize(-32.0f, 36.0f, 112.0f);
	m_pStairList[62]->Initialize(-24.0f, 42.0f, 112.0f);
	m_pStairList[63]->Initialize(-8.0f, 36.0f, 112.0f);
	m_pStairList[64]->Initialize(8.0f, 36.0f, 112.0f);
	m_pStairList[65]->Initialize(24.0f, 39.0f, 112.0f);
	m_pStairList[66]->Initialize(32.0f, 39.0f, 112.0f);
	m_pStairList[67]->Initialize(40.0f, 39.0f, 112.0f);
	m_pStairList[68]->Initialize(48.0f, 30.0f, 112.0f);
	m_pStairList[69]->Initialize(-40.0f, 39.0f, 120.0f);
	m_pStairList[70]->Initialize(-24.0f, 39.0f, 120.0f);
	m_pStairList[71]->Initialize(-8.0f, 39.0f, 120.0f);
	m_pStairList[72]->Initialize(8.0f, 39.0f, 120.0f);
	m_pStairList[73]->Initialize(48.0f, 30.0f, 120.0f);
	m_pStairList[74]->Initialize(-40.0f, 42.0f, 128.0f);
	m_pStairList[75]->Initialize(-32.0f, 36.0f, 128.0f);
	m_pStairList[76]->Initialize(-24.0f, 36.0f, 128.0f);
	m_pStairList[77]->Initialize(-8.0f, 42.0f, 128.0f);
	m_pStairList[78]->Initialize(8.0f, 42.0f, 128.0f);
	m_pStairList[79]->Initialize(40.0f, 36.0f, 128.0f);
	m_pStairList[80]->Initialize(48.0f, 33.0f, 128.0f);
	m_pStairList[81]->Initialize(-48.0f, 51.0f, 136.0f);
	m_pStairList[82]->Initialize(-40.0f, 45.0f, 136.0f);
	m_pStairList[83]->Initialize(-32.0f, 45.0f, 136.0f);
	m_pStairList[84]->Initialize(-24.0f, 45.0f, 136.0f);
	m_pStairList[85]->Initialize(-16.0f, 45.0f, 136.0f);
	m_pStairList[86]->Initialize(-8.0f, 45.0f, 136.0f);
	m_pStairList[87]->Initialize(8.0f, 45.0f, 136.0f);
	m_pStairList[88]->Initialize(16.0f, 45.0f, 136.0f);
	m_pStairList[89]->Initialize(24.0f, 42.0f, 136.0f);
	m_pStairList[90]->Initialize(32.0f, 39.0f, 136.0f);
	m_pStairList[91]->Initialize(40.0f, 36.0f, 136.0f);
	m_pStairList[92]->Initialize(48.0f, 39.0f, 136.0f);
	m_pStairList[93]->Initialize(64.0f, 42.0f, 136.0f);
	m_pStairList[94]->Initialize(72.0f, 39.0f, 136.0f);
	m_pStairList[95]->Initialize(80.0f, 39.0f, 136.0f);
	m_pStairList[96]->Initialize(-48.0f, 51.0f, 144.0f);
	m_pStairList[97]->Initialize(-8.0f, 48.0f, 144.0f);
	m_pStairList[98]->Initialize(48.0f, 42.0f, 144.0f);
	m_pStairList[99]->Initialize(64.0f, 42.0f, 144.0f);
	m_pStairList[100]->Initialize(80.0f, 39.0f, 144.0f);
	m_pStairList[101]->Initialize(-48.0f, 51.0f, 152.0f);
	m_pStairList[102]->Initialize(-40.0f, 51.0f, 152.0f);
	m_pStairList[103]->Initialize(-32.0f, 51.0f, 152.0f);
	m_pStairList[104]->Initialize(-24.0f, 51.0f, 152.0f);
	m_pStairList[105]->Initialize(-16.0f, 51.0f, 152.0f);
	m_pStairList[106]->Initialize(48.0f, 45.0f, 152.0f);
	m_pStairList[107]->Initialize(64.0f, 45.0f, 152.0f);
	m_pStairList[108]->Initialize(80.0f, 39.0f, 152.0f);
	m_pStairList[109]->Initialize(-56.0f, 54.0f, 160.0f); // 구름
	m_pStairList[110]->Initialize(-56.0f, 54.0f, 160.0f); // 구름
	m_pStairList[111]->Initialize(-56.0f, 54.0f, 160.0f); // 구름
	m_pStairList[112]->Initialize(-56.0f, 54.0f, 160.0f); // 구름
	m_pStairList[113]->Initialize(24.0f, 48.0f, 160.0f);
	m_pStairList[114]->Initialize(32.0f, 45.0f, 160.0f);
	m_pStairList[115]->Initialize(48.0f, 48.0f, 160.0f);
	m_pStairList[116]->Initialize(56.0f, 48.0f, 160.0f);
	m_pStairList[117]->Initialize(64.0f, 48.0f, 160.0f);
	m_pStairList[118]->Initialize(80.0f, 39.0f, 160.0f);
	m_pStairList[119]->Initialize(-56.0f, 57.0f, 168.0f); // 구름
	m_pStairList[120]->Initialize(-56.0f, 57.0f, 168.0f); // 구름
	m_pStairList[121]->Initialize(-56.0f, 57.0f, 168.0f); // 구름
	m_pStairList[122]->Initialize(24.0f, 51.0f, 168.0f);
	m_pStairList[123]->Initialize(32.0f, 42.0f, 168.0f);
	m_pStairList[124]->Initialize(80.0f, 39.0f, 168.0f);
	m_pStairList[125]->Initialize(-56.0f, 60.0f, 176.0f); // 구름
	m_pStairList[126]->Initialize(-56.0f, 60.0f, 176.0f); // 구름
	m_pStairList[127]->Initialize(-56.0f, 60.0f, 176.0f); // 구름
	m_pStairList[128]->Initialize(24.0f, 54.0f, 176.0f);
	m_pStairList[129]->Initialize(32.0f, 39.0f, 176.0f);
	m_pStairList[130]->Initialize(40.0f, 39.0f, 176.0f);
	m_pStairList[131]->Initialize(48.0f, 39.0f, 176.0f);
	m_pStairList[132]->Initialize(56.0f, 39.0f, 176.0f);
	m_pStairList[133]->Initialize(64.0f, 39.0f, 176.0f);
	m_pStairList[134]->Initialize(72.0f, 39.0f, 176.0f);
	m_pStairList[135]->Initialize(80.0f, 39.0f, 176.0f);
	m_pStairList[136]->Initialize(-8.0f, 63.0f, 184.0f);
	m_pStairList[137]->Initialize(0.0f, 63.0f, 184.0f);
	m_pStairList[138]->Initialize(8.0f, 63.0f, 184.0f);
	m_pStairList[139]->Initialize(16.0f, 60.0f, 184.0f);
	m_pStairList[140]->Initialize(24.0f, 57.0f, 184.0f);
	m_pStairList[141]->Initialize(0.0f, 68.5f, 200.0f);
	/////////////////////////////////////////////////////////////////////////

	/// 지역 3
	//////////////////////////////////////////////////////////////////////////
	m_pStairList[142]->Initialize(24.0f, 60.0f, 192.0f);
	m_pStairList[143]->Initialize(24.0f, 63.0f, 200.0f);
	m_pStairList[144]->Initialize(24.0f, 63.0f, 208.0f);
	m_pStairList[145]->Initialize(0.0f, 69.0f, 216.0f);
	m_pStairList[146]->Initialize(24.0f, 63.0f, 216.0f);
	m_pStairList[147]->Initialize(-24.0f, 81.0f, 224.0f); // 이동
	m_pStairList[148]->Initialize(0.0f, 72.0f, 224.0f);
	m_pStairList[149]->Initialize(24.0f, 63.0f, 224.0f);
	m_pStairList[150]->Initialize(-48.0f, 78.0f, 232.0f); // 이동
	m_pStairList[151]->Initialize(-32.0f, 75.0f, 232.0f);
	m_pStairList[152]->Initialize(-24.0f, 78.0f, 232.0f);
	m_pStairList[153]->Initialize(-24.0f, 90.0f, 232.0f);
	m_pStairList[154]->Initialize(0.0f, 75.0f, 232.0f);
	m_pStairList[155]->Initialize(8.0f, 72.0f, 232.0f);
	m_pStairList[156]->Initialize(16.0f, 69.0f, 232.0f);
	m_pStairList[157]->Initialize(24.0f, 66.0f, 232.0f);
	m_pStairList[158]->Initialize(32.0f, 63.0f, 232.0f);
	m_pStairList[159]->Initialize(40.0f, 60.0f, 232.0f);
	m_pStairList[160]->Initialize(-48.0f, 78.0f, 240.0f); // 이동
	m_pStairList[161]->Initialize(-32.0f, 78.0f, 240.0f);
	m_pStairList[162]->Initialize(-24.0f, 90.0f, 240.0f);
	m_pStairList[163]->Initialize(-16.0f, 78.0f, 240.0f); // 이동
	m_pStairList[164]->Initialize(-16.0f, 90.0f, 240.0f);
	m_pStairList[165]->Initialize(40.0f, 60.0f, 240.0f);
	m_pStairList[166]->Initialize(-56.0f, 78.0f, 248.0f); // 이동
	m_pStairList[167]->Initialize(-32.0f, 81.0f, 248.0f);
	m_pStairList[168]->Initialize(0.0f, 87.0f, 248.0f); // 이동
	m_pStairList[169]->Initialize(40.0f, 60.0f, 248.0f);
	m_pStairList[170]->Initialize(16.0f, 69.0f, 256.0f);
	m_pStairList[171]->Initialize(24.0f, 66.0f, 256.0f);
	m_pStairList[172]->Initialize(32.0f, 63.0f, 256.0f);
	m_pStairList[173]->Initialize(40.0f, 60.0f, 256.0f);
	m_pStairList[174]->Initialize(-32.0f, 87.0f, 264.0f); // 이동
	m_pStairList[175]->Initialize(0.0f, 87.0f, 264.0f); // 이동
	m_pStairList[176]->Initialize(16.0f, 72.0f, 264.0f);
	m_pStairList[177]->Initialize(0.0f, 90.0f, 272.0f);
	m_pStairList[178]->Initialize(16.0f, 75.0f, 272.0f);
	m_pStairList[179]->Initialize(24.0f, 78.0f, 272.0f);
	m_pStairList[180]->Initialize(32.0f, 81.0f, 272.0f);
	m_pStairList[181]->Initialize(40.0f, 84.0f, 272.0f);
	m_pStairList[182]->Initialize(0.0f, 93.0f, 280.0f);
	m_pStairList[183]->Initialize(40.0f, 87.0f, 280.0f);
	m_pStairList[184]->Initialize(-8.0f, 96.0f, 288.0f);
	m_pStairList[185]->Initialize(0.0f, 96.0f, 288.0f);
	m_pStairList[186]->Initialize(8.0f, 96.0f, 288.0f);
	m_pStairList[187]->Initialize(16.0f, 96.0f, 288.0f);
	m_pStairList[188]->Initialize(24.0f, 96.0f, 288.0f);
	m_pStairList[189]->Initialize(32.0f, 93.0f, 288.0f);
	m_pStairList[190]->Initialize(40.0f, 90.0f, 288.0f);
	m_pStairList[191]->Initialize(0.0f, 101.0f, 328.0f);

	m_pClearStage = new BoxCollider(pDevice);
	m_pClearStage->Initialize(4.0f, 7.0f, 4.0f);
	m_pClearStage->SetPosition(0.0f, 105.0f, 332.5f);
	//////////////////////////////////////////////////////////////////////////

	m_pUI = pResourceManager->m_pMainUI;
	m_pUI[0]->SetBitmapPosition(40, 70);
	m_pUI[1]->SetBitmapPosition(55, 850);
	m_pUI[2]->SetBitmapPosition(1620, 815);
	m_pUI[3]->SetBitmapPosition(1620, 815);
	m_pUI[4]->SetBitmapPosition(52, -500);	// Text UI 05
	m_pUI[5]->SetBitmapPosition(0, 0);
	m_pUI[6]->SetBitmapPosition(780, 700);

	/// <summary>
	/// 지연 카메라의 초깃값
	/// </summary>
	XMFLOAT3 cameraPos = XMFLOAT3(8.0f, 4.0f, -20.0f);
	cameraPos.x += m_pPlayerNew->m_pPlayerList[0]->GetPosition().x;
	cameraPos.y += m_pPlayerNew->m_pPlayerList[0]->GetPosition().y;
	cameraPos.z += m_pPlayerNew->m_pPlayerList[0]->GetPosition().z;

	m_DelayCamPos = cameraPos;

	SoundManager::GetInstance()->Resume(3, "BGM_InGame");
}

void MainInGameScene::Update(HWND hWnd, SceneManager* pSceneManager, GameTimer* pTimer, D3Camera* pCamera)
{
	for (UINT i = 0; i < m_pBackground.size(); i++)
	{
		m_pBackground[i]->Update(pTimer, pCamera);
	}

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

	// 10분이 지나면
	if (600.0f < m_TimeLimit)	// 임시 테스트 10초 -> 기존 300.0f
	{
		m_TimeImageOn = true;

		m_TimeNextScene += pTimer->DeltaTime();

		if (3.0f < m_TimeNextScene)
		{
			m_TimeOut = true;
		}

	}

	if (m_pUI[6]->GetPick())
	{
		m_TimeOut = true;
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
	m_pStairList[22]->UpDown(12.0f, 24.0f, 5.0f * pTimer->DeltaTime(), m_pPlayerNew);
	m_pStairList[23]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[29]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[33]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[35]->Strafe(8.0f, -24.0f, 10.0f * pTimer->DeltaTime(), false, m_pPlayerNew);
	m_pStairList[36]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[37]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[39]->Strafe(-16.0f, 0.0f, 5.0f * pTimer->DeltaTime(), true, m_pPlayerNew);
	m_pStairList[40]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[44]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[45]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[46]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[47]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[48]->Fall(-15.0f, m_pPlayerNew);

	m_pBirdList[0]->GetBirdObject()->SetTranslation(10.0f * pTimer->DeltaTime(), 0.0f, 0.0f); // 이전 수치 -10.0f
	m_pBirdList[1]->GetBirdObject()->SetTranslation(30.0f * pTimer->DeltaTime(), 0.0f, 0.0f); // 이전 수치 -20.0f
	m_pBirdList[2]->GetBirdObject()->SetTranslation(15.0f * pTimer->DeltaTime(), 0.0f, 0.0f); // 이전 수치 -10.0f

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


	// Save Point 01 구름에 도달했으므로 캐릭터의 Reset Position을 변경한다
	if (m_pStairList[49]->GetStairObject()->GetBoxCollider()->CheckCollided(m_pPlayerNew->m_pPlayerList[0]->GetBoxCollider()))
	{
		m_pPlayerNew->SetNowSavePoint(m_pVecSavePoints[(int)eSavePoint::SavePoint01]);
	}


	/// 지역 2
	//////////////////////////////////////////////////////////////////////////
	m_pStairList[50]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[51]->Strafe(-40.0f, -24.0f, 5.0f * pTimer->DeltaTime(), true, m_pPlayerNew);
	m_pStairList[52]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[53]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[54]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[56]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[57]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[58]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[59]->FrontBack(96.0f, 128.0f, 10.0f * pTimer->DeltaTime(), m_pPlayerNew);
	m_pStairList[60]->UpDown(33.0f, 39.0f, 5.0f * pTimer->DeltaTime(), m_pPlayerNew);
	m_pStairList[61]->FrontBack(104.0f, 120.0f, 5.0f * pTimer->DeltaTime(), m_pPlayerNew);
	m_pStairList[65]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[66]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[67]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[68]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[73]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[79]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[80]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[88]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[89]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[90]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[91]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[92]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[93]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[94]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[95]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[98]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[99]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[100]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[106]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[107]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[108]->Fall(-15.0f, m_pPlayerNew);

	Vector3 MoveVector(5.0f * pTimer->DeltaTime(), 0.0f, 0.0f);
	Vector3 MoveInverseVector(-5.0f * pTimer->DeltaTime(), 0.0f, 0.0f);

	m_pStairList[109]->Move(MoveVector, m_pResetForwardClouds, 0, m_pPlayerNew);
	m_pStairList[110]->Move(MoveVector, m_pResetForwardClouds, 1, m_pPlayerNew);
	m_pStairList[111]->Move(MoveVector, m_pResetForwardClouds, 2, m_pPlayerNew);
	m_pStairList[112]->Move(MoveVector, m_pResetForwardClouds, 3, m_pPlayerNew);

	if (m_pStairList[109]->GetStairObject()->GetBoxCollider()->CheckCollided(m_pResetForwardClouds))
	{
		m_pStairList[109]->Reset();
	}

	if (m_pStairList[110]->GetStairObject()->GetBoxCollider()->CheckCollided(m_pResetForwardClouds))
	{
		m_pStairList[110]->Reset();
	}

	if (m_pStairList[111]->GetStairObject()->GetBoxCollider()->CheckCollided(m_pResetForwardClouds))
	{
		m_pStairList[111]->Reset();
	}

	if (m_pStairList[112]->GetStairObject()->GetBoxCollider()->CheckCollided(m_pResetForwardClouds))
	{
		m_pStairList[112]->Reset();
	}

	m_pStairList[113]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[114]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[115]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[116]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[117]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[118]->Fall(-15.0f, m_pPlayerNew);

	m_pStairList[119]->Move(MoveVector, m_pResetForwardClouds, 0, m_pPlayerNew);
	m_pStairList[120]->Move(MoveVector, m_pResetForwardClouds, 2, m_pPlayerNew);
	m_pStairList[121]->Move(MoveVector, m_pResetForwardClouds, 3, m_pPlayerNew);

	if (m_pStairList[119]->GetStairObject()->GetBoxCollider()->CheckCollided(m_pResetForwardClouds))
	{
		m_pStairList[119]->Reset();
	}

	if (m_pStairList[120]->GetStairObject()->GetBoxCollider()->CheckCollided(m_pResetForwardClouds))
	{
		m_pStairList[120]->Reset();
	}

	if (m_pStairList[121]->GetStairObject()->GetBoxCollider()->CheckCollided(m_pResetForwardClouds))
	{
		m_pStairList[121]->Reset();
	}

	m_pStairList[122]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[123]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[124]->Fall(-15.0f, m_pPlayerNew);

	m_pStairList[125]->Move(MoveVector, m_pResetForwardClouds, 1, m_pPlayerNew);
	m_pStairList[126]->Move(MoveVector, m_pResetForwardClouds, 2, m_pPlayerNew);
	m_pStairList[127]->Move(MoveVector, m_pResetForwardClouds, 3, m_pPlayerNew);

	if (m_pStairList[125]->GetStairObject()->GetBoxCollider()->CheckCollided(m_pResetForwardClouds))
	{
		m_pStairList[125]->Reset();
	}

	if (m_pStairList[126]->GetStairObject()->GetBoxCollider()->CheckCollided(m_pResetForwardClouds))
	{
		m_pStairList[126]->Reset();
	}

	if (m_pStairList[127]->GetStairObject()->GetBoxCollider()->CheckCollided(m_pResetForwardClouds))
	{
		m_pStairList[127]->Reset();
	}

	m_pStairList[128]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[129]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[130]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[131]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[132]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[133]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[134]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[135]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[139]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[140]->Fall(-15.0f, m_pPlayerNew);
	//////////////////////////////////////////////////////////////////////////


	// Save Point 02 구름에 도달했으므로 캐릭터의 Reset Position을 변경한다
	if (m_pStairList[141]->GetStairObject()->GetBoxCollider()->CheckCollided(m_pPlayerNew->m_pPlayerList[0]->GetBoxCollider()))
	{
		m_pPlayerNew->SetNowSavePoint(m_pVecSavePoints[(int)eSavePoint::SavePoint02]);
	}


	/// 지역 3
	//////////////////////////////////////////////////////////////////////////
	m_pStairList[142]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[143]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[144]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[146]->Fall(0.0f, m_pPlayerNew);
	m_pStairList[147]->UpDown(78.0f, 90.0f, 5.0f * pTimer->DeltaTime(), m_pPlayerNew);
	m_pStairList[149]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[150]->Strafe(-56.0f, -40.0f, 5.0f * pTimer->DeltaTime(), true, m_pPlayerNew);
	m_pStairList[155]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[156]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[157]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[158]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[159]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[160]->UpDown(72.0f, 84.0f, 5.0f * pTimer->DeltaTime(), m_pPlayerNew);
	m_pStairList[163]->StrafeDiagonal(-8.0f, -24.0f, 70.0f, 81.0f, 232.0f, 248.0f, 0.5f * pTimer->DeltaTime(), m_pPlayerNew);
	m_pStairList[165]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[166]->StrafeDiagonal(-56.0f, -56.0f, 72.0f, 84.0f, 240.0f, 256.0f, 0.5f * pTimer->DeltaTime(), m_pPlayerNew);
	m_pStairList[168]->StrafeDiagonal(-8.0f, 8.0f, 87.0f, 87.0f, 240.0f, 256.0f, 0.5f * pTimer->DeltaTime(), m_pPlayerNew);
	m_pStairList[169]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[170]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[171]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[172]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[173]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[174]->Strafe(-48.0f, -16.0f, 10.0f * pTimer->DeltaTime(), true, m_pPlayerNew);
	m_pStairList[175]->Strafe(8.0f, -8.0f, 5.0f * pTimer->DeltaTime(), false, m_pPlayerNew);
	m_pStairList[176]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[178]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[179]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[180]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[181]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[183]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[187]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[188]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[189]->Fall(-15.0f, m_pPlayerNew);
	m_pStairList[190]->Fall(-15.0f, m_pPlayerNew);
	//////////////////////////////////////////////////////////////////////////

	CameraMove(pCamera, pTimer->DeltaTime());

	m_pClearStage->Update(pCamera);

	/// 게임 클리어 조건
	if (m_pPlayerNew->m_pPlayerList[0]->GetBoxCollider()->CheckCollided(m_pClearStage))
	{
		SoundManager::GetInstance()->Play(2, "GameClear");

		m_pUI[4]->SetBitmapPosition(52, 826);	// 임시(완료대사)
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

void MainInGameScene::Render(DXRenderer* pRenderer)
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
	m_pUI[4]->Render();

	if (m_TimeImageOn == true)
	{
		m_pUI[5]->Render();
		m_pUI[6]->Render();
	}

	//SimpleMath::Vector4 green(0.0f, 1.0f, 0.0f, 1.0f);
	//SimpleMath::Vector4 black(0.0f, 0.0f, 0.0f, 1.0f);
	//
	//pRenderer->DrawText(30, 20, black, (TCHAR*)L"");
	//
	//pRenderer->DrawText(30, 50, black, (TCHAR*)L"MoveState : %s", (TCHAR*)m_pPlayerNew->GetMoveState_Debug().c_str());
	//pRenderer->DrawText(30, 70, black, (TCHAR*)L"GameState : %s", (TCHAR*)m_pPlayerNew->GetGameState_Debug().c_str());
	//pRenderer->DrawText(30, 90, black, (TCHAR*)L"Direction : %s", (TCHAR*)m_pPlayerNew->GetDirection_Debug().c_str());
	//
	//pRenderer->DrawText(60, 130, black, (TCHAR*)L"Charater Pos X : %f", m_pPlayerNew->GetPosition().x);
	//pRenderer->DrawText(60, 150, black, (TCHAR*)L"Charater Pos Y : %f", m_pPlayerNew->GetPosition().y);
	//pRenderer->DrawText(60, 170, black, (TCHAR*)L"Charater Pos Z : %f", m_pPlayerNew->GetPosition().z);
}

void MainInGameScene::Finalize()
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

void MainInGameScene::ChangeSceneState(SceneManager* pSceneManager)
{
	// 씬 전환
	// 게임을 클리어하면 다음 씬으로 이동
	if (m_GameClear)
	{
		SoundManager::GetInstance()->Stop(2, "GameClear");
		SoundManager::GetInstance()->Pause(3, "BGM_InGame");
		pSceneManager->ChangeScene(eSceneState::MAIN_END_CUT);
	}

	// 타임아웃 되서 다시 타이틀로
	if (m_TimeOut)
	{
		SoundManager::GetInstance()->Stop(2, "GameClear");
		SoundManager::GetInstance()->Pause(3, "BGM_InGame");
		pSceneManager->ChangeScene(eSceneState::TITLE);
	}
}

void MainInGameScene::CameraMove(D3Camera* pCamera, float dTime)
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
		m_DelayCamPos += _distVec * dTime * 2.0f;

		pCamera->LookAt(m_DelayCamPos, target, XMFLOAT3(0, 1.0f, 0));
	}
}

void MainInGameScene::AddSavePoint(std::string name, Vector3 pos)
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
bool MainInGameScene::GetCollisionPos(Vector3* position)
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

void MainInGameScene::CheckPlayerPos()
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

BoxCollider* MainInGameScene::GetFootStepStair()
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