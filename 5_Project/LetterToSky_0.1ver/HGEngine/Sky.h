#pragma once
#include "DirectXDefine.h"

class D3Camera;

/// <summary>
/// Sky Box
/// 
/// 2021. 07. 28 Hacgeum
/// </summary>
class Sky
{
public:
	Sky(ID3D11Device* device, const std::wstring& cubemapFilename, float skySphereRadius);
	~Sky();

	ID3D11ShaderResourceView* CubeMapSRV();

	void Draw(ID3D11DeviceContext* dc, const D3Camera& camera);

private:
	Sky(const Sky& rhs);
	Sky& operator=(const Sky& rhs);

private:
	ID3D11Buffer* mVB;
	ID3D11Buffer* mIB;

	ID3D11ShaderResourceView* mCubeMapSRV;
	//ID3D11ShaderResourceView* mCubeMapSRV2;

	UINT mIndexCount;
};

