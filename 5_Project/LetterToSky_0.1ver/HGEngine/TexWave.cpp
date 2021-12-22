#include "TexWave.h"
#include "D3Camera.h"
#include "DDSTextureLoader.h"
#include "GameTimer.h"
#include "MathHelper.h"
#include "Vertex.h"
#include "Effects.h"

TexWave::TexWave(DXDevice* pDevice)
	: TextureObject(pDevice),
	mWaterTexOffset(0.0f, 0.0f),

	mNumRows(0), mNumCols(0),
	mVertexCount(0),
	mK1(0.0f), mK2(0.0f), mK3(0.0f),
	mTimeStep(0.0f), mSpatialStep(0.0f),
	mPrevSolution(0), mCurrSolution(0), mNormals(0), mTangentX(0)
{

}

TexWave::~TexWave()
{
	delete[] mPrevSolution;
	delete[] mCurrSolution;
	delete[] mNormals;
	delete[] mTangentX;
}

void TexWave::Initialize()
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
	m_IndexCount = (m - 1) * (n - 1) * 2;

	mTimeStep = dt;
	mSpatialStep = dx;

	float d = damping * dt + 2.0f;
	float e = (speed * speed) * (dt * dt) / (dx * dx);
	mK1 = (damping * dt - 2.0f) / d;
	mK2 = (4.0f - 8.0f * e) / d;
	mK3 = (2.0f * e) / d;

	// In case Init() called again.
	delete[] mPrevSolution;
	delete[] mCurrSolution;
	delete[] mNormals;
	delete[] mTangentX;

	mPrevSolution = new XMFLOAT3[m * n];
	mCurrSolution = new XMFLOAT3[m * n];
	mNormals = new XMFLOAT3[m * n];
	mTangentX = new XMFLOAT3[m * n];

	// Generate grid vertices in system memory.

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

void TexWave::Update(GameTimer* pTimer /*= nullptr*/, D3Camera* pCamera /*= nullptr*/)
{
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
	HR(md3dImmediateContext->Map(m_VB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));

	VertexInputLayout::PosNomTexture* v = reinterpret_cast<VertexInputLayout::PosNomTexture*>(mappedData.pData);
	for (UINT i = 0; i < mVertexCount; ++i)
	{
		v[i].Pos = mCurrSolution[i];
		v[i].Normal = mNormals[i];

		// Derive tex-coords in [0,1] from position
		v[i].Texture.x = 0.5f + mCurrSolution[i].x / mNumCols * mSpatialStep;
		v[i].Texture.y = 0.5f - mCurrSolution[i].z / mNumRows * mSpatialStep;
	}

	md3dImmediateContext->Unmap(m_VB, 0);

	// Animate water texture coordinates

	// Tile water texture
	XMMATRIX wavesScale = XMMatrixScaling(5.0f, 5.0f, 0.0f);

	// Translate texture over time
	mWaterTexOffset.y += 0.05f * pTimer->DeltaTime();
	mWaterTexOffset.x += 0.1f * pTimer->DeltaTime();
	XMMATRIX wavesOffset = XMMatrixTranslation(mWaterTexOffset.x, mWaterTexOffset.y, 0.0f);

	// Combine scale and translation
	m_TexTransform = wavesScale * wavesOffset;

	mView = pCamera->View();
	mProj = pCamera->Proj();
}

void TexWave::BuildGeometryBuffers()
{
	// Create the vertex buffer. Note that we allocate space only, as
	// we will be updating the data every time step of the simulation
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_DYNAMIC;
	vbd.ByteWidth = sizeof(VertexInputLayout::PosNomTexture) * mVertexCount;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vbd.MiscFlags = 0;
	HR(md3dDevice->CreateBuffer(&vbd, 0, &m_VB));

	// Create the index buffer. The index buffer is fixed, so we only 
	// need to create and set once
	std::vector<UINT> indices(3 * m_IndexCount); // 3 indices per face

	// Iterate over each quad
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
	HR(md3dDevice->CreateBuffer(&ibd, &iinitData, &m_IB));
}

void TexWave::UpdateBuffer(float deltaTime)
{
	static float t = 0;

	// Accumulate time
	t += deltaTime;

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

void TexWave::Disturb(UINT i, UINT j, float magnitude)
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
