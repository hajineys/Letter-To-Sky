#pragma once
#include "AlignedAllocationPolicy.h"
#include "Transform.h"
#include "DirectXDefine.h"
#include "ASEParser.h"
#include "LightHelper.h"

class DXDevice;
class DXGeometryBuilder;
class GameTimer;
class D3Camera;
struct SGeoBuffer;
class BoxCollider;

/// <summary>
/// 모든 Object의 부모 클래스
/// 
/// 2021. 06. 23 Hacgeum
/// </summary>
class IDXObjectBase : public AlignedAllocationPolicy<16>, public Transform
{
public:
	IDXObjectBase(DXDevice* pDevice);
	virtual ~IDXObjectBase();

	virtual void Initialize();
	virtual void Initialize(int bitmapWidth, int bitmapHeight) {}

	virtual void LoadGeometry(SGeoBuffer* geoBuffer) {}
	virtual void LoadGeometry(CASEParser* m_pASEParser) {}
	virtual void LoadTextureFile(const wchar_t* TextureFile) {}

	virtual void Update(GameTimer* pTimer = nullptr, D3Camera* pCamera = nullptr);
	virtual void Render();

	virtual void SetBitmapPosition(int x, int y) {}
	virtual void SetBoxColliderScale(float width = 5.0f, float height = 5.0f, float depth = 5.0f);

	BoxCollider* GetBoxCollider() const { return m_pBoxCollider; }
	///void SetBoxColliderPos(Vector3 pos) { m_pBoxCollider->SetPosition(pos); }

	virtual void Finalize();

	virtual void CheckUIPick(int x, int y) {};
	virtual void CheckUICollided(int x, int y) {};

	virtual bool GetPick() const { return m_IsPicked; }
	virtual bool GetCollided() const { return m_IsCollided; }

protected:
	ID3D11Device* md3dDevice;						// D3D11 디바이스
	ID3D11DeviceContext* md3dImmediateContext;		// 디바이스 컨텍스트
	ID3D11RasterizerState* mRenderstate;

	D3D11_PRIMITIVE_TOPOLOGY mTopology;

	XMMATRIX mView;
	XMMATRIX mProj;

	Material mMaterial;

	XMFLOAT3 mEyePosW;

	BoxCollider* m_pBoxCollider;

	bool m_IsPicked;
	bool m_IsCollided;
	bool m_DebugBoxOn;

public:
	/// (추가필요)나 자신이 누구인지 좀 알자
	//eDXObjectType m_DXOType;

	/// (추가필요)렌더링 우선순위
	//int m_RenderingPriority;
};
