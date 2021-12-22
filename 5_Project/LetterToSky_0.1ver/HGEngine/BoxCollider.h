#pragma once
#include "AlignedAllocationPolicy.h"
#include "Transform.h"
#include "DirectXDefine.h"
#include "Patch_Chapters.h"
#include "LightHelper.h"
#include "Vertex.h"

class DXDevice;
class D3Camera;

/// <summary>
/// Box Collider 클래스
/// 
/// 2021. 08. 09 Hacgeum
/// </summary>
class BoxCollider : public AlignedAllocationPolicy<16>, public Transform
{
public:
	BoxCollider(DXDevice* pDevice);
	~BoxCollider();

	void Initialize(float width, float height, float depth);
	void Initialize(Vector3 dimension);
	void Update(D3Camera* pCamera);
	void Render();
	bool CheckCollided(BoxCollider* pBox);

	DirectX::BoundingBox GetBoundingBox() const { return m_BoundingBox; }
	DirectX::BoundingBox GetBBForCollisionDetection() const { return m_BBForCollisionDetection; }

	// Update()
private:
	void BuildBoxColliderGeometryBuffers(float width, float height, float depth);
	void UpdateBoundingBox();

private:
	DXDevice* m_pDXDevice;

	ID3D11Device* md3dDevice;						// D3D11 디바이스
	ID3D11DeviceContext* md3dImmediateContext;		// 디바이스 컨텍스트
	ID3D11RasterizerState* mRenderstate;

	D3D11_PRIMITIVE_TOPOLOGY mTopology;

	XMMATRIX mView;
	XMMATRIX mProj;

	Material mMaterial;
	ID3D11ShaderResourceView* m_BasicMap;
	XMMATRIX m_TexTransform;

	DirectionalLight mDirLights[3];
	XMFLOAT3 mEyePosW;

	ID3D11Buffer* m_VB;
	ID3D11Buffer* m_IB;
	UINT m_IndexCount;

	int m_VertexOffset;
	UINT m_IndexOffset;

	DirectX::BoundingBox m_BoundingBox;				// 원본 컬라이더
	DirectX::BoundingBox m_BBForCollisionDetection;	// Transform을 반영한 바운딩 박스
};