#pragma once
#include "IDXObjectBase.h"
#include "LightHelper.h"

class GameTimer;
class D3Camera;

/// <summary>
/// Texture Object
/// 
/// Position과 Normal, Texture의 값으로 구성되어있다
/// 2021. 06. 17 Hacgeum
/// </summary>
class TextureObject : public IDXObjectBase
{
public:
	TextureObject(DXDevice* pDevice);
	virtual ~TextureObject();

	virtual void Render();

private:
	virtual void LoadGeometry(SGeoBuffer* geoBuffer) override;
	virtual void LoadTextureFile(const wchar_t* TextureFile) override;

protected:
	ID3D11Buffer* m_VB;
	ID3D11Buffer* m_IB;
	UINT m_IndexCount;

	ID3D11ShaderResourceView* m_DiffuseMapSRV;
	XMMATRIX m_TexTransform;

	int m_VertexOffset;
	UINT m_IndexOffset;
};
