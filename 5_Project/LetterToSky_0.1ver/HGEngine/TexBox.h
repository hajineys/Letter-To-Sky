#pragma once
#include "TextureObject.h"

/// <summary>
/// Texture ������ 3D �⺻ �ڽ�
/// 
/// Direct3D 11 ��å ����
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
