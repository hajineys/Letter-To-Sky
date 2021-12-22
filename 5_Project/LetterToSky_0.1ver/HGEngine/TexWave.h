#pragma once
#include "TextureObject.h"
#include "LightHelper.h"

/// <summary>
/// Texture 적용한 3D 파도
/// 
/// Direct3D 11 용책 예제
/// 2021. 06. 14 Hacgeum
/// </summary>
class TexWave : public TextureObject
{
public:
	TexWave(DXDevice* pDevice);
	virtual ~TexWave();

	virtual void Initialize() override;
	virtual void Update(GameTimer* pTimer = nullptr, D3Camera* pCamera = nullptr) override;

private:
	virtual void BuildGeometryBuffers();
	void UpdateBuffer(float deltaTime);
	void Disturb(UINT i, UINT j, float magnitude);

private:
	XMFLOAT2 mWaterTexOffset;

private:
	UINT mNumRows;
	UINT mNumCols;

	UINT mVertexCount;

	// Simulation constants we can precompute
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
