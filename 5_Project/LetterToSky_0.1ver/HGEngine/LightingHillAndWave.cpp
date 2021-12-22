#include "LightingHillAndWave.h"
#include "D3Camera.h"
#include "GameTimer.h"
#include "GeometryGenerator.h"
#include "Vertex.h"
#include "Effects.h"

LightingHillAndWave::LightingHillAndWave(DXDevice* pDevice)
	: IDXObjectBase(pDevice),
	mLandVB(0), mLandIB(0), mWavesVB(0), mWavesIB(0),
	mLandWorld(), mWavesWorld(), mView(), mProj(), mLandIndexCount(0),
	mTheta(1.5f * MathHelper::Pi), mPhi(0.1f * MathHelper::Pi), mRadius(80.0f),

	mNumRows(0), mNumCols(0), mVertexCount(0), mTriangleCount(0),
	mK1(0.0f), mK2(0.0f), mK3(0.0f), mTimeStep(0.0f), mSpatialStep(0.0f),
	mPrevSolution(0), mCurrSolution(0), mNormals(0), mTangentX(0)
{
	XMMATRIX wavesOffset = XMMatrixTranslation(0.0f, -3.0f, 0.0f);
	XMStoreFloat4x4(&mWavesWorld, wavesOffset);

	// Directional light
	mDirLight.Ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	mDirLight.Diffuse = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	mDirLight.Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	mDirLight.Direction = XMFLOAT3(0.57735f, -0.57735f, 0.57735f);

	// Point light--position is changed every frame to animate in UpdateScene function
	mPointLight.Ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	mPointLight.Diffuse = XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
	mPointLight.Specular = XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
	mPointLight.Att = XMFLOAT3(0.0f, 0.1f, 0.0f);
	mPointLight.Range = 25.0f;

	// Spot light--position and direction changed every frame to animate in UpdateScene function
	mSpotLight.Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mSpotLight.Diffuse = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
	mSpotLight.Specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	mSpotLight.Att = XMFLOAT3(1.0f, 0.0f, 0.0f);
	mSpotLight.Spot = 96.0f;
	mSpotLight.Range = 10000.0f;

	mLandMat.Ambient = XMFLOAT4(0.48f, 0.77f, 0.46f, 1.0f);
	mLandMat.Diffuse = XMFLOAT4(0.48f, 0.77f, 0.46f, 1.0f);
	mLandMat.Specular = XMFLOAT4(0.2f, 0.2f, 0.2f, 16.0f);

	mWavesMat.Ambient = XMFLOAT4(0.137f, 0.42f, 0.556f, 1.0f);
	mWavesMat.Diffuse = XMFLOAT4(0.137f, 0.42f, 0.556f, 1.0f);
	mWavesMat.Specular = XMFLOAT4(0.8f, 0.8f, 0.8f, 96.0f);

	// 오브젝트의 World 좌표를 단위행렬로 초기화한다
	XMMATRIX world = XMMatrixIdentity();
	XMStoreFloat4x4(&mLandWorld, world);
	XMStoreFloat4x4(&mWavesWorld, world);
}

LightingHillAndWave::~LightingHillAndWave()
{
	delete[] mPrevSolution;
	delete[] mCurrSolution;
	delete[] mNormals;
	delete[] mTangentX;

	ReleaseCOM(mLandVB);
	ReleaseCOM(mLandIB);
	ReleaseCOM(mWavesVB);
	ReleaseCOM(mWavesIB);
}

void LightingHillAndWave::Initialize()
{
	// 값 초기화
	UINT m = 160;
	UINT n = 160;
	float dx = 1.0f;
	float dt = 0.03f;
	float speed = 3.25f;
	float damping = 0.4f;

	mNumRows = m;
	mNumCols = n;

	mVertexCount = m * n;
	mTriangleCount = (m - 1) * (n - 1) * 2;

	mTimeStep = dt;
	mSpatialStep = dx;

	float d = damping * dt + 2.0f;
	float e = (speed * speed) * (dt * dt) / (dx * dx);
	mK1 = (damping * dt - 2.0f) / d;
	mK2 = (4.0f - 8.0f * e) / d;
	mK3 = (2.0f * e) / d;

	// In case Init() called again
	delete[] mPrevSolution;
	delete[] mCurrSolution;
	delete[] mNormals;
	delete[] mTangentX;

	mPrevSolution = new XMFLOAT3[m * n];
	mCurrSolution = new XMFLOAT3[m * n];
	mNormals = new XMFLOAT3[m * n];
	mTangentX = new XMFLOAT3[m * n];

	// Generate grid vertices in system memory
	float halfWidth = (n - 1) * dx * 0.5f;
	float halfDepth = (m - 1) * dx * 0.5f;
	for (UINT i = 0; i < m; ++i)
	{
		float z = halfDepth - i * dx;
		for (UINT j = 0; j < n; ++j)
		{
			float x = -halfWidth + j * dx;

			mPrevSolution[i * n + j] = XMFLOAT3(x, 0.0f, z);
			mCurrSolution[i * n + j] = XMFLOAT3(x, 0.0f, z);
			mNormals[i * n + j] = XMFLOAT3(0.0f, 1.0f, 0.0f);
			mTangentX[i * n + j] = XMFLOAT3(1.0f, 0.0f, 0.0f);
		}
	}

	BuildGeometryBuffers();
}

void LightingHillAndWave::Update(GameTimer* pTimer /*= nullptr*/, D3Camera* pCamera /*= nullptr*/)
{
	// Build the view matrix.
	XMVECTOR pos = XMVectorSet(mEyePosW.x, mEyePosW.y, mEyePosW.z, 1.0f);
	XMVECTOR target = XMVectorZero();
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	// Every quarter second, generate a random wave
	static float t_base = 0.0f;
	if ((pTimer->TotalTime() - t_base) >= 0.25f)
	{
		t_base += 0.25f;

		DWORD i = 5 + rand() % (mNumRows - 10);
		DWORD j = 5 + rand() % (mNumCols - 10);

		float r = MathHelper::RandF(1.0f, 2.0f);

		Disturb(i, j, r);
	}

	UpdateBuffer(pTimer->DeltaTime());

	// Update the wave vertex buffer with the new solution
	D3D11_MAPPED_SUBRESOURCE mappedData;
	HR(md3dImmediateContext->Map(mWavesVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));

	VertexInputLayout::PosNormal* v = reinterpret_cast<VertexInputLayout::PosNormal*>(mappedData.pData);
	for (UINT i = 0; i < mVertexCount; ++i)
	{
		v[i].Pos = mCurrSolution[i];
		v[i].Normal = mNormals[i];
	}

	md3dImmediateContext->Unmap(mWavesVB, 0);

	// Animate the lights

	// Circle light over the land surface
	mPointLight.Position.x = 70.0f * cosf(0.2f * pTimer->TotalTime());
	mPointLight.Position.z = 70.0f * sinf(0.2f * pTimer->TotalTime());
	mPointLight.Position.y = MathHelper::Max(GetHillHeight(mPointLight.Position.x,
		mPointLight.Position.z), -3.0f) + 10.0f;

	// The spotlight takes on the camera position and is aimed in the
	// same direction the camera is looking. In this way, it looks
	// like we are holding a flashlight
	mSpotLight.Position = mEyePosW;
	XMStoreFloat3(&mSpotLight.Direction, XMVector3Normalize(target - pos));

	mView = pCamera->View();
	mProj = pCamera->Proj();
}

void LightingHillAndWave::Render()
{
	md3dImmediateContext->IASetInputLayout(InputLayouts::PosNormal);
	md3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	UINT stride = sizeof(VertexInputLayout::PosNormal);
	UINT offset = 0;

	XMMATRIX view = mView;
	XMMATRIX proj = mProj;
	XMMATRIX viewProj = view * proj;

	// Set per frame constants
	Effects::LightingFX->SetDirLights(&mDirLight);
	Effects::LightingFX->SetPointLights(&mPointLight);
	Effects::LightingFX->SetSpotLights(&mSpotLight);
	Effects::LightingFX->SetEyePosW(mEyePosW);

	// Render State
	md3dImmediateContext->RSSetState(mRenderstate);

	ID3DX11EffectTechnique* activeTech = Effects::LightingFX->LightTech;

	D3DX11_TECHNIQUE_DESC techDesc;
	activeTech->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		// Draw the hills
		md3dImmediateContext->IASetVertexBuffers(0, 1, &mLandVB, &stride, &offset);
		md3dImmediateContext->IASetIndexBuffer(mLandIB, DXGI_FORMAT_R32_UINT, 0);

		// Set per object constants
		XMMATRIX world = XMLoadFloat4x4(&mLandWorld);
		XMMATRIX worldInvTranspose = MathHelper::InverseTranspose(world);
		XMMATRIX worldViewProj = world * view * proj;

		Effects::LightingFX->SetWorld(world);
		Effects::LightingFX->SetWorldInvTranspose(worldInvTranspose);
		Effects::LightingFX->SetWorldViewProj(worldViewProj);
		Effects::LightingFX->SetMaterial(mLandMat);

		activeTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
		md3dImmediateContext->DrawIndexed(mLandIndexCount, 0, 0);

		// Draw the waves
		md3dImmediateContext->IASetVertexBuffers(0, 1, &mWavesVB, &stride, &offset);
		md3dImmediateContext->IASetIndexBuffer(mWavesIB, DXGI_FORMAT_R32_UINT, 0);

		// Set per object constants
		world = XMLoadFloat4x4(&mWavesWorld);
		worldInvTranspose = MathHelper::InverseTranspose(world);
		worldViewProj = world * view * proj;

		Effects::LightingFX->SetWorld(world);
		Effects::LightingFX->SetWorldInvTranspose(worldInvTranspose);
		Effects::LightingFX->SetWorldViewProj(worldViewProj);
		Effects::LightingFX->SetMaterial(mWavesMat);

		activeTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
		md3dImmediateContext->DrawIndexed(3 * mTriangleCount, 0, 0);
	}
}

void LightingHillAndWave::UpdateBuffer(float dt)
{
	static float t = 0;

	// Accumulate time
	t += dt;

	// Only update the simulation at the specified time step
	if (t >= mTimeStep)
	{
		// Only update interior points; we use zero boundary conditions
		for (UINT i = 1; i < mNumRows - 1; ++i)
		{
			for (UINT j = 1; j < mNumCols - 1; ++j)
			{
				// After this update we will be discarding the old previous
				// buffer, so overwrite that buffer with the new update
				// Note how we can do this inplace (read/write to same element) 
				// because we won't need prev_ij again and the assignment happens last

				// Note j indexes x and i indexes z: h(x_j, z_i, t_k)
				// Moreover, our +z axis goes "down"; this is just to 
				// keep consistent with our row indices going down

				mPrevSolution[i * mNumCols + j].y =
					mK1 * mPrevSolution[i * mNumCols + j].y +
					mK2 * mCurrSolution[i * mNumCols + j].y +
					mK3 * (mCurrSolution[(i + 1) * mNumCols + j].y +
						mCurrSolution[(i - 1) * mNumCols + j].y +
						mCurrSolution[i * mNumCols + j + 1].y +
						mCurrSolution[i * mNumCols + j - 1].y);
			}
		}

		// We just overwrote the previous buffer with the new data, so
		// this data needs to become the current solution and the old
		// current solution becomes the new previous solution
		std::swap(mPrevSolution, mCurrSolution);

		t = 0.0f; // reset time

		// Compute normals using finite difference scheme
		for (UINT i = 1; i < mNumRows - 1; ++i)
		{
			for (UINT j = 1; j < mNumCols - 1; ++j)
			{
				float l = mCurrSolution[i * mNumCols + j - 1].y;
				float r = mCurrSolution[i * mNumCols + j + 1].y;
				float t = mCurrSolution[(i - 1) * mNumCols + j].y;
				float b = mCurrSolution[(i + 1) * mNumCols + j].y;
				mNormals[i * mNumCols + j].x = -r + l;
				mNormals[i * mNumCols + j].y = 2.0f * mSpatialStep;
				mNormals[i * mNumCols + j].z = b - t;

				XMVECTOR n = XMVector3Normalize(XMLoadFloat3(&mNormals[i * mNumCols + j]));
				XMStoreFloat3(&mNormals[i * mNumCols + j], n);

				mTangentX[i * mNumCols + j] = XMFLOAT3(2.0f * mSpatialStep, r - l, 0.0f);
				XMVECTOR T = XMVector3Normalize(XMLoadFloat3(&mTangentX[i * mNumCols + j]));
				XMStoreFloat3(&mTangentX[i * mNumCols + j], T);
			}
		}
	}
}

void LightingHillAndWave::Disturb(UINT i, UINT j, float magnitude)
{
	// Don't disturb boundaries
	assert(i > 1 && i < mNumRows - 2);
	assert(j > 1 && j < mNumCols - 2);

	float halfMag = 0.5f * magnitude;

	// Disturb the ijth vertex height and its neighbors
	mCurrSolution[i * mNumCols + j].y += magnitude;
	mCurrSolution[i * mNumCols + j + 1].y += halfMag;
	mCurrSolution[i * mNumCols + j - 1].y += halfMag;
	mCurrSolution[(i + 1) * mNumCols + j].y += halfMag;
	mCurrSolution[(i - 1) * mNumCols + j].y += halfMag;
}

float LightingHillAndWave::GetHillHeight(float x, float z) const
{
	return 0.3f * (z * sinf(0.1f * x) + x * cosf(0.1f * z));
}

DirectX::XMFLOAT3 LightingHillAndWave::GetHillNormal(float x, float z) const
{
	// n = (-df/dx, 1, -df/dz)
	XMFLOAT3 n(
		-0.03f * z * cosf(0.1f * x) - 0.3f * cosf(0.1f * z),
		1.0f,
		-0.3f * sinf(0.1f * x) + 0.03f * x * sinf(0.1f * z));

	XMVECTOR unitNormal = XMVector3Normalize(XMLoadFloat3(&n));
	XMStoreFloat3(&n, unitNormal);

	return n;
}

void LightingHillAndWave::BuildGeometryBuffers()
{
	BuildLandGeometryBuffers();
	BuildWaveGeometryBuffers();
}

void LightingHillAndWave::BuildLandGeometryBuffers()
{
	GeometryGenerator::MeshData grid;

	GeometryGenerator geoGen;

	geoGen.CreateGrid(160.0f, 160.0f, 50, 50, grid);

	mLandIndexCount = grid.Indices.size();

	// Extract the vertex elements we are interested and apply the height function to
	// each vertex

	std::vector<VertexInputLayout::PosNormal> vertices(grid.Vertices.size());
	for (size_t i = 0; i < grid.Vertices.size(); ++i)
	{
		XMFLOAT3 p = grid.Vertices[i].Position;

		p.y = GetHillHeight(p.x, p.z);

		vertices[i].Pos = p;
		vertices[i].Normal = GetHillNormal(p.x, p.z);
	}

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(VertexInputLayout::PosNormal) * grid.Vertices.size();
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &vertices[0];
	HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mLandVB));

	// Pack the indices of all the meshes into one index buffer
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * mLandIndexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &grid.Indices[0];
	HR(md3dDevice->CreateBuffer(&ibd, &iinitData, &mLandIB));
}

void LightingHillAndWave::BuildWaveGeometryBuffers()
{
	// Create the vertex buffer. Note that we allocate space only, as
	// we will be updating the data every time step of the simulation
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_DYNAMIC;
	vbd.ByteWidth = sizeof(VertexInputLayout::PosNormal) * mVertexCount;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vbd.MiscFlags = 0;
	HR(md3dDevice->CreateBuffer(&vbd, 0, &mWavesVB));

	// Create the index buffer. The index buffer is fixed, so we only 
	// need to create and set once
	std::vector<UINT> indices(3 * mTriangleCount); // 3 indices per face

	// Iterate over each quad.
	UINT m = mNumRows;
	UINT n = mNumCols;
	int k = 0;
	for (UINT i = 0; i < m - 1; ++i)
	{
		for (DWORD j = 0; j < n - 1; ++j)
		{
			indices[k] = i * n + j;
			indices[k + 1] = i * n + j + 1;
			indices[k + 2] = (i + 1) * n + j;

			indices[k + 3] = (i + 1) * n + j;
			indices[k + 4] = i * n + j + 1;
			indices[k + 5] = (i + 1) * n + j + 1;

			k += 6; // next quad
		}
	}

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * indices.size();
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indices[0];
	HR(md3dDevice->CreateBuffer(&ibd, &iinitData, &mWavesIB));
}
