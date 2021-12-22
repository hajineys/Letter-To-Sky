#pragma once
#include "../DXTK/Inc/SimpleMath.h"
using namespace DirectX;
using namespace DirectX::SimpleMath;

/// <summary>
/// Object들의 Position, Rotation, Scale을 조정하는 클래스
/// 
/// 2021. 06. 10 Hacgeum
/// </summary>
class Transform
{
public:
	Transform();
	~Transform();

	void UpdateWorldTM();
	void Strafe(float start, float end, float value);
	void StrafeInverse(float start, float end, float value);
	void StrafeDiagonal(float startX, float endX, float startY, float endY, float startZ, float endZ, float dTime);
	void MoveUpDown(float start, float end, float value);
	void MoveFrontBack(float start, float end, float value);
	void Fall(float dTime);

	// Getter / Setter
	XMMATRIX GetWorldTM() const { return m_WorldTM; }
	void SetWorldTM(XMMATRIX world) { m_WorldTM = world; }

	XMFLOAT3 GetPosition() const { return m_Position; }
	void SetPosition(float posX, float posY, float posZ);
	void AddPosition(float x, float y, float z);

	XMMATRIX GetTranslation() const { return m_TranslationTM; }
	void SetTranslation(float dx, float dy, float dz);

	void SetRotation(char axis, float angle);

	void SetScale(float px, float py, float pz);

protected:
	XMMATRIX m_WorldTM;

private:
	XMFLOAT3 m_Position;

	XMMATRIX m_TranslationTM;
	XMMATRIX m_RotationTM;
	XMMATRIX m_ScaleTM;

	float m_Angle;

	bool m_Forward;
	bool m_Backward;

	bool m_Forward1;

	float m_Weight;

	// Lerp Animation
	float m_Per;
	bool m_IsLerping;
	Vector3 m_StartPos;
	Vector3 m_EndPos;
	Vector3 m_NowPos;
};