#pragma once
#include "VertexColorObject.h"

/// <summary>
/// Topology Type이 LINELIST인 Helper Object 클래스
/// 
/// VertexColorObject 클래스를 상속받는다
/// 2021. 07. 16 Hacgeum
/// </summary>
class HelperObject : public VertexColorObject
{
public:
	HelperObject(DXDevice* pDevice);
	~HelperObject();
};
