//***************************************************************************************
// HillsDemo.cpp by Frank Luna (C) 2011 All Rights Reserved.
//
// Performs the calculations for the wave simulation.  After the simulation has been
// updated, the client must copy the current solution into vertex buffers for rendering.
// This class only does the calculations, it does not do any drawing.
//***************************************************************************************
#pragma once
#include "VertexColorObject.h"

class Waves : public VertexColorObject
{
public:
	Waves(DXDevice* pDevice);
	virtual ~Waves();

	virtual void Initialize();
	virtual void Update(GameTimer* pTimer = nullptr, D3Camera* pCamera = nullptr) override;

	// Getter / Setter
	UINT GetVertexCount() const { return mVertexCount; }
	UINT GetTriangleCount() const { return mTriangleCount; }
	UINT GetNumRows() const { return mNumRows; }
	UINT GetNumCols() const { return mNumCols; }

private:
	void UpdateBuffer(float deltaTime);
	virtual void LoadGeometry(SGeoBuffer* geoBuffer) override;
	void Disturb(UINT i, UINT j, float magnitude);
	float GetHeight(float x, float z)const;

	// Returns the solution at the ith grid point.
	const XMFLOAT3& operator[](int i)const { return mCurrSolution[i]; }

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
};
