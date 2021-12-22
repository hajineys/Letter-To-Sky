#pragma once
#include "TextureObject.h"

/// <summary>
/// Texture 적용한 3D 기본 박스
/// 
/// Direct3D 11 용책 예제
/// 2021. 06. 14 Hacgeum
/// </summary>
class TexBox : public TextureObject
{
public:
	TexBox(DXDevice* pDevice);
	~TexBox();

	virtual void Initialize();

private:
	void BuildGeometryBuffers();
};
