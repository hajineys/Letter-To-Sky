#pragma once
#include <windows.h>

class DXDevice;
class DXFactory;
class ResourceManager;
class GameTimer;
class D3Camera;
class DXRenderer;
class IScene;

/// <summary>
/// 게임 씬 상태
/// 
/// 2021. 07. 21 Hacgeum
/// </summary>
enum class eSceneState
{
	LOADING,
	TITLE,
	INTRO_CUT,
	AREA_TUTO_CHOICE,
	TUTORIALINGAME,
	TUTO_END_CUT,
	AREA_MAIN_CHOICE,
	MAININGAME,
	MAIN_END_CUT,
	THANKSPLAY,
	NONE
};

/// <summary>
/// 씬들을 관리하는 매니저 클래스
/// 
/// 2021. 07. 21 Hacgeum
/// </summary>
class SceneManager
{
public:
	SceneManager();
	~SceneManager();

	void Initialize(HWND hWnd, DXDevice* pDevice, DXFactory* pFactory, ResourceManager* pResourceManager);
	void Update(GameTimer* pTimer, D3Camera* pCamera);
	void Render(DXRenderer* pRenderer);
	void Finalize();

	void OnMouseDown(WPARAM btnState);
	void OnMouseUp(WPARAM btnState);

	void ChangeScene(eSceneState nextScene);

public:
	static eSceneState m_NowSceneState;
	static IScene* m_pNowScene;

	HWND m_hWnd;
	DXDevice* m_pDevice;
	DXFactory* m_pFactory;
	ResourceManager* m_pResourceManager;

	bool m_IsMouseDown;
	bool m_IsMouseUp;
};