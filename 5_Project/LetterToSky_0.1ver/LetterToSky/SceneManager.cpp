#include "SceneManager.h"
#include "IScene.h"

// DirectX
#include "DXRenderer.h"

// SceneList
#include "LoadingScene.h"
#include "TitleScene.h"
#include "IntroCutScene.h"
#include "AreaTutoChoiceScene.h"
#include "TutorialInGameScene.h"
#include "TutoEndCutScene.h"
#include "AreaMainChoiceScene.h"
#include "MainInGameScene.h"
#include "MainEndCutScene.h"
#include "ThanksPlayScene.h"

// static 멤버 변수 초기화
eSceneState SceneManager::m_NowSceneState = eSceneState::NONE;		// 게임 시작 할때 state는 NONE으로 설정
IScene* SceneManager::m_pNowScene = nullptr;

SceneManager::SceneManager()
	: m_hWnd(nullptr),
	m_pDevice(nullptr),
	m_pFactory(nullptr),
	m_pResourceManager(nullptr),
	m_IsMouseDown(false),
	m_IsMouseUp(false)
{

}

SceneManager::~SceneManager()
{

}

void SceneManager::Initialize(HWND hWnd, DXDevice* pDevice, DXFactory* pFactory, ResourceManager* pResourceManager)
{
	m_hWnd = hWnd;
	m_pDevice = pDevice;
	m_pFactory = pFactory;
	m_pResourceManager = pResourceManager;
}

void SceneManager::Update(GameTimer* pTimer, D3Camera* pCamera)
{
	if (m_pNowScene)
	{
		m_pNowScene->Update(m_hWnd, this, pTimer, pCamera);
	}

	// Input 초기화
	m_IsMouseUp = false;
	m_IsMouseDown = false;
}

void SceneManager::Render(DXRenderer* pRenderer)
{
	if (m_pNowScene)
	{
		m_pNowScene->Render(pRenderer);
	}
}

void SceneManager::Finalize()
{
	if (m_pNowScene)
	{
		m_pNowScene->Finalize();
	}
}

void SceneManager::OnMouseDown(WPARAM btnState)
{
	m_IsMouseDown = true;

	SetCapture(m_hWnd);
}

void SceneManager::OnMouseUp(WPARAM btnState)
{
	m_IsMouseUp = true; // 이거 어케가져와 

	ReleaseCapture();
}

void SceneManager::ChangeScene(eSceneState nextScene)
{
	// 씬 전환
	if (m_NowSceneState != nextScene)
	{
		// 기존 씬 삭제(Safe Delete)
		if (m_pNowScene)
		{
			m_pNowScene->Finalize();
			delete m_pNowScene;
			m_pNowScene = nullptr;
		}

		// 씬 전환
		switch (nextScene)
		{
			case eSceneState::LOADING:
			{
				m_pNowScene = new LoadingScene();
				m_pNowScene->Initialize(m_pDevice, m_pFactory);
				m_NowSceneState = nextScene;
			}
			break;

			case eSceneState::TITLE:
			{
				m_pNowScene = new TitleScene();
				m_pNowScene->Initialize(m_pDevice, m_pFactory, m_pResourceManager);
				m_NowSceneState = nextScene;
			}
			break;

			case eSceneState::INTRO_CUT:
			{
				m_pNowScene = new IntroCutScene();
				m_pNowScene->Initialize(m_pDevice, m_pFactory);
				m_NowSceneState = nextScene;
			}
			break;

			case eSceneState::AREA_TUTO_CHOICE:
			{
				m_pNowScene = new AreaTutoChoiceScene();
				m_pNowScene->Initialize(m_pDevice, m_pFactory);
				m_NowSceneState = nextScene;
			}
			break;

			case eSceneState::TUTORIALINGAME:
			{
				m_pNowScene = new TutorialInGameScene();
				m_pNowScene->Initialize(m_pDevice, m_pFactory, m_pResourceManager);
				m_NowSceneState = nextScene;
			}
			break;

			case eSceneState::TUTO_END_CUT:
			{
				m_pNowScene = new TutoEndCutScene();
				m_pNowScene->Initialize(m_pDevice, m_pFactory);
				m_NowSceneState = nextScene;
			}
			break;

			case eSceneState::AREA_MAIN_CHOICE:
			{
				m_pNowScene = new AreaMainChoiceScene();
				m_pNowScene->Initialize(m_pDevice, m_pFactory);
				m_NowSceneState = nextScene;
			}
			break;

			case eSceneState::MAININGAME:
			{
				m_pNowScene = new MainInGameScene();
				m_pNowScene->Initialize(m_pDevice, m_pFactory, m_pResourceManager);
				m_NowSceneState = nextScene;
			}
			break;

			case eSceneState::MAIN_END_CUT:
			{
				m_pNowScene = new MainEndCutScene();
				m_pNowScene->Initialize(m_pDevice, m_pFactory);
				m_NowSceneState = nextScene;
			}
			break;

			case eSceneState::THANKSPLAY:
			{
				m_pNowScene = new ThanksPlayScene();
				m_pNowScene->Initialize(m_pDevice, m_pFactory);
				m_NowSceneState = nextScene;
			}
			break;

			case eSceneState::NONE:
			{
				// 아무것도 없음
			}
			break;
		}
	}
}