#pragma once
#include "IDXObjectBase.h"

/// <summary>
/// 2D Image Å¬·¡½º
/// 
/// 2021. 07. 28 Hacgeum
/// </summary>
class BitmapObject : public IDXObjectBase
{
public:
	BitmapObject(DXDevice* pDevice);
	virtual ~BitmapObject();

	virtual void Initialize(int bitmapWidth, int bitmapHeight) override;

	virtual void LoadGeometry(SGeoBuffer* geoBuffer) override;
	virtual void LoadTextureFile(const wchar_t* TextureFile) override;

	virtual void Update(GameTimer* pTimer = nullptr, D3Camera* pCamera = nullptr) override;
	virtual void Render() override;

	virtual void SetBitmapPosition(int x, int y) override;

	virtual void CheckUIPick(int x, int y) override;
	virtual void CheckUICollided(int x, int y) override;

private:
	DXDevice* m_pDirectX;

	ID3D11Buffer* m_VB;
	ID3D11Buffer* m_IB;
	int m_VertexCount;
	UINT m_IndexCount;

	ID3D11ShaderResourceView* mDiffuseMapSRV;

	int m_ScreenWidth;
	int m_ScreenHeight;

	int m_BitmapWidth;
	int m_BitmapHeight;

	int m_PositionX;
	int m_PositionY;

	int m_PreviousPosX;
	int m_PreviousPosY;
};
