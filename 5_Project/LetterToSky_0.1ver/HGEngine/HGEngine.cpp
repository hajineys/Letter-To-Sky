#include "pch.h"
#include "HGEngine.h"
#include "../ProjectDemo/GameTimer.h"
#include "D3Camera.h"
#include "MathHelper.h"
#include "../DXTK/Inc/SimpleMath.h"
#include "DXTKFont.h"					// Font
#include "Effects.h"
#include "Vertex.h"

// Helper Object
#include "Grid.h"
#include "Axis.h"

// Color Object
#include "Box.h"
#include "Hills.h"
#include "Skull.h"
#include "Car.h"
#include "Waves.h"
#include "MikeWazowski.h"

// Texture Object
#include "TexWave.h"
#include "TexHill.h"
#include "TexBox.h"

// Lighting 예제
#include "LightingHillAndWave.h"
#include "LightingSkull.h"

// ASE Object
#include "ASEObject.h"

// ASE Parser
#include "ASEParser.h"

bool HGEngine::m_DebugShowFlag = true;

HGEngine::HGEngine(HWND hWnd, int width, int height)
	:m_hWnd(hWnd),
	m_ClientWidth(width),
	m_ClientHeight(height),

	m_pD3dDevice(nullptr),
	m_pD3dImmediateContext(nullptr),
	m_pSwapChain(nullptr),
	m_pDepthStencilBuffer(nullptr),
	m_pRenderTargetView(nullptr),
	m_pDepthStencilView(nullptr),
	m_D3dDriverType(D3D_DRIVER_TYPE_HARDWARE),
	m_4xMsaaQuality(0),
	m_Enable4xMsaa(false),

	m_pWireframeRS(0), m_pSolidRS(0), m_pNormalDSS(0),

	m_pTimer(nullptr),
	m_pCamera(nullptr),
	m_LastMousePos{ 0,0 },

	m_pFont(nullptr)
{
	ZeroMemory(&m_ScreenViewport, sizeof(D3D11_VIEWPORT));

	// Direc3D Camera 생성
	m_pCamera = new D3Camera;
	m_pCamera->LookAt(XMFLOAT3(8.0f, 8.0f, -8.0f), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 1.0f, 0));

	// ASE Parser 생성
	m_pASEParser = new CASEParser;
	m_pASEParser->Init();

	m_ObjectIndex = 14;
}

HGEngine::~HGEngine()
{
	Effects::DestroyAll();
	InputLayouts::DestroyAll();

	SafeDelete(m_pASEParser);

	for (int i = 0; i < 15; i++)
	{
		SafeDelete(m_pObject[i]);
	}

	for (int i = 0; i < 2; i++)
	{
		SafeDelete(m_pHelperObject[i]);
	}

	SafeDelete(m_pFont);
	SafeDelete(m_pCamera);

	ReleaseCOM(m_pNormalDSS);
	ReleaseCOM(m_pWireframeRS);
	ReleaseCOM(m_pSolidRS);

	ReleaseCOM(m_pRenderTargetView);
	ReleaseCOM(m_pDepthStencilView);
	ReleaseCOM(m_pSwapChain);
	ReleaseCOM(m_pDepthStencilBuffer);

	// 모든 기본 설정을 복원
	if (m_pD3dImmediateContext)
		m_pD3dImmediateContext->ClearState();

	ReleaseCOM(m_pD3dImmediateContext);
	ReleaseCOM(m_pD3dDevice);
}

bool HGEngine::Initialize()
{
	// D3DDevice와 D3DImmediateContext를 생성한다
	UINT createDeviceFlags = 0;

#if defined(DEBUG) || defined(_DEBUG)  
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	HRESULT hr = D3D11CreateDevice(
		0,							// 기본 어댑터
		m_D3dDriverType,
		0,							// 소프트웨어 장치를 사용하지 않음
		createDeviceFlags,
		0, 0,						// 기본 기능 수준 배열
		D3D11_SDK_VERSION,
		&m_pD3dDevice,
		&m_FeatureLevel,
		&m_pD3dImmediateContext);

	if (FAILED(hr))
	{
		MessageBox(0, L"D3D11CreateDevice Failed.", 0, 0);
		return false;
	}

	if (m_FeatureLevel != D3D_FEATURE_LEVEL_11_0)
	{
		MessageBox(0, L"Direct3D Feature Level 11 unsupported.", 0, 0);
		return false;
	}

	// 백 버퍼 형식에 대한 4X MSAA 품질 지원 확인
	// 모든 Direct3D 11 지원 장치는 모든 렌더 대상 형식에 대해 4X MSAA 지원
	// 따라서 우리는 품질 지원만 확인하면 된다
	HR(m_pD3dDevice->CheckMultisampleQualityLevels(
		DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_4xMsaaQuality));

	assert(m_4xMsaaQuality > 0);

	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = m_ClientWidth;
	sd.BufferDesc.Height = m_ClientHeight;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// 4X MSAA를 사용하는가? 
	if (m_Enable4xMsaa)
	{
		sd.SampleDesc.Count = 4;
		sd.SampleDesc.Quality = m_4xMsaaQuality - 1;
	}
	// MSAA를 사용하지 않음
	else
	{
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
	}

	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = m_hWnd;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	// Swap Chain을 올바르게 생성하려면 다음과 같이 해야한다
	// 디바이스를 생성하는 데 사용된 IDXGIFactory를 사용해야 한다
	// 다른 IDXGIFactory 인스턴스를 사용하려고 하면
	// 오류를 얻는다. "IDXGIFactory::CreateSwapChain : This function is being called with a device from a different IDXGIFactory"

	// D3디바이스로부터 dxgi디바이스를 얻어온다
	IDXGIDevice* dxgiDevice = 0;
	HR(m_pD3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice));

	// dxgi디바이스로부터 dxgi어탭터를 얻어온다
	IDXGIAdapter* dxgiAdapter = 0;
	HR(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter));

	// dxgi어탭터로부터 dxgi팩토리를 얻어온다
	IDXGIFactory* dxgiFactory = 0;
	HR(dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory));

	// Swap Chain 생성
	HR(dxgiFactory->CreateSwapChain(m_pD3dDevice, &sd, &m_pSwapChain));
	dxgiFactory->MakeWindowAssociation(m_hWnd, DXGI_MWA_NO_ALT_ENTER);

	ReleaseCOM(dxgiDevice);
	ReleaseCOM(dxgiAdapter);
	ReleaseCOM(dxgiFactory);

	// d3d 생성을 위해 수행해야 하는 나머지 단계도 창의 크기가 조정될 때 마다 실행해야 한다
	// 코드 중복을 방지하려면 OnResize 메소드를 여기로 호출해야한다
	OnResize();

	// Render State
	CreateRenderStates();

	// 폰트 생성
	m_pFont = new DXTKFont;
	m_pFont->Create(m_pD3dDevice, m_pSolidRS, m_pNormalDSS);

	// 어댑터의 정보를 얻는다
	AdapterInfo();

	Effects::InitAll(m_pD3dDevice);
	InputLayouts::InitAll(m_pD3dDevice);

	// Object
	CreateHelperObject();
	CreateObject();

	return true;
}

void HGEngine::Update(GameTimer* pTimer)
{
	m_pTimer = pTimer;

	float cameraValue = 10.0f;
	float distance = cameraValue * m_pTimer->DeltaTime();

	if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
	{
		distance *= 2;
	}

	// 카메라 이동 키
	if (GetAsyncKeyState('W') & 0x8000)
		m_pCamera->Walk(distance);

	if (GetAsyncKeyState('S') & 0x8000)
		m_pCamera->Walk(-distance);

	if (GetAsyncKeyState('A') & 0x8000)
		m_pCamera->Strafe(-distance);

	if (GetAsyncKeyState('D') & 0x8000)
		m_pCamera->Strafe(distance);

	if (GetAsyncKeyState('Q') & 0x8000)
		m_pCamera->WorldUpDown(-distance);

	if (GetAsyncKeyState('E') & 0x8000)
		m_pCamera->WorldUpDown(distance);

	// View Matrix 생성
	m_pCamera->UpdateViewMatrix();

	for (int i = 0; i < 2; i++)
	{
		m_pHelperObject[i]->Update(m_pCamera->View(), m_pCamera->Proj(), nullptr, m_pCamera);
	}

	for (int i = 0; i < 15; i++)
	{
		m_pObject[i]->Update(m_pCamera->View(), m_pCamera->Proj(), m_pTimer, m_pCamera);
	}

	// Object Index 변경
	if (GetAsyncKeyState(VK_LEFT) & 0x0001)
	{
		m_ObjectIndex--;

		if (m_ObjectIndex < 0)
		{
			m_ObjectIndex = 14;
		}
	}

	if (GetAsyncKeyState(VK_RIGHT) & 0x0001)
	{
		m_ObjectIndex++;

		if (14 < m_ObjectIndex)
		{
			m_ObjectIndex = 0;
		}
	}

	if (GetAsyncKeyState(VK_F5) & 0x0001)
	{
		m_DebugShowFlag ^= true;
	}
}

void HGEngine::BeginRender()
{
	assert(m_pD3dImmediateContext);

	// 후면 버퍼를 특정색으로 지운다
	m_pD3dImmediateContext->ClearRenderTargetView(m_pRenderTargetView,
		reinterpret_cast<const float*>(&DirectX::Colors::DarkSlateBlue));

	// 깊이 버퍼를 1.0f로, 스텐실 버퍼를 0으로 지운다
	m_pD3dImmediateContext->ClearDepthStencilView(m_pDepthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void HGEngine::Render()
{
	m_pD3dImmediateContext->OMSetDepthStencilState(m_pNormalDSS, 0);

	if (m_DebugShowFlag)
	{
		for (int i = 0; i < 2; i++)
		{
			m_pHelperObject[i]->Render();
		}
	}

	m_pObject[m_ObjectIndex]->Render();

	if (m_DebugShowFlag)
	{
		ShowStatus();
	}

	// Restore default
	m_pD3dImmediateContext->RSSetState(0);
}

void HGEngine::EndRender()
{
	assert(m_pSwapChain);

	// 후면 버퍼를 화면에 제시한다
	HR(m_pSwapChain->Present(0, 0));
}

void HGEngine::OnResize()
{
	assert(m_pD3dImmediateContext);
	assert(m_pD3dDevice);
	assert(m_pSwapChain);

	// 버퍼에 대한 참조가 있는 이전 뷰를 해제하고 
	// 이전 Depth/Stencil Buffer 또한 해제한다
	ReleaseCOM(m_pRenderTargetView);
	ReleaseCOM(m_pDepthStencilView);
	ReleaseCOM(m_pDepthStencilBuffer);

	// Swap Chain의 크기를 조정하고 Render Target View를 다시 생성한다
	HR(m_pSwapChain->ResizeBuffers(1, m_ClientWidth, m_ClientHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0));
	ID3D11Texture2D* backBuffer;
	HR(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer)));
	HR(m_pD3dDevice->CreateRenderTargetView(backBuffer, 0, &m_pRenderTargetView));
	ReleaseCOM(backBuffer);

	// Depth/Stencil Buffer와 View 생성
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = m_ClientWidth;
	depthStencilDesc.Height = m_ClientHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// 4X MSAA를 사용하는가? -> Swap Chain의 MSAA 값과 일치해야 한다
	if (m_Enable4xMsaa)
	{
		depthStencilDesc.SampleDesc.Count = 4;
		depthStencilDesc.SampleDesc.Quality = m_4xMsaaQuality - 1;
	}
	// MSAA를 사용하지 않음
	else
	{
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
	}

	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	HR(m_pD3dDevice->CreateTexture2D(&depthStencilDesc, 0, &m_pDepthStencilBuffer));
	HR(m_pD3dDevice->CreateDepthStencilView(m_pDepthStencilBuffer, 0, &m_pDepthStencilView));

	// Render Target View and Depth/Stencil View를 파이프라인에 바인드한다
	m_pD3dImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

	// Viewport 변환 설정
	m_ScreenViewport.TopLeftX = 0;
	m_ScreenViewport.TopLeftY = 0;
	m_ScreenViewport.Width = static_cast<float>(m_ClientWidth);
	m_ScreenViewport.Height = static_cast<float>(m_ClientHeight);
	m_ScreenViewport.MinDepth = 0.0f;
	m_ScreenViewport.MaxDepth = 1.0f;

	m_pD3dImmediateContext->RSSetViewports(1, &m_ScreenViewport);

	// 창 크기가 조정되었으므로 가로 세로 비율을 업데이트하고 투영 행렬을 다시 계산한다
	m_pCamera->SetLens(0.25f * MathHelper::Pi, AspectRatio(), 1.0f, 1000.0f);

	// 창의 크기가 변했으므로, 종횡비를 업데이트하고 투영 행렬을 재계산한다
	XMMATRIX P = XMMatrixPerspectiveFovLH(0.25f * MathHelper::Pi, AspectRatio(), 1.0f, 1000.0f);

	// 바뀐 투영행렬로 카메라의 proj 행렬을 넣어준다
	m_pCamera->SetProj(P);
}

void HGEngine::OnMouseDown(WPARAM btnState, int x, int y)
{
	m_LastMousePos.x = x;
	m_LastMousePos.y = y;

	SetCapture(m_hWnd);
}

void HGEngine::OnMouseUp(WPARAM btnState, int x, int y)
{
	ReleaseCapture();
}

void HGEngine::OnMouseMove(WPARAM btnState, int x, int y)
{
	if ((btnState & MK_RBUTTON) != 0)
	{
		// Make each pixel correspond to a quarter of a degree.
		float dx = XMConvertToRadians(0.25f * static_cast<float>(x - m_LastMousePos.x));
		float dy = XMConvertToRadians(0.25f * static_cast<float>(y - m_LastMousePos.y));

		m_pCamera->Pitch(dy);
		m_pCamera->RotateY(dx);
	}

	m_LastMousePos.x = x;
	m_LastMousePos.y = y;
}

void HGEngine::CreateRenderStates()
{
	// Render State 중 Rasterizer State
	D3D11_RASTERIZER_DESC solidDesc;
	ZeroMemory(&solidDesc, sizeof(D3D11_RASTERIZER_DESC));
	solidDesc.FillMode = D3D11_FILL_SOLID;
	solidDesc.CullMode = D3D11_CULL_BACK;
	solidDesc.FrontCounterClockwise = false;
	solidDesc.DepthClipEnable = true;
	HR(m_pD3dDevice->CreateRasterizerState(&solidDesc, &m_pSolidRS));

	D3D11_RASTERIZER_DESC wireframeDesc;
	ZeroMemory(&wireframeDesc, sizeof(D3D11_RASTERIZER_DESC));
	wireframeDesc.FillMode = D3D11_FILL_WIREFRAME;
	wireframeDesc.CullMode = D3D11_CULL_BACK;
	wireframeDesc.FrontCounterClockwise = false;
	wireframeDesc.DepthClipEnable = true;
	HR(m_pD3dDevice->CreateRasterizerState(&wireframeDesc, &m_pWireframeRS));

	// 폰트용 DSS
	D3D11_DEPTH_STENCIL_DESC equalsDesc;
	ZeroMemory(&equalsDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	equalsDesc.DepthEnable = true;
	equalsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;		// 깊이버퍼에 쓰기는 한다
	equalsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	HR(m_pD3dDevice->CreateDepthStencilState(&equalsDesc, &m_pNormalDSS));
}

float HGEngine::AspectRatio()
{
	// 후면 버퍼의 종횡비, 즉 높이에 대한 너비의 비율을 돌려준다
	return static_cast<float>(m_ClientWidth) / m_ClientHeight;
}

// 디바이스/GPU 정보 획득 <방법 #1> 
// DXGI 1.1 / DXGI Factory 사용 / DX11 이상시 권장, DXGI 1.0과 1.1 혼용금지. DXGI 1.3은 Windows 8.1 이상필수
HRESULT HGEngine::AdapterInfo()
{
	// DXGI버전별로 다름
	IDXGIAdapter1* pAdapter;
	IDXGIFactory1* pFactory = NULL;

	HRESULT hr = S_OK;

	// DXGIFactory 개체 생성(DXGI.lib 필요)
	if (FAILED(CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&pFactory)))
	{
		return E_FAIL;
	}

	hr = pFactory->EnumAdapters1(0, &pAdapter);

	if (hr != DXGI_ERROR_NOT_FOUND)
	{
		pAdapter->GetDesc1(&m_AdapterDesc1);
	}

	SAFE_RELEASE(pAdapter);
	SAFE_RELEASE(pFactory);

	return S_OK;
}

void HGEngine::ShowStatus()
{
	SimpleMath::Vector4 yellow(1.0f, 1.0f, 0.0f, 1.0f);
	SimpleMath::Vector4 white(1.0f, 1.0f, 1.0f, 1.0f);
	SimpleMath::Vector4 red(1.0f, 0.0f, 0.5f, 1.0f);
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

	m_pFont->DrawTextColor(1920 / 2 - 80, 10, yellow, (TCHAR*)L"오브젝트 변경 : <-  방향키  ->");
	m_pFont->DrawTextColor(200, 100, yellow, (TCHAR*)L"한글과 버퍼가 잘 작동하는가? %d / %f / %s", 79, 3.14f, L"노란색글씨 똠방각하 펲시 콜라 뾸뾸");

	// 피쳐레벨
	int _yPos = 50;
	m_pFont->DrawTextColor(1, _yPos, white, (TCHAR*)L"Feature Level : %x", m_FeatureLevel);

	// 어댑터 정보
	m_pFont->DrawTextColor(1, _yPos += 15, white, (TCHAR*)L"Description : %s", m_AdapterDesc1.Description);
	m_pFont->DrawTextColor(1, _yPos += 15, white, (TCHAR*)L"VendorID : %u", m_AdapterDesc1.VendorId);
	m_pFont->DrawTextColor(1, _yPos += 15, white, (TCHAR*)L"DeviceID : %u", m_AdapterDesc1.DeviceId);
	m_pFont->DrawTextColor(1, _yPos += 15, white, (TCHAR*)L"SubSysID : %u", m_AdapterDesc1.SubSysId);
	m_pFont->DrawTextColor(1, _yPos += 15, white, (TCHAR*)L"Revision : %u", m_AdapterDesc1.Revision);
	m_pFont->DrawTextColor(1, _yPos += 15, white, (TCHAR*)L"VideoMemory : %lu MB", m_AdapterDesc1.DedicatedVideoMemory / 1024 / 1024);
	m_pFont->DrawTextColor(1, _yPos += 15, white, (TCHAR*)L"SystemMemory : %lu MB", m_AdapterDesc1.DedicatedSystemMemory / 1024 / 1024);
	m_pFont->DrawTextColor(1, _yPos += 15, white, (TCHAR*)L"SharedSysMemory : %lu MB", m_AdapterDesc1.SharedSystemMemory / 1024 / 1024);
	m_pFont->DrawTextColor(1, _yPos += 15, white, (TCHAR*)L"AdpaterLuid : %u.%d", m_AdapterDesc1.AdapterLuid.HighPart, m_AdapterDesc1.AdapterLuid.LowPart);

	// Debug Show Flag
	m_pFont->DrawTextColor(1, _yPos += 30, red, (TCHAR*)L"Debug Show On / Off : F5");

	// 카메라 정보
	m_pFont->DrawTextColor(1, _yPos += 30, customColor, (TCHAR*)L"Camera Pos : %.2f / %.2f / %.2f", m_pCamera->GetPosition().x, m_pCamera->GetPosition().y, m_pCamera->GetPosition().z);
	m_pFont->DrawTextColor(1, _yPos += 15, customColor, (TCHAR*)L"Camera 이동속도 증가 : Shift");

	// 보여지는 Object 정보
	m_pFont->DrawTextColor(1, _yPos += 30, white, (TCHAR*)L"현재 Object Index : %d", m_ObjectIndex);
}

void HGEngine::CreateHelperObject()
{
	m_pHelperObject[0] = new Grid(m_pD3dDevice, m_pD3dImmediateContext, m_pWireframeRS);
	m_pHelperObject[1] = new Axis(m_pD3dDevice, m_pD3dImmediateContext, m_pWireframeRS);

	for (int i = 0; i < 2; i++)
	{
		m_pHelperObject[i]->Initialize();
	}
}

void HGEngine::CreateObject()
{
	m_pObject[0] = new Box(m_pD3dDevice, m_pD3dImmediateContext, m_pSolidRS);
	m_pObject[1] = new Hills(m_pD3dDevice, m_pD3dImmediateContext, m_pSolidRS);
	m_pObject[2] = new Skull(m_pD3dDevice, m_pD3dImmediateContext, m_pWireframeRS);
	m_pObject[3] = new Car(m_pD3dDevice, m_pD3dImmediateContext, m_pWireframeRS);
	m_pObject[4] = new Waves(m_pD3dDevice, m_pD3dImmediateContext, m_pWireframeRS);
	m_pObject[5] = new MikeWazowski(m_pD3dDevice, m_pD3dImmediateContext, m_pSolidRS);
	m_pObject[6] = new TexWave(m_pD3dDevice, m_pD3dImmediateContext, m_pSolidRS);
	m_pObject[7] = new TexHill(m_pD3dDevice, m_pD3dImmediateContext, m_pSolidRS);
	m_pObject[8] = new TexBox(m_pD3dDevice, m_pD3dImmediateContext, m_pSolidRS);
	m_pObject[9] = new LightingSkull(m_pD3dDevice, m_pD3dImmediateContext, m_pSolidRS);
	m_pObject[10] = new LightingHillAndWave(m_pD3dDevice, m_pD3dImmediateContext, m_pSolidRS);
	m_pObject[11] = new ASEObject(m_pD3dDevice, m_pD3dImmediateContext, m_pSolidRS);
	m_pObject[12] = new ASEObject(m_pD3dDevice, m_pD3dImmediateContext, m_pSolidRS);
	m_pObject[13] = new ASEObject(m_pD3dDevice, m_pD3dImmediateContext, m_pSolidRS);
	m_pObject[14] = new ASEObject(m_pD3dDevice, m_pD3dImmediateContext, m_pSolidRS);

	for (int i = 0; i < 15; i++)
	{
		m_pObject[i]->Initialize();
	}

	/// 추후 필요 작업
	/// Load -> ResourceManager, Parser
	/// 오브젝트 생성 (Factory, Builder)

	m_pASEParser->Load((LPSTR)"../Data/ASE/box.ASE");
	m_pObject[11]->LoadGeometry(m_pASEParser);

	m_pASEParser->Load((LPSTR)"../Data/ASE/teapot.ASE");
	m_pObject[12]->LoadGeometry(m_pASEParser);

	m_pASEParser->Load((LPSTR)"../Data/ASE/genji_max.ASE");
	m_pObject[13]->LoadGeometry(m_pASEParser);
	m_pObject[13]->LoadTextureFile(L"../Data/Textures/000000002405_reverse.dds");

	m_pASEParser->Load((LPSTR)"../Data/ASE/03IK-Joe_onlymesh.ASE");
	m_pObject[14]->LoadGeometry(m_pASEParser);
	m_pObject[14]->SetEditWorld(XMMatrixScaling(0.05f, 0.05f, 0.05f));
}
