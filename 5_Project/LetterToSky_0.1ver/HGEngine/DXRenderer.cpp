#include "DXRenderer.h"
#include "DXDevice.h"
#include "DXRenderStates.h"
#include "GameTimer.h"
#include "D3Camera.h"
#include "DXTKFont.h"					// Font
#include "Effects.h"
#include "Vertex.h"
#include "../DXTK/Inc/SimpleMath.h"
#include "GeometryGenerator.h"

// Effect
#include "BlurFilter.h"
#include "Sky.h"

bool DXRenderer::m_DebugShowFlag = false;
bool DXRenderer::m_BlurFlag = false;

DXRenderer::DXRenderer(DXDevice* pDirectX)
	:m_pDirectX(pDirectX),
	m_pTimer(nullptr),
	m_pCamera(),

	m_pFont(nullptr),
	renderTargets(),
	m_pBlur(nullptr),
	m_Sky(nullptr)
{
	m_pCamera = m_pDirectX->GetD3Camera();

	// 폰트 생성
	m_pFont = new DXTKFont;
	m_pFont->Create(m_pDirectX->GetD3dDevice(), DXRenderStates::m_pSolidRS, DXRenderStates::m_pNormalDSS);

	m_pBlur = m_pDirectX->GetBlur();

	m_Sky = new Sky(m_pDirectX->GetD3dDevice(), L"../Data/Textures/StandardCubeMap_SKY5.dds", 5000.0f);
}

DXRenderer::~DXRenderer()
{
	m_pDirectX->GetD3dImmediateContext()->ClearState();

	SafeDelete(m_pFont);
	SafeDelete(m_Sky);

	Effects::DestroyAll();
	InputLayouts::DestroyAll();
	DXRenderStates::DestroyAll();
}

void DXRenderer::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	Effects::InitAll(m_pDirectX->GetD3dDevice());
	InputLayouts::InitAll(m_pDirectX->GetD3dDevice());
	DXRenderStates::InitAll(m_pDirectX->GetD3dDevice());

	m_pDirectX->BuildScreenQuadGeometryBuffers();
	m_pDirectX->BuildOffscreenViews();
}

void DXRenderer::Update(GameTimer* pTimer)
{
	m_pTimer = pTimer;

	if (GetAsyncKeyState('P') & 0x0001)
	{
		// DebugCam
		m_CameraIndex = 0;
	}
	else if (GetAsyncKeyState('O') & 0x0001)
	{
		// ObjectTrackingCam
		m_CameraIndex = 1;
	}

	float cameraValue = 10.0f;
	float distance = cameraValue * m_pTimer->DeltaTime();

	// Shift 누르면 카메라 이동속도 3배로 증가
	if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
	{
		distance *= 3;
	}

	// 카메라 이동 키
	if (GetAsyncKeyState('W') & 0x8000)
		m_pCamera[m_CameraIndex]->Walk(distance);

	if (GetAsyncKeyState('S') & 0x8000)
		m_pCamera[m_CameraIndex]->Walk(-distance);

	if (GetAsyncKeyState('A') & 0x8000)
		m_pCamera[0]->Strafe(-distance);

	if (GetAsyncKeyState('D') & 0x8000)
		m_pCamera[0]->Strafe(distance);

	if (GetAsyncKeyState('Q') & 0x8000)
		m_pCamera[m_CameraIndex]->WorldUpDown(-distance);

	if (GetAsyncKeyState('E') & 0x8000)
		m_pCamera[m_CameraIndex]->WorldUpDown(distance);

	// View Matrix 생성
	m_pCamera[m_CameraIndex]->UpdateViewMatrix();

	// Debug On/Off
	if (GetAsyncKeyState(VK_F5) & 0x0001)
	{
		m_DebugShowFlag ^= true;
	}

	// Blur On/Off
	if (GetAsyncKeyState('B') & 0x0001)
	{
		m_BlurFlag ^= true;
	}
}

void DXRenderer::BeginRender()
{
	assert(m_pDirectX->GetD3dImmediateContext());

	if (m_BlurFlag)
	{
		// Render to our offscreen texture.  Note that we can use the same depth/stencil buffer
		// we normally use since our offscreen texture matches the dimensions
		ID3D11RenderTargetView* renderTargets[1] = { m_pDirectX->GetOffscreenRTV() };
		m_pDirectX->GetD3dImmediateContext()->OMSetRenderTargets(1, renderTargets, m_pDirectX->GetDepthStencilView());

		m_pDirectX->GetD3dImmediateContext()->ClearRenderTargetView(m_pDirectX->GetOffscreenRTV(), reinterpret_cast<const float*>(&Colors::DarkSlateBlue));
		m_pDirectX->GetD3dImmediateContext()->ClearDepthStencilView(m_pDirectX->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}
	else
	{
		// 후면 버퍼를 특정색으로 지운다
		m_pDirectX->GetD3dImmediateContext()->ClearRenderTargetView(m_pDirectX->GetRenderTargetView(),
			reinterpret_cast<const float*>(&DirectX::Colors::DarkSlateBlue));

		// 깊이 버퍼를 1.0f로, 스텐실 버퍼를 0으로 지운다
		m_pDirectX->GetD3dImmediateContext()->ClearDepthStencilView(m_pDirectX->GetDepthStencilView(),
			D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}

	m_pDirectX->GetD3dImmediateContext()->OMSetDepthStencilState(DXRenderStates::m_pNormalDSS, 0);
}

void DXRenderer::Render()
{
	//m_pDirectX->GetD3dImmediateContext()->OMSetDepthStencilState(DXRenderStates::m_pNormalDSS, 0);

	Effects::BasicFX->SetCubeMap(m_Sky->CubeMapSRV());

	m_Sky->Draw(m_pDirectX->GetD3dImmediateContext(), *m_pCamera[m_CameraIndex]);

	// restore default states, as the SkyFX changes them in the effect file.
	//m_pDirectX->GetD3dImmediateContext()->RSSetState(0);
	//m_pDirectX->GetD3dImmediateContext()->OMSetDepthStencilState(0, 0);

	if (m_DebugShowFlag)
	{
		ShowStatus();
	}

	// Restore default
	//m_pDirectX->GetD3dImmediateContext()->RSSetState(0);
}

void DXRenderer::EndRender()
{
	// Restore default
	m_pDirectX->GetD3dImmediateContext()->RSSetState(0);

	assert(m_pDirectX->GetSwapChain());

	if (m_BlurFlag)
	{
		// Restore the back buffer, The offscreen render target will serve as an input into
		// the compute shader for blurring, so we must unbind it from the OM stage before we
		// can use it as an input into the compute shader
		renderTargets[0] = m_pDirectX->GetRenderTargetView();
		m_pDirectX->GetD3dImmediateContext()->OMSetRenderTargets(1, renderTargets, m_pDirectX->GetDepthStencilView());

		//mBlur.SetGaussianWeights(4.0f);
		m_pBlur->BlurInPlace(m_pDirectX->GetD3dImmediateContext(), m_pDirectX->GetOffscreenSRV(), m_pDirectX->GetOffscreenUAV(), 4);

		// 후면 버퍼를 특정색으로 지운다
		m_pDirectX->GetD3dImmediateContext()->ClearRenderTargetView(m_pDirectX->GetRenderTargetView(),
			reinterpret_cast<const float*>(&DirectX::Colors::DarkSlateBlue));

		// 깊이 버퍼를 1.0f로, 스텐실 버퍼를 0으로 지운다
		m_pDirectX->GetD3dImmediateContext()->ClearDepthStencilView(m_pDirectX->GetDepthStencilView(),
			D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		m_pDirectX->DrawScreenQuad();
	}

	// 후면 버퍼를 화면에 제시한다
	HR(m_pDirectX->GetSwapChain()->Present(0, 0));
}

void DXRenderer::DrawText(int x, int y, DirectX::XMFLOAT4 color, TCHAR* text, ...)
{
	TCHAR _buffer[1024] = L"";
	va_list vl;
	va_start(vl, text);
	_vstprintf(_buffer, 1024, text, vl);
	va_end(vl);

	m_pFont->DrawTextColor2(x, y, color, _buffer);
}

D3Camera* DXRenderer::GetCamera()
{
	return m_pCamera[m_CameraIndex];
}

void DXRenderer::ShowStatus()
{
	SimpleMath::Vector4 white(1.0f, 1.0f, 1.0f, 1.0f);
	SimpleMath::Vector4 red(1.0f, 0.0f, 0.5f, 1.0f);
	SimpleMath::Vector4 blue(0.0f, 0.0f, 1.0f, 1.0f);
	SimpleMath::Vector4 yellow(1.0f, 1.0f, 0.0f, 1.0f);
	SimpleMath::Vector4 customColor(0.5f, 1.0f, 5.0f, 1.0f);

	// 갱신주기
	static float addedTime = 0;
	static float fps = 0;
	static float deltaTimeMS = 0;

	// 갱신주기는 0.2초
	if (0.2f < addedTime)
	{
		fps = (1.0f / m_pTimer->DeltaTime());
		deltaTimeMS = m_pTimer->DeltaTime() * 1000.0f;

		addedTime = 0;
	}

	addedTime += (m_pTimer->DeltaTime());

	// FPS, deltaTime을 그린다.
	m_pFont->DrawTextColor(1, 4, white, (TCHAR*)L"FPS : %.2f", fps);
	m_pFont->DrawTextColor(1, 19, white, (TCHAR*)L"DeltaTime : %.4f(ms)", deltaTimeMS);

	m_pFont->DrawTextColor(200, 100, yellow, (TCHAR*)L"한글과 버퍼가 잘 작동하는가? %d / %f / %s", 79, 3.14f, L"노란색글씨 똠방각하 펲시 콜라 뾸뾸");

	int _yPos = 50;

	// 피쳐레벨
	m_pFont->DrawTextColor(1, _yPos, white, (TCHAR*)L"Feature Level : %x", m_pDirectX->GetFeatureLevel());

	// 어댑터 정보
	m_pFont->DrawTextColor(1, _yPos += 15, white, (TCHAR*)L"Description : %s", m_pDirectX->GetAdapterDesc().Description);
	m_pFont->DrawTextColor(1, _yPos += 15, white, (TCHAR*)L"VendorID : %u", m_pDirectX->GetAdapterDesc().VendorId);
	m_pFont->DrawTextColor(1, _yPos += 15, white, (TCHAR*)L"DeviceID : %u", m_pDirectX->GetAdapterDesc().DeviceId);
	m_pFont->DrawTextColor(1, _yPos += 15, white, (TCHAR*)L"SubSysID : %u", m_pDirectX->GetAdapterDesc().SubSysId);
	m_pFont->DrawTextColor(1, _yPos += 15, white, (TCHAR*)L"Revision : %u", m_pDirectX->GetAdapterDesc().Revision);
	m_pFont->DrawTextColor(1, _yPos += 15, white, (TCHAR*)L"VideoMemory : %lu MB", m_pDirectX->GetAdapterDesc().DedicatedVideoMemory / 1024 / 1024);
	m_pFont->DrawTextColor(1, _yPos += 15, white, (TCHAR*)L"SystemMemory : %lu MB", m_pDirectX->GetAdapterDesc().DedicatedSystemMemory / 1024 / 1024);
	m_pFont->DrawTextColor(1, _yPos += 15, white, (TCHAR*)L"SharedSysMemory : %lu MB", m_pDirectX->GetAdapterDesc().SharedSystemMemory / 1024 / 1024);
	m_pFont->DrawTextColor(1, _yPos += 15, white, (TCHAR*)L"AdpaterLuid : %u.%d", m_pDirectX->GetAdapterDesc().AdapterLuid.HighPart, m_pDirectX->GetAdapterDesc().AdapterLuid.LowPart);

	// Flag
	m_pFont->DrawTextColor(1, _yPos += 30, red, (TCHAR*)L"Debug Show On / Off : F5");
	m_pFont->DrawTextColor(1, _yPos += 15, yellow, (TCHAR*)L"Blur Show On / Off : B");

	// 카메라 정보
	m_pFont->DrawTextColor(1, _yPos += 30, customColor, (TCHAR*)L"Camera Pos : %.2f / %.2f / %.2f", m_pCamera[m_CameraIndex]->GetPosition().x, m_pCamera[m_CameraIndex]->GetPosition().y, m_pCamera[m_CameraIndex]->GetPosition().z);
	m_pFont->DrawTextColor(1, _yPos += 15, customColor, (TCHAR*)L"Camera Rot : %.2f / %.2f / %.2f", m_pCamera[m_CameraIndex]->GetRight().x, m_pCamera[m_CameraIndex]->GetRight().y, m_pCamera[m_CameraIndex]->GetRight().z);
	m_pFont->DrawTextColor(1, _yPos += 30, customColor, (TCHAR*)L"Camera 이동속도 증가 : Shift");

	if (m_CameraIndex == 0)
	{
		m_pFont->DrawTextColor(1, _yPos += 15, customColor, (TCHAR*)L"현재 Camera : DebugCam");
	}
	else if (m_CameraIndex == 1)
	{
		m_pFont->DrawTextColor(1, _yPos += 15, customColor, (TCHAR*)L"현재 Camera : ObjectTrackingCam");
	}
}