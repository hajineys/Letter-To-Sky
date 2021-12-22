#include <windowsx.h>
#include <sstream>
#include "framework.h"
#include "GameApp.h"

// 게임 타이머
#include "GameTimer.h"

// DirectX
#include "DXDevice.h"
#include "DXFactory.h"
#include "DXRenderer.h"
#include "IDXObjectBase.h"

// Manager
#include "ResourceManager.h"
#include "SceneManager.h"
#include "SoundManager.h"

#define MAX_LOADSTRING 100

namespace
{
	// This is just used to forward Windows messages from a global window
	// procedure to our member function window procedure because we cannot
	// assign a member function to WNDCLASS::lpfnWndProc.
	GameApp* pGameApp = 0;
}

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// Forward hwnd on because we can get messages (e.g., WM_CREATE)
	// before CreateWindow returns, and thus before mhMainWnd is valid.
	return pGameApp->MsgProc(hwnd, msg, wParam, lParam);
}

GameApp::GameApp()
	:m_hMainWnd(nullptr),
	m_MainWndCaption(L"하늘에 보내는 편지"),
	m_ClientWidth(1920), m_ClientHeight(1080),
	m_Msg{ 0 },
	m_AppPaused(false),
	m_Minimized(false),
	m_Maximized(false),
	m_Resizing(false),

	m_pTimer(nullptr),
	m_pDXDevice(nullptr),
	m_pDXFactory(nullptr),
	m_pDXRenderer(nullptr),
	m_pSceneManager(nullptr),
	m_pResourceManager(nullptr)
{
	pGameApp = this;
}

GameApp::~GameApp()
{

}

bool GameApp::Initialize(HINSTANCE hInstance)
{
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = MainWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = L"Letter To Sky";

	if (!RegisterClass(&wc))
	{
		MessageBox(0, L"RegisterClass Failed.", 0, 0);
		return false;
	}

	// 요청된 클라이언트 영역 치수를 기반으로 창 사각형 치수를 계산
	RECT R = { 0, 0, m_ClientWidth, m_ClientHeight };
	AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
	int width = R.right - R.left;
	int height = R.bottom - R.top;

	m_hMainWnd = CreateWindow(L"Letter To Sky", m_MainWndCaption.c_str(),
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX, 0, 0, width, height, 0, 0, hInstance, 0);

	if (!m_hMainWnd)
	{
		MessageBox(0, L"CreateWindow Failed.", 0, 0);
		return false;
	}

	ShowWindow(m_hMainWnd, SW_SHOW);
	UpdateWindow(m_hMainWnd);

	// DirectX 관련 Class 생성 및 초기화
	CreateAndInitGraphicsElementClass();

	// SoundManager 생성
	SoundManager::GetInstance()->Initialize(16);

	// 사운드 로드
	SoundManager::GetInstance()->LoadSoundFile("../Data/Sounds");

	// Game SceneManager 생성 및 초기화
	CreateAndInitSceneManager();

	return true;
}

int GameApp::Run()
{
	m_Msg = { 0 };

	// 게임 타이머(경과 시간과 게임 전체 시간을 측정하는데 쓰인다)
	m_pTimer = new GameTimer;
	m_pTimer->Reset();

	while (m_Msg.message != WM_QUIT)
	{
		// Windows 메시지가 있는 경우 처리
		if (PeekMessage(&m_Msg, 0, 0, 0, PM_REMOVE))
		{
			if (m_Msg.message == WM_QUIT) break;

			if (GetAsyncKeyState(VK_ESCAPE))
			{
				m_Msg.message = WM_DESTROY;
			}

			TranslateMessage(&m_Msg);
			DispatchMessage(&m_Msg);
		}
		// 그렇지 않으면 애니메이션/게임 관련 작업
		else
		{
			m_pTimer->Tick();

			if (!m_AppPaused)
			{
				CalculateFrameStats();
				UpdateAll(m_pTimer);
				RenderAll();
			}
			else
			{
				Sleep(100);
			}
		}
	}

	return (int)m_Msg.wParam;
}

void GameApp::UpdateAll(GameTimer* pTimer)
{
	m_pDXRenderer->Update(pTimer);
	m_pSceneManager->Update(pTimer, m_pDXRenderer->GetCamera());
}

void GameApp::RenderAll()
{
	m_pDXRenderer->BeginRender();
	m_pDXRenderer->Render();
	m_pSceneManager->Render(m_pDXRenderer);
	m_pDXRenderer->EndRender();
}

void GameApp::Finalize()
{
	// SceneManager 메모리 해제
	m_pSceneManager->Finalize();
	delete m_pSceneManager;
	m_pSceneManager = nullptr;

	// 그래픽스 엔진 메모리 해제
	delete m_pDXRenderer;
	m_pDXRenderer = nullptr;

	// DirectX Object Factory 메모리 해제
	delete m_pDXFactory;
	m_pDXFactory = nullptr;

	delete m_pDXDevice;
	m_pDXDevice = nullptr;

	// 게임 타이머 메모리 해제
	delete m_pTimer;
	m_pTimer = nullptr;
}

void GameApp::CreateAndInitGraphicsElementClass()
{
	// DirectX Device 생성 및 초기화
	m_pDXDevice = new DXDevice(m_hMainWnd, m_ClientWidth, m_ClientHeight);
	m_pDXDevice->Initialize();

	// DirectX Object Factory 생성
	m_pDXFactory = new DXFactory(m_pDXDevice);

	// 그래픽스 엔진 생성 및 초기화
	m_pDXRenderer = new DXRenderer(m_pDXDevice);
	m_pDXRenderer->Initialize(m_ClientWidth, m_ClientHeight, m_hMainWnd);
}

void GameApp::CreateAndInitSceneManager()
{
	m_pResourceManager = new ResourceManager(m_pDXDevice, m_pDXFactory);

	// SceneManager 생성
	m_pSceneManager = new SceneManager();
	m_pSceneManager->Initialize(m_hMainWnd, m_pDXDevice, m_pDXFactory, m_pResourceManager);

	// 씬 매니저의 상태를 NONE -> TITLE로 바꿈
	m_pSceneManager->ChangeScene(eSceneState::LOADING);
}

void GameApp::CalculateFrameStats()
{
	// Code computes the average frames per second, and also the 
	// average time it takes to render one frame, These stats 
	// are appended to the window caption bar
	static int frameCnt = 0;
	static float timeElapsed = 0.0f;

	frameCnt++;

	// Compute averages over one second period
	if ((m_pTimer->TotalTime() - timeElapsed) >= 1.0f)
	{
		float fps = (float)frameCnt; // fps = frameCnt / 1
		float mspf = 1000.0f / fps;

		std::wostringstream outs;
		outs.precision(6);
		outs << m_MainWndCaption << L"   "
			<< L"FPS:" << fps << L"   "
			<< L"Frame Time:" << mspf << L"(ms)";
		SetWindowText(m_hMainWnd, outs.str().c_str());

		// Reset for next average
		frameCnt = 0;
		timeElapsed += 1.0f;
	}
}

LRESULT GameApp::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		// WM_ACTIVATE is sent when the window is activated or deactivated.
		// We pause the game when the window is deactivated and unpause it
		// when it becomes active
		case WM_ACTIVATE:
			if (m_pTimer != nullptr)
			{
				if (LOWORD(wParam) == WA_INACTIVE)
				{
					m_AppPaused = true;
					m_pTimer->Stop();
				}
				else
				{
					m_AppPaused = false;
					m_pTimer->Start();
				}
			}
			return 0;

			// WM_SIZE is sent when the user resizes the window
		case WM_SIZE:
			// Save the new client area dimensions
			m_ClientWidth = LOWORD(lParam);
			m_ClientHeight = HIWORD(lParam);

			if (m_pDXDevice == nullptr) break;

			// 변경된 넓이와 높이 값을 엔진에도 똑같이 적용해야한다
			m_pDXDevice->SetClientWidth(m_ClientWidth);
			m_pDXDevice->SetClientHeight(m_ClientHeight);

			if (m_pDXDevice->GetD3dDevice())
			{
				if (wParam == SIZE_MINIMIZED)
				{
					m_AppPaused = true;
					m_Minimized = true;
					m_Maximized = false;
				}
				else if (wParam == SIZE_MAXIMIZED)
				{
					m_AppPaused = false;
					m_Minimized = false;
					m_Maximized = true;
					m_pDXDevice->OnResize();
				}
				else if (wParam == SIZE_RESTORED)
				{

					// Restoring from minimized state?
					if (m_Minimized)
					{
						m_AppPaused = false;
						m_Minimized = false;
						m_pDXDevice->OnResize();
					}

					// Restoring from maximized state?
					else if (m_Maximized)
					{
						m_AppPaused = false;
						m_Maximized = false;
						m_pDXDevice->OnResize();
					}
					else if (m_Resizing)
					{
						// If user is dragging the resize bars, we do not resize
						// the buffers here because as the user continuously
						// drags the resize bars, a stream of WM_SIZE messages are
						// sent to the window, and it would be pointless (and slow)
						// to resize for each WM_SIZE message received from dragging
						// the resize bars, So instead, we reset after the user is
						// done resizing the window and releases the resize bars, which
						// sends a WM_EXITSIZEMOVE message
					}
					else // API call such as SetWindowPos or mSwapChain->SetFullscreenState
					{
						m_pDXDevice->OnResize();
					}
				}
			}
			return 0;

			/*
			// WM_EXITSIZEMOVE is sent when the user grabs the resize bars
		case WM_ENTERSIZEMOVE:
			m_AppPaused = true;
			m_Resizing = true;
			m_pTimer->Stop();
			return 0;

			// WM_EXITSIZEMOVE is sent when the user releases the resize bars
			// Here we reset everything based on the new window dimensions
		case WM_EXITSIZEMOVE:
			m_AppPaused = false;
			m_Resizing = false;
			m_pTimer->Start();
			m_pDXDevice->OnResize();
			return 0;*/

			// The WM_MENUCHAR message is sent when a menu is active and the user presses
			// a key that does not correspond to any mnemonic or accelerator key
		case WM_MENUCHAR:
			// Don't beep when we alt-enter.
			return MAKELRESULT(0, MNC_CLOSE);

			// Catch this message so to prevent the window from becoming too small
		case WM_GETMINMAXINFO:
			((MINMAXINFO*)lParam)->ptMinTrackSize.x = 200;
			((MINMAXINFO*)lParam)->ptMinTrackSize.y = 200;
			return 0;

		case WM_LBUTTONDOWN:
			m_pSceneManager->OnMouseDown(wParam);
		case WM_MBUTTONDOWN:
		case WM_RBUTTONDOWN:
			m_pDXDevice->OnMouseDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			return 0;
		case WM_LBUTTONUP:
			m_pSceneManager->OnMouseUp(wParam);
		case WM_MBUTTONUP:
		case WM_RBUTTONUP:
			m_pDXDevice->OnMouseUp(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			return 0;
		case WM_MOUSEMOVE:
			m_pDXDevice->OnMouseMove(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			return 0;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}