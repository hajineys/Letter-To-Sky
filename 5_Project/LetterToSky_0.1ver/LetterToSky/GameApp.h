#pragma once
#include <windows.h>
#include <string>
using namespace std;

class GameTimer;

class DXDevice;
class DXFactory;
class DXRenderer;

class ResourceManager;
class SceneManager;

/// <summary>
/// 어플리케이션 클래스
/// 
/// App 생성, 실행, 종료 담당
/// 2021. 05. 29 Hacgeum
/// </summary>
class GameApp
{
public:
	GameApp();
	~GameApp();

	bool Initialize(HINSTANCE hInstance);
	int Run();
	void UpdateAll(GameTimer* pTimer);
	void RenderAll();
	void Finalize();

	void CreateAndInitGraphicsElementClass();
	void CreateAndInitSceneManager();

	void CalculateFrameStats();
	LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	HWND m_hMainWnd;
	wstring m_MainWndCaption;
	int m_ClientWidth;
	int m_ClientHeight;

	MSG m_Msg;

	bool m_AppPaused;			// 응용 프로그램이 일시 정지된 상태인가?
	bool m_Minimized;			// 응용 프로그램이 최소화된 상태인가?
	bool m_Maximized;			// 응용 프로그램이 최대화된 상태인가?
	bool m_Resizing;			// 사용자가 크기 조정용 테두리를 끌고 있는 상태인가?

	// 게임 타이머(경과 시간과 게임 전체 시간을 측정하는데 쓰인다)
	GameTimer* m_pTimer;

	// DirectX
	DXDevice* m_pDXDevice;
	DXFactory* m_pDXFactory;
	DXRenderer* m_pDXRenderer;

	// Manager
	ResourceManager* m_pResourceManager;
	SceneManager* m_pSceneManager;
};