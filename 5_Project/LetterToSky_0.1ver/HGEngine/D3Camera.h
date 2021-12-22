#pragma once
#include "AlignedAllocationPolicy.h"
#include "DirectXDefine.h"

/// <summary>
/// Direct3D 11 Camera
/// 
/// 2021. 06. 08 Hacgeum
/// </summary>
class D3Camera : public AlignedAllocationPolicy<16>
{
public:
	D3Camera();
	~D3Camera();

	// Get/Set world camera position
	XMVECTOR GetPositionXM() const;
	XMFLOAT3 GetPosition() const;
	void SetPosition(float x, float y, float z);
	void SetPosition(const XMFLOAT3& v);

	// Get camera basis vectors.
	XMVECTOR GetRightXM() const;
	XMFLOAT3 GetRight() const;
	XMVECTOR GetUpXM() const;
	XMFLOAT3 GetUp() const;
	XMVECTOR GetLookXM() const;
	XMFLOAT3 GetLook() const;

	void SetRotation(float x, float y, float z);

	// Get frustum properties.
	float GetNearZ() const;
	float GetFarZ() const;
	float GetAspect() const;
	float GetFovY() const;
	float GetFovX() const;

	// Get near and far plane dimensions in view space coordinates
	float GetNearWindowWidth() const;
	float GetNearWindowHeight() const;
	float GetFarWindowWidth() const;
	float GetFarWindowHeight() const;

	// Set frustum
	void SetLens(float fovY, float aspect, float zn, float zf);

	// Define camera space via LookAt parameters
	void LookAt(FXMVECTOR pos, FXMVECTOR target, FXMVECTOR worldUp);
	void LookAt(const XMFLOAT3& pos, const XMFLOAT3& target, const XMFLOAT3& up);

	// Get View/Proj matrices
	XMMATRIX View() const;
	XMMATRIX Proj() const;
	XMMATRIX ViewProj() const;

	// Set View/Proj matrices
	void SetProj(XMMATRIX val) { XMStoreFloat4x4(&mProj, val); }

	int GetIndex() const { return mIndex; }
	void SetIndex(int index) { mIndex = index; }

	// Strafe/Walk the camera a distance d
	void Strafe(float d);
	void Walk(float d);
	void WorldUpDown(float d);

	// Rotate the camera
	void Pitch(float angle);
	void RotateY(float angle);

	// After modifying camera position/orientation, call to rebuild the view matrix
	void UpdateViewMatrix();

private:
	// Camera coordinate system with coordinates relative to world space
	XMFLOAT3 mPosition;
	XMFLOAT3 mRight;
	XMFLOAT3 mUp;
	XMFLOAT3 mLook;

	XMFLOAT3 mRotation;

	// Cache frustum properties
	float mNearZ;
	float mFarZ;
	float mAspect;
	float mFovY;
	float mNearWindowHeight;
	float mFarWindowHeight;

	// Cache View/Proj matrices
	XMFLOAT4X4 mView;
	XMFLOAT4X4 mProj;

	int mIndex;
};