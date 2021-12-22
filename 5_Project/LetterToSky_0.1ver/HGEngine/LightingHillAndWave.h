#pragma once
#include "IDXObjectBase.h"
#include "LightHelper.h"

class D3Camera;
class GameTimer;

/// <summary>
/// Lighting Class
/// 
/// Direct3D 11 용책 예제
/// 2021. 06. 18 Hacgeum
class LightingHillAndWave : public IDXObjectBase
{
public:
	LightingHillAndWave(DXDevice* pDevice);
	virtual ~LightingHillAndWave();

	virtual void Initialize();
	virtual void Update(GameTimer* pTimer = nullptr, D3Camera* pCamera = nullptr) override;
	virtual void Render();

private:
	void UpdateBuffer(float dt);
	void Disturb(UINT i, UINT j, float magnitude);
	float GetHillHeight(float x, float z)const;
	XMFLOAT3 GetHillNormal(float x, float z)const;
	virtual void BuildGeometryBuffers();
	void BuildLandGeometryBuffers();
	void BuildWaveGeometryBuffers();

private:
	ID3D11Buffer* mLandVB;
	ID3D11Buffer* mLandIB;

	ID3D11Buffer* mWavesVB;
	ID3D11Buffer* mWavesIB;

	DirectionalLight mDirLight;
	PointLight mPointLight;
	SpotLight mSpotLight;
	Material mLandMat;
	Material mWavesMat;

	// Define transformations from local spaces to world space.
	XMFLOAT4X4 mLandWorld;
	XMFLOAT4X4 mWavesWorld;

	XMMATRIX mView;
	XMMATRIX mProj;

	UINT mLandIndexCount;

	float mTheta;
	float mPhi;
	float mRadius;

private:
	UINT mNumRows;
	UINT mNumCols;

	UINT mVertexCount;
	UINT mTriangleCount;

	// Simulation constants we can precompute.
	float mK1;
	float mK2;
	float mK3;

	float mTimeStep;
	float mSpatialStep;

	XMFLOAT3* mPrevSolution;
	XMFLOAT3* mCurrSolution;
	XMFLOAT3* mNormals;
	XMFLOAT3* mTangentX;
};
