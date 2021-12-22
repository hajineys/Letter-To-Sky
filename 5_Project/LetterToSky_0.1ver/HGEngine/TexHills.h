#pragma once
#include "TextureObject.h"

/// <summary>
/// Texture 적용한 3D 언덕
/// 
/// Direct3D 11 용책 예제
/// 2021. 06. 14 Hacgeum
/// </summary>
class TexHills : public TextureObject
{
public:
	TexHills(DXDevice* pDevice);
	~TexHills();

	float GetHillHeight(float x, float z)const;
	XMFLOAT3 GetHillNormal(float x, float z)const;
};
