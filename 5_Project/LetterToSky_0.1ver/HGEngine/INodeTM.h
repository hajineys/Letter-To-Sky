#pragma once
#include "AlignedAllocationPolicy.h"
#include "DirectXDefine.h"
#include "ParsingData.h"
#include "Animation.h"
#include "../DXTK/Inc/SimpleMath.h"
using namespace DirectX::SimpleMath;
using namespace ASEParser;

struct ASEParser::Mesh;

class INodeTM : public AlignedAllocationPolicy<16>
{
public:
	INodeTM();
	virtual ~INodeTM();

protected:
	//--------------------------------------------------
	// Variables
	//--------------------------------------------------
	/// 이름, 부모 노드
	std::string	m_name;					// 이름
	std::string	m_nodeparent;			// 부모 노드의 이름
	bool m_isparentexist;				// 부모가 있는가?
	INodeTM* m_parentspointer;			// 부모 노드의 포인터

	/// 변환, 행렬
	Matrix m_NodeTM;					// 분해된 값을 합친 것
	Matrix m_NodeTM2;					// 위와 같지만, row0123을 조립한 것

	// NodeTM 관련 (ASE파싱시 초기값)
	XMFLOAT3	m_vec_pos;				// 위치 정보
	XMFLOAT3	m_vec_rot;				// 메시의 회전축 정보
	float m_vec_rotangle;				// 그 회전의 각
	XMFLOAT3	m_vec_scale;			// 메시의 스케일 정보
	XMFLOAT3	m_vec_scaleaxis;		// 스케일 축
	float m_vec_scaleangle;				// 각도

	// AnimationTM
	Matrix m_LocalTM;					// 로컬 변환 행렬 (부모와의 차이)
	XMFLOAT3 m_LocalTM_pos;				// 
	Quaternion m_LocalTM_rot;			// 메시의 회전 정보(쿼터니언)
	XMFLOAT3 m_LocalTM_scl;				// 메시의 스케일 정보
	Matrix m_WorldTM;					// 결론적으로 이 메시가 찍힐 때 사용될 최종 변환 행렬

	// MoveTM
	XMFLOAT4X4 m_MoveTM;				// 사용자가 입력해서 만든 이동/변환 행렬
	XMFLOAT3 m_MoveTMpos;				// 이동 벡터
	XMFLOAT3 m_MoveTMrot;				// 회전 벡터
	float m_MoveTMrot_speed;			// 회전 속도
	float m_MoveTMmov_speed;			// 이동 속도

	/// 애니메이션 데이터
	bool m_isExistAnimation;			// 애니메이션 데이터가 있는가?

	float m_elapsedFrame;				// 시간 기준으로 계산한 진행 프레임(float) (아래의 현재프레임을 결정짓는 요소)
	int m_nowframe;						// 현재 프레임
	LONG m_nowticks;					// 현재 Ticks

	LONG m_minKeyframe;					// 최소 키프레임
	Animation* m_pAnimationTable;		// 애니메이션 데이터의 실제 테이블
	Scenedata* m_pSceneData;			// ASE파일의 Scene데이터

public:
	//--------------------------------------------------
	// Get Variable
	//int GetType();
	//bool GetIsExistAnimation();
	//CAnimation* GetAnimationTable();
	std::string& GetParentsName();
	std::string& GetNodeName();
	
	Matrix GetNodeTM();
	//XMFLOAT4X4& GetNodeTM2();
	Matrix GetLocalTM();
	//XMFLOAT4X4& GetWorldTM();
	
	Matrix* GetNodeTM_p();
	//XMFLOAT4X4* GetLocalTM_p();
	Matrix* GetWorldTM_p();
	
	//XMFLOAT3& GetLocalTM_pos();
	//D3DXQUATERNION& GetLocalTM_rot();
	//XMFLOAT3& GetLocalTM_scl();
	
	//--------------------------------------------------
	// Set Variable
	//bool SetName(std::string& name);
	//bool SetType(eObjectType type);	// 오브젝트의 타입을 할당해준다. 그런데 외부에서 타입을 설정해줄 일이 처음 생겼다.
	bool SetLocalTM(Matrix LocalTM);
	//bool SetPos( XMFLOAT3 position);
	bool SetParentsPointer(INodeTM* parentmesh);
	bool SetAnimationData(Animation* animationdata);
	//--------------------------------------------------
	// Is Do Something?
	//bool GetIsParentExist();
	bool Check_IsparentExist();

	//--------------------------------------------------
	// ObjectRoot를 계승한 오브젝트들의 초기화를 위해 쓰이는 메소드들
	//bool GetDevice(LPDIRECT3DDEVICE9	pd3dDevice);
	bool GetScenedata(Mesh* ponemesh);
	//bool MakeNodeTM(COneMesh* ponemesh);				// NodeTM 구하기
	//virtual bool AnimateFrames();						// 애니메이션의 현재 tick을 계산한다
	//bool MakeAnimationTM(CScenedata* worldSceneData);

	/// TM관련 메소드
	// 로컬TM의 각 요소들을 분리, 저장
	bool Decompose_LocalTM();
	// 부모들의 모든 LocalTM을 곱해서 자신의 WorldTM을 구한다.
	bool MakeWorldTM();

	//--------------------------------------------------
	// 루프 내에서 그리기 전에 사용되는 메소드들
	// 찍기 전에 사용자가 입력한 변환 행렬을 셋팅해준다.
	bool MakeMoveTM();
	bool InputMoveTM(const XMFLOAT4X4& InputMoveTM);	// 이동 행렬 자체를 받는다
	//bool Move_left();									// 이것들은 이동 메소드
	//bool Move_right();
	//bool Move_up();
	//bool Move_down();
	//bool Move_front();
	//bool Move_back();

	//bool Rotate( XMFLOAT3 rotatevector );

	///--------------------------------------------------
	///	애니메이션 관련
	///--------------------------------------------------
public:
	bool AnimateFrames(float deltaTime);
	bool MakeAnimationTM(Scenedata* worldSceneData);

public:
	bool GetNodeTM_FromMesh(Mesh* pMesh);

	/// 축 에러보정 (DX11)
	/// ASE NodeTM 데이터에 의미 없는 값이 들어 있는 경우가 있는데,
	/// DX11의 XMMatrixRotationAxis()는 축과 각도가 0이면 예외를 뱉는다.
	/// DX9의 D3DXMatrixRotationAxis는 그냥 단위행렬을 리턴해준다.
	/// 때문에 이런 경우 1,0,0 축에 0도인 각으로 만들어준다.
	void FixAxis(Vector3& axis, float& angle);
};
