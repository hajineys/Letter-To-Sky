#pragma once
#include "IDXObjectBase.h"

/// <summary>
/// Vertex Color Object
/// 
/// Position과 Color값으로 구성되어있다
/// 2021. 06. 09 Hacgeum
/// </summary>
class VertexColorObject : public IDXObjectBase
{
public:
	VertexColorObject(DXDevice* pDevice);
	virtual ~VertexColorObject();

public:
	virtual void Render();

private:
	virtual void LoadGeometry(SGeoBuffer* geoBuffer) override;

protected:
	ID3D11Buffer* m_VB;
	ID3D11Buffer* m_IB;
	UINT m_IndexCount;
};
