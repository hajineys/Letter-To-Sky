//=======================================================================================
// Waves.cpp by Frank Luna (C) 2008 All Rights Reserved.
//=======================================================================================
#include <algorithm>
#include <vector>
#include <cassert>
#include "Waves.h"
#include "DXDevice.h"
#include "DirectXStruct.h"
#include "DXRenderStates.h"
#include "Vertex.h"
#include "GameTimer.h"
#include "D3Camera.h"
#include "MathHelper.h"

Waves::Waves(DXDevice* pDevice)
	: VertexColorObject(pDevice),
	mNumRows(0), mNumCols(0),
	mVertexCount(0), mTriangleCount(0),
	mK1(0.0f), mK2(0.0f), mK3(0.0f),
	mTimeStep(0.0f), mSpatialStep(0.0f),
	mPrevSolution(0), mCurrSolution(0)
{
	mRenderstate = DXRenderStates::m_pWireframeRS;
}

Waves::~Waves()
{
	delete[] mPrevSolution;
	delete[] mCurrSolution;
}

void Waves::Initialize()
{
	UINT m = 200;
	UINT n = 200;

	float dx = 0.8f;
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

	// In case Init() called again.
	delete[] mPrevSolution;
	delete[] mCurrSolution;

	mPrevSolution = new XMFLOAT3[m * n];
	mCurrSolution = new XMFLOAT3[m * n];

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
		}
	}
}

void Waves::Update(GameTimer* pTimer /*= nullptr*/, D3Camera* pCamera /*= nullptr*/)
{
	mView = pCamera->View();
	mProj = pCamera->Proj();

	// Every quarter second, generate a random wave
	static float t_base = 0.0f;
	if ((pTimer->TotalTime() - t_base) >= 0.25f)
	{
		t_base += 0.25f;

		DWORD i = 5 + rand() % 190;
		DWORD j = 5 + rand() % 190;

		float r = MathHelper::RandF(1.0f, 2.0f);

		Disturb(i, j, r);
	}

	UpdateBuffer(pTimer->DeltaTime());

	// Update the wave vertex buffer with the new solution
	D3D11_MAPPED_SUBRESOURCE mappedData;
	HR(md3dImmediateContext->Map(m_VB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));

	VertexInputLayout::PosColor* v = reinterpret_cast<VertexInputLayout::PosColor*>(mappedData.pData);
	for (UINT i = 0; i < GetVertexCount(); ++i)
	{
		v[i].Pos = mCurrSolution[i];
		v[i].Color = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	}

	md3dImmediateContext->Unmap(m_VB, 0);
}

void Waves::UpdateBuffer(float deltaTime)
{
	static float t = 0;

	// Accumulate time
	t += deltaTime;

	// Only update the simulation at the specified time step
	if (t >= mTimeStep)
	{
		// Only update interior points; we use zero boundary conditions
		for (DWORD i = 1; i < GetNumRows() - 1; ++i)
		{
			for (DWORD j = 1; j < GetNumCols() - 1; ++j)
			{
				// After this update we will be discarding the old previous
				// buffer, so overwrite that buffer with the new update
				// Note how we can do this inplace (read/write to same element) 
				// because we won't need prev_ij again and the assignment happens last

				// Note j indexes x and i indexes z: h(x_j, z_i, t_k)
				// Moreover, our +z axis goes "down"; this is just to 
				// keep consistent with our row indices going down
				mPrevSolution[i * GetNumCols() + j].y =
					mK1 * mPrevSolution[i * GetNumCols() + j].y +
					mK2 * mCurrSolution[i * GetNumCols() + j].y +
					mK3 * (mCurrSolution[(i + 1) * GetNumCols() + j].y +
						mCurrSolution[(i - 1) * GetNumCols() + j].y +
						mCurrSolution[i * GetNumCols() + j + 1].y +
						mCurrSolution[i * GetNumCols() + j - 1].y);
			}
		}

		// We just overwrote the previous buffer with the new data, so
		// this data needs to become the current solution and the old
		// current solution becomes the new previous solution
		std::swap(mPrevSolution, mCurrSolution);

		t = 0.0f; // reset time
	}
}

void Waves::LoadGeometry(SGeoBuffer* geoBuffer)
{
	// ?? ????
	m_VB = geoBuffer->m_VB;
	m_IB = geoBuffer->m_IB;
	m_IndexCount = geoBuffer->m_IndexCount;
}

void Waves::Disturb(UINT i, UINT j, float magnitude)
{
	// Don't disturb boundaries.
	assert(i > 1 && i < GetNumRows() - 2);
	assert(j > 1 && j < GetNumCols() - 2);

	float halfMag = 0.5f * magnitude;

	// Disturb the ijth vertex height and its neighbors.
	mCurrSolution[i * GetNumCols() + j].y += magnitude;
	mCurrSolution[i * GetNumCols() + j + 1].y += halfMag;
	mCurrSolution[i * GetNumCols() + j - 1].y += halfMag;
	mCurrSolution[(i + 1) * GetNumCols() + j].y += halfMag;
	mCurrSolution[(i - 1) * GetNumCols() + j].y += halfMag;
}

float Waves::GetHeight(float x, float z) const
{
	return 0.3f * (z * sinf(0.1f * x) + x * cosf(0.1f * z));
}
