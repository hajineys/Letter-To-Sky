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
/// ���ø����̼� Ŭ����
/// 
/// App ����, ����, ���� ���
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

	bool m_AppPaused;			// ���� ���α׷��� �Ͻ� ������ �����ΰ�?
	bool m_Minimized;			// ���� ���α׷��� �ּ�ȭ�� �����ΰ�?
	bool m_Maximized;			// ���� ���α׷��� �ִ�ȭ�� �����ΰ�?
	bool m_Resizing;			// ����ڰ� ũ�� ������ �׵θ��� ���� �ִ� �����ΰ�?

	// ���� Ÿ�̸�(��� �ð��� ���� ��ü �ð��� �����ϴµ� ���δ�)
	GameTimer* m_pTimer;

	// DirectX
	DXDevice* m_pDXDevice;
	DXFactory* m_pDXFactory;
	DXRenderer* m_pDXRenderer;

	// Manager
	ResourceManager* m_pResourceManager;
	SceneManager* m_pSceneManager;
};