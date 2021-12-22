#include "DXDevice.h"
#include "../DXTK/Inc/SimpleMath.h"
#include "D3Camera.h"
#include "GeometryGenerator.h"
#include "Vertex.h"
#include "Effects.h"

DXDevice::DXDevice(HWND hWnd, int width, int height)
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

	m_pCamera(),
	m_LastMousePos{ 0,0 }
{
	ZeroMemory(&m_ScreenViewport, sizeof(D3D11_VIEWPORT));

	// Direc3D Camera ����
	m_pObjectTrackingCam = new D3Camera;
	m_pObjectTrackingCam->LookAt(XMFLOAT3(7.0f, 4.0f, -20.0f), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 1.0f, 0));
	m_pObjectTrackingCam->SetIndex(1);
	m_pCamera.push_back(m_pObjectTrackingCam);

	m_pDebugCam = new D3Camera;
	m_pDebugCam->LookAt(XMFLOAT3(8.0f, 8.0f, -8.0f), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 1.0f, 0));
	m_pDebugCam->SetIndex(0);
	m_pCamera.push_back(m_pDebugCam);
}

DXDevice::~DXDevice()
{
	ReleaseCOM(m_RasterState);
	ReleaseCOM(m_DepthDisabledStencilState);
	ReleaseCOM(m_DepthStencilState);

	SafeDelete(mBlur);

	ReleaseCOM(mScreenQuadVB);
	ReleaseCOM(mScreenQuadIB);
	ReleaseCOM(mOffscreenSRV);
	ReleaseCOM(mOffscreenUAV);
	ReleaseCOM(mOffscreenRTV);

	m_pCamera.clear();

	SafeDelete(m_pObjectTrackingCam);
	SafeDelete(m_pDebugCam);

	ReleaseCOM(m_pRenderTargetView);
	ReleaseCOM(m_pDepthStencilView);
	ReleaseCOM(m_pSwapChain);
	ReleaseCOM(m_pDepthStencilBuffer);

	// ��� �⺻ ������ ����
	if (m_pD3dImmediateContext)
		m_pD3dImmediateContext->ClearState();

	ReleaseCOM(m_pD3dImmediateContext);
	ReleaseCOM(m_pD3dDevice);
}

bool DXDevice::Initialize()
{
	// D3DDevice�� D3DImmediateContext�� �����Ѵ�
	UINT createDeviceFlags = 0;

#if defined(DEBUG) || defined(_DEBUG)  
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	HRESULT hr = D3D11CreateDevice(
		0,							// �⺻ �����
		m_D3dDriverType,
		0,							// ����Ʈ���� ��ġ�� ������� ����
		createDeviceFlags,
		0, 0,						// �⺻ ��� ���� �迭
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

	// �� ���� ���Ŀ� ���� 4X MSAA ǰ�� ���� Ȯ��
	// ��� Direct3D 11 ���� ��ġ�� ��� ���� ��� ���Ŀ� ���� 4X MSAA ����
	// ���� �츮�� ǰ�� ������ Ȯ���ϸ� �ȴ�
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

	// 4X MSAA�� ����ϴ°�? 
	if (m_Enable4xMsaa)
	{
		sd.SampleDesc.Count = 4;
		sd.SampleDesc.Quality = m_4xMsaaQuality - 1;
	}
	// MSAA�� ������� ����
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

	// Swap Chain�� �ùٸ��� �����Ϸ��� ������ ���� �ؾ��Ѵ�
	// ����̽��� �����ϴ� �� ���� IDXGIFactory�� ����ؾ� �Ѵ�
	// �ٸ� IDXGIFactory �ν��Ͻ��� ����Ϸ��� �ϸ�
	// ������ ��´�. "IDXGIFactory::CreateSwapChain : This function is being called with a device from a different IDXGIFactory"

	// D3����̽��κ��� dxgi����̽��� ���´�
	IDXGIDevice* dxgiDevice = 0;
	HR(m_pD3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice));

	// dxgi����̽��κ��� dxgi�����͸� ���´�
	IDXGIAdapter* dxgiAdapter = 0;
	HR(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter));

	// dxgi�����ͷκ��� dxgi���丮�� ���´�
	IDXGIFactory* dxgiFactory = 0;
	HR(dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory));

	// Swap Chain ����
	HR(dxgiFactory->CreateSwapChain(m_pD3dDevice, &sd, &m_pSwapChain));
	dxgiFactory->MakeWindowAssociation(m_hWnd, DXGI_MWA_NO_ALT_ENTER);

	ReleaseCOM(dxgiDevice);
	ReleaseCOM(dxgiAdapter);
	ReleaseCOM(dxgiFactory);

	// d3d ������ ���� �����ؾ� �ϴ� ������ �ܰ赵 â�� ũ�Ⱑ ������ �� ���� �����ؾ� �Ѵ�
	// �ڵ� �ߺ��� �����Ϸ��� OnResize �޼ҵ带 ����� ȣ���ؾ��Ѵ�
	OnResize();

	// Recreate the resources that depend on the client area size.
	BuildOffscreenViews();

	mBlur = new BlurFilter;
	mBlur->Init(m_pD3dDevice, m_ClientWidth, m_ClientHeight, DXGI_FORMAT_R8G8B8A8_UNORM);

	// ������� ������ ��´�
	AdapterInfo();

	return true;
}

void DXDevice::OnResize()
{
	assert(m_pD3dImmediateContext);
	assert(m_pD3dDevice);
	assert(m_pSwapChain);

	// ���ۿ� ���� ������ �ִ� ���� �並 �����ϰ� 
	// ���� Depth/Stencil Buffer ���� �����Ѵ�
	ReleaseCOM(m_pRenderTargetView);
	ReleaseCOM(m_pDepthStencilView);
	ReleaseCOM(m_pDepthStencilBuffer);

	// Swap Chain�� ũ�⸦ �����ϰ� Render Target View�� �ٽ� �����Ѵ�
	HR(m_pSwapChain->ResizeBuffers(1, m_ClientWidth, m_ClientHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0));
	ID3D11Texture2D* backBuffer;
	HR(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer)));
	HR(m_pD3dDevice->CreateRenderTargetView(backBuffer, 0, &m_pRenderTargetView));
	ReleaseCOM(backBuffer);

	// Depth/Stencil Buffer�� View ����
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	depthBufferDesc.Width = m_ClientWidth;
	depthBufferDesc.Height = m_ClientHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// 4X MSAA�� ����ϴ°�? -> Swap Chain�� MSAA ���� ��ġ�ؾ� �Ѵ�
	if (m_Enable4xMsaa)
	{
		depthBufferDesc.SampleDesc.Count = 4;
		depthBufferDesc.SampleDesc.Quality = m_4xMsaaQuality - 1;
	}
	// MSAA�� ������� ����
	else
	{
		depthBufferDesc.SampleDesc.Count = 1;
		depthBufferDesc.SampleDesc.Quality = 0;
	}

	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	HR(m_pD3dDevice->CreateTexture2D(&depthBufferDesc, 0, &m_pDepthStencilBuffer));

	//////////////////////////////////////////////////////////////////////////

	// DepthStencilState ����ü �ʱ�ȭ
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// DepthStencilState ����ü�� �ۼ�
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// �ȼ� ������ ���ٽ� ����
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// �ȼ� �޸��� ���ٽ� ����
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	HR(m_pD3dDevice->CreateDepthStencilState(&depthStencilDesc, &m_DepthStencilState));

	// ���� ���ٽ� ���¸� ����
	m_pD3dImmediateContext->OMSetDepthStencilState(m_DepthStencilState, 1);

	// ���� ���ٽ� ���� ����ü�� �ʱ�ȭ
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// ���� ���ٽ� �� ����ü�� ����
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// ���� ���ٽ� �並 ����
	HR(m_pD3dDevice->CreateDepthStencilView(m_pDepthStencilBuffer, &depthStencilViewDesc, &m_pDepthStencilView));

	// Render Target View and Depth/Stencil View�� ���������ο� ���ε��Ѵ�
	m_pD3dImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

	// �׷����� ������� ����� ������ ������ ����ü�� ����
	D3D11_RASTERIZER_DESC rasterDesc;
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	HR(m_pD3dDevice->CreateRasterizerState(&rasterDesc, &m_RasterState));

	// ���� ������ ������ ���¸� �����մϴ�
	m_pD3dImmediateContext->RSSetState(m_RasterState);
	
	//////////////////////////////////////////////////////////////////////////

	// Viewport ��ȯ ����
	m_ScreenViewport.Width = static_cast<float>(m_ClientWidth);
	m_ScreenViewport.Height = static_cast<float>(m_ClientHeight);
	m_ScreenViewport.MinDepth = 0.0f;
	m_ScreenViewport.MaxDepth = 1.0f;
	m_ScreenViewport.TopLeftX = 0;
	m_ScreenViewport.TopLeftY = 0;

	m_pD3dImmediateContext->RSSetViewports(1, &m_ScreenViewport);

	// â ũ�Ⱑ �����Ǿ����Ƿ� ���� ���� ������ ������Ʈ�ϰ� ���� ����� �ٽ� ����Ѵ�
	m_pDebugCam->SetLens(0.25f * MathHelper::Pi, AspectRatio(), 1.0f, 1000.0f);
	m_pObjectTrackingCam->SetLens(0.25f * MathHelper::Pi, AspectRatio(), 1.0f, 1000.0f);

	// â�� ũ�Ⱑ �������Ƿ�, ��Ⱦ�� ������Ʈ�ϰ� ���� ����� �����Ѵ�
	XMMATRIX P = XMMatrixPerspectiveFovLH(0.25f * MathHelper::Pi, AspectRatio(), 0.1f, 1000.0f);

	// �ٲ� ������ķ� ī�޶��� proj ����� �־��ش�
	m_pDebugCam->SetProj(P);
	m_pObjectTrackingCam->SetProj(P);

	// 2D ������������ ���� ���� ����� ����
	m_OrthoMatrix = XMMatrixOrthographicLH((float)m_ClientWidth, (float)m_ClientHeight, 0.1f, 1000.0f);

	// ���� 2D ������������ Z ���۸� ���� �� ��° ���� ���ٽ� ���¸� ����ϴ�. ������ ��������
	// DepthEnable�� false�� �����ϸ� �ٸ� ��� �Ű� ������ �ٸ� ���� ���ٽ� ���¿� �����մϴ�.
	D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;
	ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));

	depthDisabledStencilDesc.DepthEnable = false;
	depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthDisabledStencilDesc.StencilEnable = true;
	depthDisabledStencilDesc.StencilReadMask = 0xFF;
	depthDisabledStencilDesc.StencilWriteMask = 0xFF;
	depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	HR(m_pD3dDevice->CreateDepthStencilState(&depthDisabledStencilDesc, &m_DepthDisabledStencilState));
}

void DXDevice::BuildScreenQuadGeometryBuffers()
{
	GeometryGenerator::MeshData quad;

	GeometryGenerator geoGen;
	geoGen.CreateFullscreenQuad(quad);

	//
	// Extract the vertex elements we are interested in and pack the
	// vertices of all the meshes into one vertex buffer.
	//

	std::vector<VertexInputLayout::PosNomTexture> vertices(quad.Vertices.size());

	for (UINT i = 0; i < quad.Vertices.size(); ++i)
	{
		vertices[i].Pos = quad.Vertices[i].Position;
		vertices[i].Normal = quad.Vertices[i].Normal;
		vertices[i].Texture = quad.Vertices[i].TexC;
	}

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(VertexInputLayout::PosNomTexture) * quad.Vertices.size();
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &vertices[0];
	HR(m_pD3dDevice->CreateBuffer(&vbd, &vinitData, &mScreenQuadVB));

	//
	// Pack the indices of all the meshes into one index buffer.
	//

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * quad.Indices.size();
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &quad.Indices[0];
	HR(m_pD3dDevice->CreateBuffer(&ibd, &iinitData, &mScreenQuadIB));
}

void DXDevice::BuildOffscreenViews()
{
	// We call this function everytime the window is resized so that the render target is a quarter
	// the client area dimensions.  So Release the previous views before we create new ones.
	ReleaseCOM(mOffscreenSRV);
	ReleaseCOM(mOffscreenRTV);
	ReleaseCOM(mOffscreenUAV);

	D3D11_TEXTURE2D_DESC texDesc;

	texDesc.Width = m_ClientWidth;
	texDesc.Height = m_ClientHeight;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	ID3D11Texture2D* offscreenTex = 0;
	HR(m_pD3dDevice->CreateTexture2D(&texDesc, 0, &offscreenTex));

	// Null description means to create a view to all mipmap levels using 
	// the format the texture was created with.
	HR(m_pD3dDevice->CreateShaderResourceView(offscreenTex, 0, &mOffscreenSRV));
	HR(m_pD3dDevice->CreateRenderTargetView(offscreenTex, 0, &mOffscreenRTV));
	HR(m_pD3dDevice->CreateUnorderedAccessView(offscreenTex, 0, &mOffscreenUAV));

	// View saves a reference to the texture so we can release our reference.
	ReleaseCOM(offscreenTex);
}

void DXDevice::DrawScreenQuad()
{
	m_pD3dImmediateContext->IASetInputLayout(InputLayouts::PosNomTexture);
	m_pD3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	UINT stride = sizeof(VertexInputLayout::PosNomTexture);
	UINT offset = 0;

	XMMATRIX identity = XMMatrixIdentity();

	ID3DX11EffectTechnique* texOnlyTech = Effects::BasicFX->Light0TexTech;
	D3DX11_TECHNIQUE_DESC techDesc;

	texOnlyTech->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		m_pD3dImmediateContext->IASetVertexBuffers(0, 1, &mScreenQuadVB, &stride, &offset);
		m_pD3dImmediateContext->IASetIndexBuffer(mScreenQuadIB, DXGI_FORMAT_R32_UINT, 0);

		Effects::BasicFX->SetWorld(identity);
		Effects::BasicFX->SetWorldInvTranspose(identity);
		Effects::BasicFX->SetWorldViewProj(identity);
		Effects::BasicFX->SetTexTransform(identity);
		Effects::BasicFX->SetDiffuseMap(mBlur->GetBlurredOutput());

		texOnlyTech->GetPassByIndex(p)->Apply(0, m_pD3dImmediateContext);
		m_pD3dImmediateContext->DrawIndexed(6, 0, 0);
	}
}

void DXDevice::TurnZBufferOn()
{
	m_pD3dImmediateContext->OMSetDepthStencilState(m_DepthStencilState, 1);
}

void DXDevice::TurnZBufferOff()
{
	m_pD3dImmediateContext->OMSetDepthStencilState(m_DepthDisabledStencilState, 1);
}

float DXDevice::AspectRatio()
{
	// �ĸ� ������ ��Ⱦ��, �� ���̿� ���� �ʺ��� ������ �����ش�
	return static_cast<float>(m_ClientWidth) / m_ClientHeight;
}

// ����̽�/GPU ���� ȹ�� <��� #1> 
// DXGI 1.1 / DXGI Factory ��� / DX11 �̻�� ����, DXGI 1.0�� 1.1 ȥ�����. DXGI 1.3�� Windows 8.1 �̻��ʼ�
HRESULT DXDevice::AdapterInfo()
{
	// DXGI�������� �ٸ�
	IDXGIAdapter1* pAdapter;
	IDXGIFactory1* pFactory = NULL;

	HRESULT hr = S_OK;

	// DXGIFactory ��ü ����(DXGI.lib �ʿ�)
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

void DXDevice::OnMouseDown(WPARAM btnState, int x, int y)
{
	m_LastMousePos.x = x;
	m_LastMousePos.y = y;

	SetCapture(m_hWnd);
}

void DXDevice::OnMouseUp(WPARAM btnState, int x, int y)
{
	ReleaseCapture();
}

void DXDevice::OnMouseMove(WPARAM btnState, int x, int y)
{
	if ((btnState & MK_RBUTTON) != 0)
	{
		// Make each pixel correspond to a quarter of a degree.
		float dx = XMConvertToRadians(0.25f * static_cast<float>(x - m_LastMousePos.x));
		float dy = XMConvertToRadians(0.25f * static_cast<float>(y - m_LastMousePos.y));

		m_pDebugCam->Pitch(dy);
		m_pDebugCam->RotateY(dx);
	}

	m_LastMousePos.x = x;
	m_LastMousePos.y = y;
}