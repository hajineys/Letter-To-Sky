#pragma once
#include <windows.h>
#include "LightHelper.h"

class DXDevice;
class DXFactory;
class GameTimer;
class D3Camera;
class DXTKFont;

// Effect
class BlurFilter;
class Sky;

// Box Collider
class BoxCollider;

/// <summary>
/// Direct3D 11 Graphics Engine
/// 
/// 개쩌는 그래픽스 엔진이 될뻔한 학금이의 소소한 그래픽스 엔진
/// 2021. 05. 29 Hacgeum
/// </summary>
class DXRenderer
{
public:
	DXRenderer(DXDevice* pDirectX);
	~DXRenderer();

	void Initialize(int screenWidth, int screenHeight, HWND hwnd);
	void Update(GameTimer* pTimer);
	void BeginRender();
	void Render();
	void EndRender();

	void DrawText(int x, int y, DirectX::XMFLOAT4 color, TCHAR* text, ...);

	// Getter / Setter
	D3Camera* GetCamera();

private:
	void ShowStatus();

private:
	// DirectX InitClass
	DXDevice* m_pDirectX;

	// GameTimer
	GameTimer* m_pTimer;

	// Camera
	vector<D3Camera*> m_pCamera;
	int m_CameraIndex;

	// Font
	DXTKFont* m_pFont;

	static bool m_DebugShowFlag;

	// Blur
	static bool m_BlurFlag;
	ID3D11RenderTargetView* renderTargets[2];

	BlurFilter* m_pBlur;

	Sky* m_Sky;

	// Box Collider
	BoxCollider* m_pBoxCollider01;
	BoxCollider* m_pBoxCollider02;
	bool CheckBoxCollided = false;
};