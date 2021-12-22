#pragma once
#include <windows.h>
#include "DirectXDefine.h"
#include "LightHelper.h"

class GameTimer;
class D3Camera;
class DXTKFont;

// Object
class IDXObject;
class LightingHillAndWave;
class LightingSkull;
class CASEParser;
class ASEObject;

/// <summary>
/// Direct3D 11 Graphics Engine
/// 
/// ��¼�� �׷��Ƚ� ������ �ɻ��� �б����� �Ҽ��� �׷��Ƚ� ����
/// 2021. 05. 29 Hacgeum
/// </summary>
class HGEngine
{
public:
	HGEngine(HWND hWnd, int width, int height);
	~HGEngine();

	bool Initialize();
	void Update(GameTimer* pTimer);
	void BeginRender();
	void Render();
	void EndRender();

	// â ũ�Ⱑ ������ �� �ٽ� ó��
	void OnResize();

	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);

	// Getter / Setter
	ID3D11Device* GetD3dDevice() const { return m_pD3dDevice; }

	void SetClientWidth(int val) { m_ClientWidth = val; }
	void SetClientHeight(int val) { m_ClientHeight = val; }

private:
	void CreateRenderStates();
	float AspectRatio();

	// DXGI 1.1 ����
	HRESULT AdapterInfo();

	void ShowStatus();

	// Create Object
	void CreateHelperObject();
	void CreateObject();

private:
	HWND m_hWnd;
	int m_ClientWidth;
	int m_ClientHeight;

	// Direct3D 11 ���� ���
private:
	ID3D11Device* m_pD3dDevice;
	ID3D11DeviceContext* m_pD3dImmediateContext;
	IDXGISwapChain* m_pSwapChain;
	ID3D11Texture2D* m_pDepthStencilBuffer;
	ID3D11RenderTargetView* m_pRenderTargetView;
	ID3D11DepthStencilView* m_pDepthStencilView;
	D3D11_VIEWPORT m_ScreenViewport;

	D3D_DRIVER_TYPE m_D3dDriverType;

	UINT m_4xMsaaQuality;		// 4X MSAA�� ǰ�� ����
	bool m_Enable4xMsaa;

	// �ʱ�ȭ ���� ����ü��
	D3D_FEATURE_LEVEL m_FeatureLevel;

	// ���� ���� ��ü���� ����
	ID3D11RasterizerState* m_pWireframeRS;
	ID3D11RasterizerState* m_pSolidRS;
	ID3D11DepthStencilState* m_pNormalDSS;

	DXGI_ADAPTER_DESC1 m_AdapterDesc1;

private:
	// GameTimer
	GameTimer* m_pTimer;

	// Camera
	D3Camera* m_pCamera;
	POINT m_LastMousePos;

	// Font
	DXTKFont* m_pFont;

	// Helper Object
	static bool m_DebugShowFlag;
	IDXObject* m_pHelperObject[2];

	// Object
	int m_ObjectIndex;
	IDXObject* m_pObject[15];
	
	// ASE Parser
	CASEParser* m_pASEParser;
};
