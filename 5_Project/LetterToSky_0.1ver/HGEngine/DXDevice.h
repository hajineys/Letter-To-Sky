#pragma once
#include "AlignedAllocationPolicy.h"
#include "DirectXDefine.h"
#include "BlurFilter.h"

class D3Camera;

/// <summary>
/// DirectX 초기화
/// 
/// 2021. 07. 15 Hacgeum
/// </summary>
class DXDevice : public AlignedAllocationPolicy<16>
{
public:
	DXDevice(HWND hWnd, int width, int height);
	~DXDevice();

	bool Initialize();

	// 창 크기가 변했을 때 다시 처리
	void OnResize();

	void BuildScreenQuadGeometryBuffers();
	void BuildOffscreenViews();
	void DrawScreenQuad();

	void TurnZBufferOn();
	void TurnZBufferOff();

	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);

	// Getter / Setter
	int GetClientWidth() const { return m_ClientWidth; }
	void SetClientWidth(int val) { m_ClientWidth = val; }
	int GetClientHeight() const { return m_ClientHeight; }
	void SetClientHeight(int val) { m_ClientHeight = val; }

	ID3D11Device* GetD3dDevice() const { return m_pD3dDevice; }
	ID3D11DeviceContext* GetD3dImmediateContext() const { return m_pD3dImmediateContext; }
	IDXGISwapChain* GetSwapChain() const { return m_pSwapChain; }
	ID3D11Texture2D* GetDepthStencilBuffer() const { return m_pDepthStencilBuffer; }
	ID3D11RenderTargetView* GetRenderTargetView() const { return m_pRenderTargetView; }
	ID3D11DepthStencilView* GetDepthStencilView() const { return m_pDepthStencilView; }

	D3D_FEATURE_LEVEL GetFeatureLevel() const { return m_FeatureLevel; }

	DXGI_ADAPTER_DESC1 GetAdapterDesc() const { return m_AdapterDesc1; }

	vector<D3Camera*> GetD3Camera() const { return m_pCamera; }

	ID3D11Buffer* GetScreenQuadVB() const { return mScreenQuadVB; }
	ID3D11Buffer* GetScreenQuadIB() const { return mScreenQuadIB; }

	ID3D11ShaderResourceView* GetOffscreenSRV() const { return mOffscreenSRV; }
	ID3D11UnorderedAccessView* GetOffscreenUAV() const { return mOffscreenUAV; }
	ID3D11RenderTargetView* GetOffscreenRTV() const { return mOffscreenRTV; }

	BlurFilter* GetBlur() const { return mBlur; }

	XMMATRIX GetOrthoMatrix() const { return m_OrthoMatrix; }

private:
	float AspectRatio();

	// DXGI 1.1 대응
	HRESULT AdapterInfo();

private:
	HWND m_hWnd;
	int m_ClientWidth;
	int m_ClientHeight;

	// Direct3D 11 관련 멤버
private:
	ID3D11Device* m_pD3dDevice;
	ID3D11DeviceContext* m_pD3dImmediateContext;
	IDXGISwapChain* m_pSwapChain;
	ID3D11Texture2D* m_pDepthStencilBuffer;
	ID3D11RenderTargetView* m_pRenderTargetView;
	ID3D11DepthStencilView* m_pDepthStencilView;
	D3D11_VIEWPORT m_ScreenViewport;

	D3D_DRIVER_TYPE m_D3dDriverType;

	UINT m_4xMsaaQuality;		// 4X MSAA의 품질 수준
	bool m_Enable4xMsaa;

	// 초기화 관련 구조체들
	D3D_FEATURE_LEVEL m_FeatureLevel;

	DXGI_ADAPTER_DESC1 m_AdapterDesc1;

private:
	// Camera
	D3Camera* m_pDebugCam;
	D3Camera* m_pObjectTrackingCam;
	vector<D3Camera*> m_pCamera;

	POINT m_LastMousePos;

	// Blur
private:
	ID3D11Buffer* mScreenQuadVB;
	ID3D11Buffer* mScreenQuadIB;

	ID3D11ShaderResourceView* mOffscreenSRV;
	ID3D11UnorderedAccessView* mOffscreenUAV;
	ID3D11RenderTargetView* mOffscreenRTV;

	BlurFilter* mBlur;

	// 2D Bitmap
private:
	ID3D11DepthStencilState* m_DepthStencilState;
	ID3D11DepthStencilState* m_DepthDisabledStencilState;
	ID3D11RasterizerState* m_RasterState;

	XMMATRIX m_OrthoMatrix;
};