#pragma once
#include "IDXObjectBase.h"
#include "LightHelper.h"

struct ASEObjectMesh
{
	ID3D11Buffer* VB;
	ID3D11Buffer* IB;
	UINT IndexCount;
	Mesh* pMesh;
};

class D3Camera;
class BoxCollider;

/// <summary>
/// Mesh Object
/// 
/// ASE File을 Parsing하여 표현한 Object
/// 2021. 06. 28 Hacgeum
/// </summary>
class ASEObject : public IDXObjectBase
{
public:
	ASEObject(DXDevice* pDevice);
	virtual ~ASEObject();

	virtual void Update(GameTimer* pTimer = nullptr, D3Camera* pCamera = nullptr) override;
	virtual void Render() override;
	virtual void LoadGeometry(CASEParser* m_pASEParser) override;
	virtual void LoadTextureFile(const wchar_t* TextureFile) override;
	virtual void Finalize() override;

private:
	XMMATRIX mTexTransform;

	ID3D11Buffer* m_VB;
	ID3D11Buffer* m_IB;
	UINT m_IndexCount;

	int mVertexOffset;
	UINT mIndexOffset;

	vector<Mesh*> mMeshList;
	vector<ASEObjectMesh*> mASEObjectMesh;

	int m_MaxParentMeshCount;				// 메시들의 부모의 최대 갯수

	ID3D11ShaderResourceView* mDiffuseMapSRV;

	DirectionalLight m_DirLights[3];
	UINT m_LightCount;
};
