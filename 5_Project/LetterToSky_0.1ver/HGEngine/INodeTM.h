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
	/// �̸�, �θ� ���
	std::string	m_name;					// �̸�
	std::string	m_nodeparent;			// �θ� ����� �̸�
	bool m_isparentexist;				// �θ� �ִ°�?
	INodeTM* m_parentspointer;			// �θ� ����� ������

	/// ��ȯ, ���
	Matrix m_NodeTM;					// ���ص� ���� ��ģ ��
	Matrix m_NodeTM2;					// ���� ������, row0123�� ������ ��

	// NodeTM ���� (ASE�Ľ̽� �ʱⰪ)
	XMFLOAT3	m_vec_pos;				// ��ġ ����
	XMFLOAT3	m_vec_rot;				// �޽��� ȸ���� ����
	float m_vec_rotangle;				// �� ȸ���� ��
	XMFLOAT3	m_vec_scale;			// �޽��� ������ ����
	XMFLOAT3	m_vec_scaleaxis;		// ������ ��
	float m_vec_scaleangle;				// ����

	// AnimationTM
	Matrix m_LocalTM;					// ���� ��ȯ ��� (�θ���� ����)
	XMFLOAT3 m_LocalTM_pos;				// 
	Quaternion m_LocalTM_rot;			// �޽��� ȸ�� ����(���ʹϾ�)
	XMFLOAT3 m_LocalTM_scl;				// �޽��� ������ ����
	Matrix m_WorldTM;					// ��������� �� �޽ð� ���� �� ���� ���� ��ȯ ���

	// MoveTM
	XMFLOAT4X4 m_MoveTM;				// ����ڰ� �Է��ؼ� ���� �̵�/��ȯ ���
	XMFLOAT3 m_MoveTMpos;				// �̵� ����
	XMFLOAT3 m_MoveTMrot;				// ȸ�� ����
	float m_MoveTMrot_speed;			// ȸ�� �ӵ�
	float m_MoveTMmov_speed;			// �̵� �ӵ�

	/// �ִϸ��̼� ������
	bool m_isExistAnimation;			// �ִϸ��̼� �����Ͱ� �ִ°�?

	float m_elapsedFrame;				// �ð� �������� ����� ���� ������(float) (�Ʒ��� ������������ �������� ���)
	int m_nowframe;						// ���� ������
	LONG m_nowticks;					// ���� Ticks

	LONG m_minKeyframe;					// �ּ� Ű������
	Animation* m_pAnimationTable;		// �ִϸ��̼� �������� ���� ���̺�
	Scenedata* m_pSceneData;			// ASE������ Scene������

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
	//bool SetType(eObjectType type);	// ������Ʈ�� Ÿ���� �Ҵ����ش�. �׷��� �ܺο��� Ÿ���� �������� ���� ó�� �����.
	bool SetLocalTM(Matrix LocalTM);
	//bool SetPos( XMFLOAT3 position);
	bool SetParentsPointer(INodeTM* parentmesh);
	bool SetAnimationData(Animation* animationdata);
	//--------------------------------------------------
	// Is Do Something?
	//bool GetIsParentExist();
	bool Check_IsparentExist();

	//--------------------------------------------------
	// ObjectRoot�� ����� ������Ʈ���� �ʱ�ȭ�� ���� ���̴� �޼ҵ��
	//bool GetDevice(LPDIRECT3DDEVICE9	pd3dDevice);
	bool GetScenedata(Mesh* ponemesh);
	//bool MakeNodeTM(COneMesh* ponemesh);				// NodeTM ���ϱ�
	//virtual bool AnimateFrames();						// �ִϸ��̼��� ���� tick�� ����Ѵ�
	//bool MakeAnimationTM(CScenedata* worldSceneData);

	/// TM���� �޼ҵ�
	// ����TM�� �� ��ҵ��� �и�, ����
	bool Decompose_LocalTM();
	// �θ���� ��� LocalTM�� ���ؼ� �ڽ��� WorldTM�� ���Ѵ�.
	bool MakeWorldTM();

	//--------------------------------------------------
	// ���� ������ �׸��� ���� ���Ǵ� �޼ҵ��
	// ��� ���� ����ڰ� �Է��� ��ȯ ����� �������ش�.
	bool MakeMoveTM();
	bool InputMoveTM(const XMFLOAT4X4& InputMoveTM);	// �̵� ��� ��ü�� �޴´�
	//bool Move_left();									// �̰͵��� �̵� �޼ҵ�
	//bool Move_right();
	//bool Move_up();
	//bool Move_down();
	//bool Move_front();
	//bool Move_back();

	//bool Rotate( XMFLOAT3 rotatevector );

	///--------------------------------------------------
	///	�ִϸ��̼� ����
	///--------------------------------------------------
public:
	bool AnimateFrames(float deltaTime);
	bool MakeAnimationTM(Scenedata* worldSceneData);

public:
	bool GetNodeTM_FromMesh(Mesh* pMesh);

	/// �� �������� (DX11)
	/// ASE NodeTM �����Ϳ� �ǹ� ���� ���� ��� �ִ� ��찡 �ִµ�,
	/// DX11�� XMMatrixRotationAxis()�� ��� ������ 0�̸� ���ܸ� ��´�.
	/// DX9�� D3DXMatrixRotationAxis�� �׳� ��������� �������ش�.
	/// ������ �̷� ��� 1,0,0 �࿡ 0���� ������ ������ش�.
	void FixAxis(Vector3& axis, float& angle);
};
