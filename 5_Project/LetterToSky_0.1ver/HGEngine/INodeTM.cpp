#include "INodeTM.h"

INodeTM::INodeTM()
{
	/// Ÿ��, �̸�, �θ� ���
	//std::string m_name;		// �̸�
	//std::string m_nodeparent;	// �θ� ����� �̸�
	m_isparentexist = false;	// �θ� �ִ°�?
	m_parentspointer = NULL;	// �غθ� ����� ������

	/// ��ȯ, ���
	m_NodeTM = Matrix::Identity;
	m_NodeTM2 = Matrix::Identity;

	// NodeTM ���� (ASE�Ľ̽� �ʱⰪ)
	m_vec_pos = XMFLOAT3(0, 0, 0);
	m_vec_rot = XMFLOAT3(0, 0, 0);
	m_vec_rotangle = 0.0f;
	m_vec_scale = XMFLOAT3(0, 0, 0);
	m_vec_scaleaxis = XMFLOAT3(0, 0, 0);
	m_vec_scaleangle = 0.0f;

	// AnimationTM
	m_LocalTM = Matrix::Identity;
	m_LocalTM_pos = XMFLOAT3(0, 0, 0);
	m_LocalTM_rot = Quaternion(0, 0, 0, 0);
	m_LocalTM_scl = XMFLOAT3(1, 1, 1);
	m_WorldTM = Matrix::Identity;

	// MoveTM
	m_MoveTM = Matrix::Identity;
	m_MoveTMpos = XMFLOAT3(0, 0, 0);
	m_MoveTMrot = XMFLOAT3(0, 0, 0);
	m_MoveTMrot_speed = 0.0f;
	m_MoveTMmov_speed = 0.0f;

	/// �ִϸ��̼� ������
	m_isExistAnimation = false;	// �⺻������ ������Ʈ�� �ִϸ��̼��� ���� ������ �����Ѵ�
	m_elapsedFrame = 0.0f;
	m_nowframe = 0;				// ���� ������
	m_nowticks = 0;				// ���� Ticks
	m_minKeyframe = 192;		// �밡���� 800, Joe�� 192��� �ִϸ��̼Ǹ��� �ٸ���.

	m_pAnimationTable = NULL;	// Parser���� �޾ƿ´�
	m_pSceneData = NULL;		// ��������
}

INodeTM::~INodeTM()
{

}

bool INodeTM::GetNodeTM_FromMesh(Mesh* pMesh)
{
	//------------------------------------------------------------------------------------
	//	�̸�, �θ� �� ��������
	//------------------------------------------------------------------------------------
	m_name = pMesh->m_nodename;						// �̸�
	m_nodeparent = pMesh->m_nodeparent;				// �θ�
	m_isparentexist = pMesh->m_isparentexist;		// �θ��� ���� ����
	m_isExistAnimation = pMesh->m_isAnimated;		// �ִϸ��̼ǵ����� ���� ����

	///--------------------
	/// NodeTM1
	///--------------------
	m_vec_pos = pMesh->m_tm_pos;
	m_vec_rot = pMesh->m_tm_rotaxis;
	m_vec_rotangle = pMesh->m_tm_rotangle;
	m_vec_scale = pMesh->m_tm_scale;
	m_vec_scaleaxis = pMesh->m_tm_scaleaxis;
	m_vec_scaleangle = pMesh->m_tm_scaleaxisang;

	FixAxis(pMesh->m_tm_rotaxis, pMesh->m_tm_rotangle);
	FixAxis(pMesh->m_tm_scaleaxis, pMesh->m_tm_scaleaxisang);

	/// ���� ���� ���ļ� NodeTM�� �����
	XMMATRIX _posTM;
	XMMATRIX _rotTM;
	XMMATRIX _sclTM;

	// ������
	_posTM = XMMatrixTranslation(pMesh->m_tm_pos.x, pMesh->m_tm_pos.y, pMesh->m_tm_pos.z);

	// �����̼�
	//D3DXQUATERNION __tempQT;	// ���ʹϾ����� �����ô�.
	//D3DXQuaternionRotationAxis(&__tempQT, &ponemesh->m_tm_rotaxis, ponemesh->m_tm_rotangle);
	//XMMatrixRotationQuaternion(&_RotTM, &__tempQT);
	_rotTM = XMMatrixRotationAxis(pMesh->m_tm_rotaxis, pMesh->m_tm_rotangle);

	// ������
	_sclTM = XMMatrixScaling(pMesh->m_tm_scale.x, pMesh->m_tm_scale.y, pMesh->m_tm_scale.z);

	XMMATRIX _tempTM;
	XMMATRIX _tempTM2;
	_tempTM = XMMatrixRotationAxis(pMesh->m_tm_scaleaxis, pMesh->m_tm_scaleaxisang);
	_tempTM2 = XMMatrixInverse(nullptr, _tempTM);
	//_SclTM = __tempTM2 * _SclTM * __tempTM;

	_sclTM = XMMatrixMultiply(_tempTM2, _sclTM);
	_sclTM = XMMatrixMultiply(_sclTM, _tempTM);
	/// ���̰� ��������� �ʿ��ߴ� NodeTM.
	m_NodeTM = _sclTM * _rotTM * _posTM;

	///--------------------
	/// NodeTM2
	///--------------------
	/// �ϴ� ��ȯ ����� ���ؼ� 4x4�� ������ش�.
	m_NodeTM2._11 = pMesh->m_tm_row0.x;
	m_NodeTM2._12 = pMesh->m_tm_row0.y;
	m_NodeTM2._13 = pMesh->m_tm_row0.z;
	m_NodeTM2._14 = 0.0f;
	m_NodeTM2._21 = pMesh->m_tm_row1.x;
	m_NodeTM2._22 = pMesh->m_tm_row1.y;
	m_NodeTM2._23 = pMesh->m_tm_row1.z;
	m_NodeTM2._24 = 0.0f;
	m_NodeTM2._31 = pMesh->m_tm_row2.x;
	m_NodeTM2._32 = pMesh->m_tm_row2.y;
	m_NodeTM2._33 = pMesh->m_tm_row2.z;
	m_NodeTM2._34 = 0.0f;
	m_NodeTM2._41 = pMesh->m_tm_row3.x;
	m_NodeTM2._42 = pMesh->m_tm_row3.y;
	m_NodeTM2._43 = pMesh->m_tm_row3.z;
	m_NodeTM2._44 = 1.0f;

	// �嵥 �׳� �����ϸ� D3D������ ������
	// �� ������ �̷��� ������ ��ȯ�ؼ� �о�� ��
	swap(m_NodeTM2._12, m_NodeTM2._13);
	swap(m_NodeTM2._22, m_NodeTM2._23);
	swap(m_NodeTM2._32, m_NodeTM2._33);
	swap(m_NodeTM2._42, m_NodeTM2._43);

	swap(m_NodeTM2._21, m_NodeTM2._31);
	swap(m_NodeTM2._22, m_NodeTM2._32);
	swap(m_NodeTM2._23, m_NodeTM2._33);
	swap(m_NodeTM2._24, m_NodeTM2._34);

	///--------------------
	/// Negative Scale ���� �õ�.
	/// NodeTM1�� ���� ���, 2���� �񱳸� ���ؼ� ����
	///--------------------
	{
		float dot1, dot2, dot3;
		Vector3 tempVec1, tempVec2;
		tempVec1 = Vector3(m_NodeTM2._11, m_NodeTM2._21, m_NodeTM2._31);
		tempVec2 = Vector3(m_NodeTM._11, m_NodeTM._21, m_NodeTM._31);
		dot1 = tempVec1.Dot(tempVec2);
		tempVec1 = Vector3(m_NodeTM2._12, m_NodeTM2._22, m_NodeTM2._32);
		tempVec2 = Vector3(m_NodeTM._12, m_NodeTM._22, m_NodeTM._32);
		dot2 = tempVec1.Dot(tempVec2);
		tempVec1 = Vector3(m_NodeTM2._13, m_NodeTM2._23, m_NodeTM2._33);
		tempVec2 = Vector3(m_NodeTM._13, m_NodeTM._23, m_NodeTM._33);
		dot3 = tempVec1.Dot(tempVec2);

		if (dot1 < 0 || dot2 < 0 || dot3 < 0)
		{
			// �װ�Ƽ�� �������� ���
			// �븻�� �� �������
			// �ٺ����� �ذ�å�� ���Ǹ� Cannon�� ������ �������� ������ ������ �ִ�

			for (unsigned int i = 0; i < pMesh->m_opt_vertex.size(); i++)
			{
				pMesh->m_opt_vertex[i]->m_normal.x = -(pMesh->m_opt_vertex[i]->m_normal.x);
				pMesh->m_opt_vertex[i]->m_normal.y = -(pMesh->m_opt_vertex[i]->m_normal.y);
				pMesh->m_opt_vertex[i]->m_normal.z = -(pMesh->m_opt_vertex[i]->m_normal.z);
			}

			/// ������ ��ü�� ������ �ʿ䰡 ����. (decomposed data�� ���� ���)
			//m_NodeTM2._11 *= -1.0f;
			//m_NodeTM2._22 *= -1.0f;
			//m_NodeTM2._33 *= -1.0f;
		}
	}

	///--------------------
	/// �� ��°
	/// Negative Scale ���� �õ�.
	/// NodeTM2�� ���� ���. ��ü������ cross�ϰ� �� ��° ���Ϳ� ��
	///--------------------
	{
		/*
		Vector3 tempVec1, tempVec2, tempVec3;
		tempVec1 = Vector3(m_NodeTM2._11, m_NodeTM2._12, m_NodeTM2._13);
		tempVec2 = Vector3(m_NodeTM2._21, m_NodeTM2._22, m_NodeTM2._23);
		tempVec3 = Vector3(m_NodeTM2._31, m_NodeTM2._32, m_NodeTM2._33);

		XMVECTOR cross = XMVector3Cross(tempVec1, tempVec2);

		XMVECTOR dot = XMVector3Dot(cross, tempVec3);

		XMFLOAT3 dotfloat;

		XMStoreFloat3(&dotfloat, dot);

		if (dotfloat.x < 0)
		{
			XMVECTOR pos;
			XMVECTOR rot;
			XMVECTOR scl;

			XMMatrixDecompose(&scl, &rot, &pos, m_NodeTM2);

			scl = scl * -1.0f;

			m_NodeTM2 = XMMatrixScalingFromVector(scl) * XMMatrixRotationQuaternion(rot) * XMMatrixTranslationFromVector(pos);

			// �븻�� ������ (�� �� ������) �������ش�.
			for (unsigned int i = 0; i < pMesh->m_opt_vertex.size(); i++)
			{
				pMesh->m_opt_vertex[i]->m_normal.x = -(pMesh->m_opt_vertex[i]->m_normal.x);
				pMesh->m_opt_vertex[i]->m_normal.y = -(pMesh->m_opt_vertex[i]->m_normal.y);
				pMesh->m_opt_vertex[i]->m_normal.z = -(pMesh->m_opt_vertex[i]->m_normal.z);
			}
		}
		*/
	}

	/// �� ������ ���� ������ �� �� �ִ�.
	//m_NodeTM = m_NodeTM2;

	return true;
}


void INodeTM::FixAxis(Vector3& axis, float& angle)
{
	if (axis.x == 0 && axis.y == 0 && axis.z == 0)
	{
		axis.x = 1.0f;
		axis.y = 0;
		axis.z = 0;
		angle = 0;
	}
}

bool INodeTM::MakeMoveTM()
{
	//m_WorldTM = m_WorldTM * m_MoveTM;

	/*XMFLOAT4X4 tempTM;					// ���������� ����� ��Ʈ����.

	//---------------------------------------------------------------------------------------------------------------
	// ���ؽ����� �����̼� ��Ʈ������ ���Ѵ�.
	// 1. ���� �����ʹ� ���� �������� �Ǿ� �ִ�.
	// m_axi_pos�� ���� ��ġ�� ��Ÿ���� ��
	XMFLOAT4X4 rotateTM;				// ȸ���� ��Ʈ����

	XMMatrixIdentity(&rotateTM);		// ȸ���� ��Ʈ����
	XMMatrixIdentity(&tempTM);		// �ӽÿ� ��Ʈ����

	// (���� ���¿���) �����̼�
	XMMatrixIdentity(&tempTM);
	D3DXMatrixRotationX(&tempTM, m_MoveTMrot.x);
	rotateTM *= tempTM;

	XMMatrixIdentity(&tempTM);
	D3DXMatrixRotationY(&tempTM, m_MoveTMrot.y);
	rotateTM *= tempTM;

	XMMatrixIdentity(&tempTM);
	D3DXMatrixRotationZ(&tempTM, m_MoveTMrot.z);
	rotateTM *= tempTM;

	/// �ѹ��� ������
	//XMMatrixIdentity(&tempTM);
	//D3DXMatrixRotationYawPitchRoll(&tempTM, m_rot.y, m_rot.x, m_rot.z);
	//rotateTM *= tempTM;
	//---------------------------------------------------------------------------------------------------------------

	//---------------------------------------------------------------------------------------------------------------
	// 2. �̹����� �̵� ��ȭ�� ��Ʈ������ ���ϴµ�, ��ǥ�谡 �ڽ��� �����̴�.
	XMFLOAT4X4 moveTM;					// �̵��� ���� ��Ʈ����
	XMMatrixIdentity(&moveTM);
	XMMatrixTranslation( &moveTM, m_MoveTMpos.x, m_MoveTMpos.y, m_MoveTMpos.z);

	XMMatrixIdentity(&tempTM);
	tempTM = rotateTM * moveTM;

	m_LocalTM = tempTM * m_LocalTM;*/
	//---------------------------------------------------------------------------------------------------------------

	return true;
}

bool INodeTM::InputMoveTM(const XMFLOAT4X4& InputMoveTM)
{
	m_MoveTM = InputMoveTM;
	return true;
}


bool INodeTM::MakeWorldTM()
{
	if (m_isparentexist == false)
	{
		// �θ� ���ٸ�? ����� ���ð� ����.
		m_WorldTM = m_LocalTM;
	}
	else
	{
		// �θ� �ִٸ� �ϳ��� ���س����� �ȴ�
		m_WorldTM = m_LocalTM;		/// �̰� ������ �����ڵ��� �� �� �ִ�.

		// �θ� �ִٸ� �θ��� ���� ��ǥ�� ��������
		INodeTM* nowparent;
		nowparent = (INodeTM*)m_parentspointer;

		while (nowparent != NULL)
		{
			m_WorldTM = m_WorldTM * nowparent->GetLocalTM();

			// ���� �θ� �޽��� �����͸� �� ĭ ���� �ø���
			nowparent = (INodeTM*)nowparent->m_parentspointer;
		}
	}

	return true;
}

Matrix INodeTM::GetLocalTM()
{
	return m_LocalTM;
}

DirectX::SimpleMath::Matrix* INodeTM::GetNodeTM_p()
{
	return &m_NodeTM;
}

DirectX::SimpleMath::Matrix* INodeTM::GetWorldTM_p()
{
	return &m_WorldTM;
}

bool INodeTM::Check_IsparentExist()
{
	// �θ���� �༮�̶�� LocalTM�� �ڽ��� WorldTM�� ����.
	if (m_isparentexist == false)
	{
		m_LocalTM = m_NodeTM;
		return false;
	}
	else
	{
		return true;
	}
}

bool INodeTM::Decompose_LocalTM()
{
	XMVECTOR scl;
	XMVECTOR rot;
	XMVECTOR pos;

	XMMatrixDecompose(&scl, &rot, &pos, m_LocalTM);

	XMStoreFloat3(&m_LocalTM_scl, scl);
	XMStoreFloat4(&m_LocalTM_rot, rot);
	XMStoreFloat3(&m_LocalTM_pos, pos);

	return true;
}

std::string& INodeTM::GetParentsName()
{
	return m_nodeparent;
}

std::string& INodeTM::GetNodeName()
{
	return m_name;
}

Matrix INodeTM::GetNodeTM()
{
	return m_NodeTM;
}

bool INodeTM::SetAnimationData(Animation* animationdata)
{
	m_pAnimationTable = animationdata;
	return true;
}

bool INodeTM::SetParentsPointer(INodeTM* parentmesh)
{
	m_parentspointer = parentmesh;
	return true;
}

bool INodeTM::SetLocalTM(Matrix LocalTM)
{
	XMStoreFloat4x4(&m_LocalTM, LocalTM);
	return true;
}

///--------------------------------------------------
///	�ִϸ��̼� ����
///--------------------------------------------------

bool INodeTM::AnimateFrames(float deltaTime)
{
	// �ִϸ��̼��� ���ٸ� �� ������Ʈ�� m_nowticks�� ����
	if (m_pAnimationTable == NULL)
	{
		m_nowticks = 0;
		return false;
	}

	///m_elaspedtime += ((float)m_pSceneData->m_framespeed / 60.0f) * deltaTime;	// ����ؼ� �þ����.

	m_elapsedFrame += deltaTime * (float)m_pSceneData->m_framespeed;
	m_nowframe = (int)m_elapsedFrame;			// float���̹Ƿ� ĳ����
	m_nowticks = m_nowframe * m_pAnimationTable->m_ticksperFrame;

	// �ִϸ��̼� �� ����
	if (m_nowframe > m_pSceneData->m_lastframe)
	{
		m_nowframe = m_pSceneData->m_firstframe;
		m_elapsedFrame = m_nowframe;
		//TRACE("AnimationDataReset %d \n", m_nowframe);
	}

	return true;
}

bool INodeTM::MakeAnimationTM(Scenedata* worldSceneData)
{
	// �ִϸ��̼��� ���ٸ�
	if (m_isExistAnimation == false)
	{
		//��� �� �� �͵� ���� ��� ����� �⺻���� LocalTM�̴�.
		return false;
	}

	//--------------------------------------------------
	// ��ǥ :
	// ������Ʈ ���ο� �ִϸ��̼� ���̺��� ������ �ִ�.
	// �������� ������ �ѹ��� ����, �ִϸ��̼� ������ ���� �����´�.(Position, Rotation, Scale�� ��������)
	// �� �����͸� ������ ���� ��ü�ؼ� �ᱹ �޽� �ϳ��� LocalTM�� �����.
	//--------------------------------------------------

	// ����������
	bool flag_findsamename = false;

	// nowtick
	LONG nowtick = m_nowticks;

	LONG starttick, endtick;
	int lastindex;
	bool flag_findkeyframe = false;

	// SceneData�� �Է¹��� ���ؼ� ���� ������Ʈ�ϼ��� �ִ�.
	// �̷� ��� ����Ʈ ������ SceneData�� ����. ���� ������ ������ �ٲ��� �����Ҽ� ����.
	if (m_pSceneData == NULL)
	{
		m_pSceneData = worldSceneData;
	}

	int ticksperframe = m_pAnimationTable->m_ticksperFrame;

	// ticks per frame�� ������ �ִ�.
	// �Ƹ� �̰��� �ּ��������� ���� �ƴѵ�? ������ �ʿ��� �����ʹ� �ּ� Ű������ ���� ���ε�.
	// ������ζ�� �ּ� Ű�������� ���� ���� �ϴµ� dragon, IK JOE �ּ�Ű������ ���� �ƴϴ�. (�� �۴�-�׷��� �Ҷ� ����� ���� �߻�. �ݴ�� ĳ���� Ű�������� �� ũ�� ��ƹ����� ������ �ƴ϶� ������ �ȴ�..)
	/// �׷��� �ӽ� ����.
	/// �ñ����δ� �ּ� Ű�������� ������ִ� �κ��� �־�߰ڴ�.
	///if (m_pSceneData->m_filename != "Remote_Control_Cannon.max") ticksperframe = 10000;
	if (m_pSceneData->m_filename != "Remote_Control_Cannon.max") ticksperframe = 10000;


	//&&m_name != "Biped-R_legup")
	//&& m_name != "Biped-R_leg"
	//&& m_name != "Biped - R_leg_end"
	//&& m_name != "Bone01")
	//{
	//	XMMATRIX tempPositionTM;
	//	XMMATRIX tempRotationTM;
	//	XMMATRIX tempScale_sumTM;
	//
	//	/// �ִϸ��̼��� ���� ����� �⺻ LocalTM..
	//	tempPositionTM = XMMatrixTranslation(m_LocalTM_pos.x, m_LocalTM_pos.y, m_LocalTM_pos.z);
	//	tempRotationTM = XMMatrixRotationQuaternion(m_LocalTM_rot);
	//	tempScale_sumTM = XMMatrixScaling(m_LocalTM_scl.x, m_LocalTM_scl.y, m_LocalTM_scl.z);
	//
	//	SetLocalTM(tempScale_sumTM * tempRotationTM * tempPositionTM);
	//	//SetLocalTM(tempScale_sumTM* tempRotationTM);
	//	//SetLocalTM(XMMatrixIdentity());
	//
	//	return true;
	//}

	///--------------------------------------------------
	/// 1. �ϴ� �������� ������.
	///--------------------------------------------------
	XMMATRIX tempPositionTM;
	tempPositionTM = XMMatrixIdentity();

	// ���������̼ǿ� �ʿ��� ����
	XMFLOAT3	leftpos;				// ���� ������ ��
	XMFLOAT3	rightpos;
	float		lefttime = 0.0f;		// �� �������� ��ϵ� �ð���
	float		righttime = 0.0f;

	// ������ ����� 0�̶�� �������� �̵��� ���°Ŵ� (�ִ� �����Ͱ� ������ �������� �ִϸ��̼��� �������� �ִ�)
	if (m_pAnimationTable->m_position.size() == 0)
	{
		// �⺻���� LocalTM�� Decomposed Data�� ����
		tempPositionTM = XMMatrixTranslation(m_LocalTM_pos.x, m_LocalTM_pos.y, m_LocalTM_pos.z);
	}
	else
	{
		///--------------------------------------------------------------
		/// ���� �������� �������� ��, �찪�� ã���ֱ� ���� �κ�
		lastindex = m_pAnimationTable->m_position.size() - 1;

		starttick = m_pAnimationTable->m_position[0]->m_time;
		endtick = m_pAnimationTable->m_position[lastindex]->m_time;

		// 1. ���� tick�� starttick���� �۴ٸ� ���� �ִϸ��̼��� ����.
		if (nowtick <= starttick)
		{
			// �⺻���� LocalTM�� Decomposed Data�� ����.
			tempPositionTM = XMMatrixTranslation(m_LocalTM_pos.x, m_LocalTM_pos.y, m_LocalTM_pos.z);
		}	// 1.
		else
		{
			if (m_name == "Bone01")
			{
				int kk = 0;
			}

			// 2. �������� ������� ���Դٴ� ��.
			// a. endtick���� ũ�ٸ� ������ ���� �����Ѵ�.
			if (nowtick > endtick)
			{
				// ������ ���� ��� ����
				leftpos = m_pAnimationTable->m_position[lastindex]->m_pos;
				rightpos = m_pAnimationTable->m_position[lastindex]->m_pos;
				lefttime = (float)m_pAnimationTable->m_position[lastindex]->m_time;
				righttime = (float)m_pAnimationTable->m_position[lastindex]->m_time + ticksperframe;
			}
			else
			{
				// b. �װ͵� �ƴϸ� ���� tick�� starttick�� endtick ���̿� �ִ� ���̴�.
				// ������ ���ư��鼭 �ϴ� ���� tick���� ū ���� ã�´�
				for (int i = 1; i <= lastindex; i++)
				{
					// ���� ƽ���� ū ���� �߰����� ��
					if (nowtick <= m_pAnimationTable->m_position[i]->m_time)
					{
						// �� ���� ��������, �ٷ� ���� Ű ���� �ƴ϶��(���� ticksperframe���� ũ�ٸ�) ��, �� ���� �°����� �ȴ�.
						if (m_pAnimationTable->m_position[i]->m_time - nowtick > ticksperframe)
						{
							leftpos = m_pAnimationTable->m_position[i - 1]->m_pos;
							rightpos = m_pAnimationTable->m_position[i - 1]->m_pos;
							lefttime = (float)m_pAnimationTable->m_position[i - 1]->m_time;	// �ð� ������ �⺻��
							righttime = (float)m_pAnimationTable->m_position[i - 1]->m_time + ticksperframe;
							flag_findkeyframe = true;
							break;
						}
						else
						{
							// �ٷ� ������ Ű ���� �ִٸ�, ��, �� ���� �����ϸ� �ȴ�
							// �°��� �ٷ� �� ���̴� �ƴϴ� �������. (�׳� ���� tick���� ���ʿ��� �������� �ȴ�)

							// ��, �찪�� ����Ѵ�.
							leftpos = m_pAnimationTable->m_position[i - 1]->m_pos;
							rightpos = m_pAnimationTable->m_position[i]->m_pos;
							lefttime = (float)m_pAnimationTable->m_position[i - 1]->m_time;
							righttime = (float)m_pAnimationTable->m_position[i]->m_time;
							flag_findkeyframe = true;
							break;
						}
					}
				}	// for m_position.size()

				// ������ �� ���Ҵµ� ��ã�� ��찡 ������?
				// ���� ƽ���� ū �� �߰� ��������.. �̰� ������ ������ �ִ� ����
				if (flag_findkeyframe == false)
				{
					____error(L"�ִϸ��̼� Tick�� ã�� ���� ������ �߻��߽��ϴ�!");
				}

			}	// b

			///--------------------------------------------------------------
			/// ������� �Դٸ� ��, �찪�� ��ϵȰŴ�. (����,�ð���)
			/// �� ���� �ð��븦 �������� interpolation

			// �� ���� ������ �� ����
			LONG nowtime = nowtick;

			// ���� ������ ���� �����̴�
			float s = 0;
			if (righttime - lefttime != 0)
			{
				s = (float)((nowtime - lefttime) / (righttime - lefttime));
			}
			else
			{
				s = 0;
			}

			// ���� ������ �� pos�����̴�
			XMFLOAT3 resultVec;
			///resultVec = XMVectorLerp(&leftpos, &rightpos, s);
			resultVec = Vector3::Lerp(leftpos, rightpos, s);

			// ��������� ������ �̵� ����� ������ش�.
			tempPositionTM = XMMatrixTranslation(resultVec.x, resultVec.y, resultVec.z);
		}
	}

	/// ------------------------------------------------------------------
	/// 2. �����̼��� ���Ѵ�.
	/// ------------------------------------------------------------------

	XMMATRIX tempRotationTM;
	tempRotationTM = XMMatrixIdentity();
	Quaternion leftQT;
	Quaternion rightQT;
	Quaternion resultQT;
	leftQT = Quaternion::Identity;
	rightQT = Quaternion::Identity;
	resultQT = Quaternion::Identity;

	//D3DXQuaternionIsIdentity(&leftQT);			// identity�� ����� �� �ʿ������ ������!
	//D3DXQuaternionIsIdentity(&rightQT);			// ���� ������?
	//D3DXQuaternionIsIdentity(&resultQT);

	flag_findsamename = false;
	flag_findkeyframe = false;

	// �����̼� ����� 0�̶�� �����̼��� ���°Ŵ� (�ִ� �����Ͱ� ������ �����̼��� �ִϸ��̼��� �������� �ִ�)
	if (m_pAnimationTable->m_rotation.size() == 0)
	{
		// �⺻���� LocalTM�� Decomposed Data�� ����
		tempRotationTM = XMMatrixRotationQuaternion(m_LocalTM_rot);
	}
	else
	{
		///--------------------------------------------------------------
		/// ���� �������� �������� ��, �찪�� ã���ֱ� ���� �κ�
		lastindex = m_pAnimationTable->m_rotation.size() - 1;

		starttick = m_pAnimationTable->m_rotation[0]->m_time;
		endtick = m_pAnimationTable->m_rotation[lastindex]->m_time;

		// 1. ���� tick�� starttick���� �۴ٸ� ���� �ִϸ��̼��� ����.
		if (nowtick <= starttick)
		{
			// �⺻���� LocalTM�� Decomposed Data�� ����.
			tempRotationTM = XMMatrixRotationQuaternion(m_LocalTM_rot);
		}	// 1.
		else
		{
			// 2. endtick���� ũ�ٸ� ������ ���� �����Ѵ�.
			if (nowtick > endtick)
			{
				// ������ ���� ��� ����
				leftQT = m_pAnimationTable->m_rotation[lastindex]->m_rotQT_accumulation;
				rightQT = m_pAnimationTable->m_rotation[lastindex]->m_rotQT_accumulation;
				lefttime = (float)m_pAnimationTable->m_rotation[lastindex]->m_time;
				righttime = (float)m_pAnimationTable->m_rotation[lastindex]->m_time + ticksperframe;
			}	// 2.
			else
			{
				// 3. �װ͵� �ƴϸ� ���� tick�� starttick�� endtick ���̿� �ִ� ���̴�.
				// ������ ���ư��鼭 �ϴ� ���� tick���� ū ���� ã�´�
				for (int i = 1; i <= lastindex; i++)
				{
					// ���� ƽ���� ū ���� �߰����� ��
					if (nowtick <= m_pAnimationTable->m_rotation[i]->m_time)
					{
						// �� ���� ��������, �ٷ� ���� Ű ���� �ƴ϶��(���� ticksperframe���� ũ�ٸ�) ��, �� ���� �°����� �ȴ�.
						if (m_pAnimationTable->m_rotation[i]->m_time - nowtick > ticksperframe)
						{
							leftQT = m_pAnimationTable->m_rotation[i - 1]->m_rotQT_accumulation;
							rightQT = m_pAnimationTable->m_rotation[i - 1]->m_rotQT_accumulation;
							lefttime = (float)m_pAnimationTable->m_rotation[i - 1]->m_time;	// �ð� ������ �⺻��
							righttime = (float)m_pAnimationTable->m_rotation[i - 1]->m_time + ticksperframe;
							flag_findkeyframe = true;
							//TRACE("1\n");
							break;
						}
						else
						{
							// �ٷ� ������ Ű ���� �ִٸ�, ��, �� ���� �����ϸ� �ȴ�
							// �°��� �ٷ� �� ���̴� �ƴϴ� �������. (�׳� ���� tick���� ���ʿ��� �������� �ȴ�)

							// ��, �찪�� ����Ѵ�.
							leftQT = m_pAnimationTable->m_rotation[i - 1]->m_rotQT_accumulation;
							rightQT = m_pAnimationTable->m_rotation[i]->m_rotQT_accumulation;
							lefttime = (float)m_pAnimationTable->m_rotation[i - 1]->m_time;
							righttime = (float)m_pAnimationTable->m_rotation[i]->m_time;
							flag_findkeyframe = true;
							break;
						}
					}
				}	// for m_position.size()

				// ������ �� ���Ҵµ� ��ã�� ��찡 ������?
				// ���� ƽ���� ū �� �߰� ��������.. �̰� ������ ������ �ִ� ����
				if (flag_findkeyframe == false)
				{
					TRACE(L"�ִϸ��̼� Tick�� ã�� ���� ������ �߻��߽��ϴ�!");
				}

			}	// 3.

			///--------------------------------------------------------------
			/// ������� �Դٸ� ��, �찪�� ��ϵȰŴ�. (����,�ð���)
			/// �� ���� �ð��븦 �������� interpolation

			// �� ���� ������ �� ����
			LONG nowtime = nowtick;
			// ���� ������ ���� �����̴�
			float s = 0;
			if (righttime - lefttime != 0)
			{
				s = (float)((nowtime - lefttime) / (righttime - lefttime));
				//s = (float)((nowtime - lefttime) / (ticksperframe));
			}
			else
			{
				// �� �� �ð����� ���� ����, �� ���� Ű�� �� ������ ���̴�. �׷��Ƿ� S�� ���� 0�̾�� �Ѵ�.
				s = 0;
			}

			if (s < 0 || 1.0f < s)
			{

				int kk = 0;
			}

			// ���ʹϾ� ���鼱������.
			leftQT.Normalize();
			rightQT.Normalize();
			resultQT = XMQuaternionSlerp(leftQT, rightQT, s);
			resultQT.Normalize();

			//XMVECTOR r = XMVectorLerp(leftQT, rightQT, s);
			//resultQT = XMQuaternionNormalize(r);

			//resultQT = leftQT;
			//resultQT = rightQT;

			// ��� ���ʹϾ��� ��ķ� �ٲ��ش�. 
			tempRotationTM = XMMatrixRotationQuaternion(resultQT);

			// tempRotationTM�� �������� �ݵ�� break!	
		}
	}


#pragma region _SCALE
	/// ------------------------------------------------------------------
	/// 3. �������̴�. �������� ���Ѵ�. ����..
	/// ------------------------------------------------------------------

	// ������ ������ �ΰ����� TM�� �����Ѵ�. ���� pos�� rotó�� ó�����ָ� �ȴٰ� �Ѵ�.
	XMMATRIX tempScale_scaTM;
	XMMATRIX tempScale_rotTM;
	XMMATRIX tempScale_sumTM;			// �� ���� ���ߴ�.
	tempScale_scaTM = XMMatrixIdentity();
	tempScale_rotTM = XMMatrixIdentity();
	tempScale_sumTM = XMMatrixIdentity();

	Vector3 leftscale;					// ���� ������ ��
	Vector3 rightscale;
	/// rot������ �����̼ǰ��� �״�� �����ٰ� ����
	leftQT = Quaternion::Identity;
	rightQT = Quaternion::Identity;
	resultQT = Quaternion::Identity;
	//D3DXQuaternionIsIdentity(&leftQT);	// identity�� ����� �� �ʿ������ ������!
	//D3DXQuaternionIsIdentity(&rightQT);	// ���� ������?
	//D3DXQuaternionIsIdentity(&resultQT);

	flag_findsamename = false;

	// ������ ����� 0�̶�� �������� ���°Ŵ� (�ִ� �����Ͱ� ������ �������� �ִϸ��̼��� �������� �ִ�)
	if (m_pAnimationTable->m_scale.size() == 0)
	{
		// �⺻���� LocalTM�� Decomposed Data�� ����
		tempScale_sumTM = XMMatrixScaling(m_LocalTM_scl.x, m_LocalTM_scl.y, m_LocalTM_scl.z);
	}
	else
	{
		///--------------------------------------------------------------
		/// ���� �������� �������� ��, �찪�� ã���ֱ� ���� �κ�
		lastindex = m_pAnimationTable->m_scale.size() - 1;

		starttick = m_pAnimationTable->m_scale[0]->m_time;
		endtick = m_pAnimationTable->m_scale[lastindex]->m_time;

		// 1. ���� tick�� starttick���� �۴ٸ� ���� �ִϸ��̼��� ����.
		if (nowtick <= starttick)
		{
			// �⺻���� LocalTM�� Decomposed Data�� ����.
			tempScale_sumTM = XMMatrixScaling(m_LocalTM_scl.x, m_LocalTM_scl.y, m_LocalTM_scl.z);
		}	// 1.
		else
		{
			// 2. endtick���� ũ�ٸ� ������ ���� �����Ѵ�.
			if (nowtick > endtick)
			{
				// ������ ���� ��� ����
				leftscale = m_pAnimationTable->m_scale[lastindex]->m_scale;
				rightscale = m_pAnimationTable->m_scale[lastindex]->m_scale;
				leftQT = m_pAnimationTable->m_scale[lastindex]->m_scalerotQT_accumulation;
				rightQT = m_pAnimationTable->m_scale[lastindex]->m_scalerotQT_accumulation;
				lefttime = (float)m_pAnimationTable->m_scale[lastindex]->m_time;
				righttime = (float)m_pAnimationTable->m_scale[lastindex]->m_time + ticksperframe;
			}
			else
			{
				// 3. �װ͵� �ƴϸ� ���� tick�� starttick�� endtick ���̿� �ִ� ���̴�.
				// ������ ���ư��鼭 �ϴ� ���� tick���� ū ���� ã�´�
				for (int i = 1; i <= lastindex; i++)
				{
					// ���� ƽ���� ū ���� �߰����� ��
					if (nowtick <= m_pAnimationTable->m_scale[i]->m_time)
					{
						// �� ���� ��������, �ٷ� ���� Ű ���� �ƴ϶��(���� ticksperframe���� ũ�ٸ�) ��, �� ���� �°����� �ȴ�.
						if (m_pAnimationTable->m_scale[i]->m_time - nowtick > ticksperframe)
						{
							leftscale = m_pAnimationTable->m_scale[i - 1]->m_scale;
							rightscale = m_pAnimationTable->m_scale[i - 1]->m_scale;
							leftQT = m_pAnimationTable->m_scale[i - 1]->m_scalerotQT_accumulation;
							rightQT = m_pAnimationTable->m_scale[i - 1]->m_scalerotQT_accumulation;
							lefttime = (float)m_pAnimationTable->m_scale[i - 1]->m_time;	// �ð� ������ �⺻��
							righttime = (float)m_pAnimationTable->m_scale[i - 1]->m_time + ticksperframe;
							flag_findkeyframe = true;
							break;
						}
						else
						{
							// �ٷ� ������ Ű ���� �ִٸ�, ��, �� ���� �����ϸ� �ȴ�
							// �°��� �ٷ� �� ���̴� �ƴϴ� �������. (�׳� ���� tick���� ���ʿ��� �������� �ȴ�)

							// ��, �찪�� ����Ѵ�.
							leftscale = m_pAnimationTable->m_scale[i - 1]->m_scale;
							rightscale = m_pAnimationTable->m_scale[i]->m_scale;
							leftQT = m_pAnimationTable->m_scale[i - 1]->m_scalerotQT_accumulation;
							rightQT = m_pAnimationTable->m_scale[i]->m_scalerotQT_accumulation;
							lefttime = (float)m_pAnimationTable->m_scale[i - 1]->m_time;
							righttime = (float)m_pAnimationTable->m_scale[i]->m_time;
							flag_findkeyframe = true;
							break;
						}
					}
				}	// for m_scale.size()

				// ������ �� ���Ҵµ� ��ã�� ��찡 ������?
				// ���� ƽ���� ū �� �߰� ��������.. �̰� ������ ������ �ִ� ����
				if (flag_findkeyframe == false) ____error(L"�ִϸ��̼� Tick�� ã�� ���� ������ �߻��߽��ϴ�!");

			}	// 3.

			///--------------------------------------------------------------
			/// ������� �Դٸ� ��, �찪�� ��ϵȰŴ�. (����,�ð���)
			/// �� ���� �ð��븦 �������� interpolation

			// �� ���� ������ �� ����
			LONG nowtime = nowtick;
			// ���� ������ ���� �����̴�
			float s = 0;
			if (righttime - lefttime != 0)
			{
				s = (float)((nowtime - lefttime) / (righttime - lefttime));
			}
			else
			{
				s = 0;
			}

			/// Scale
			// ���� ������ �� scale�����̴�
			Vector3 resultVec;
			resultVec = Vector3::Lerp(leftscale, rightscale, s);

			// ��������� ������ ����� ������ش�.
			tempScale_scaTM = XMMatrixScaling(resultVec.x, resultVec.y, resultVec.z);

			// tempScale_scaTM ������

			/// ScaleRotation
			// ���ʹϾ� ���鼱������.
			resultQT = XMQuaternionSlerp(leftQT, rightQT, s);

			// ��� ���ʹϾ��� ��ķ� �ٲ��ش�.
			tempScale_rotTM = XMMatrixRotationQuaternion(resultQT);

			// tempScale_rotTM ������

			/// tempScale_sumTM �� ������ش�.
			tempScale_sumTM = tempScale_rotTM * tempScale_scaTM;

			// �� �������� break;
		}
	}
#pragma endregion

	////--------------------------------------------------
	//// ������ ������Ʈ�� ��� �������� ������ķ� ó���ϵ��� �ߴ� (�ӽ�)
	//if ( GetType() == eObjectType_HelperObject) {
	//	XMMatrixIdentity( &tempScale_sumTM );
	//}
	//
	//// ī�޶��� ��� �������� ������ķ� ó���ϵ��� ���� (�ӽ�)
	//if ( GetType() == eObjectType_Camera) {
	//
	//	/// ù°, �������� ������ �̻������� ����?
	//	/// ��°, �ִϸ��̼� �����Ͱ� ������ ó���� ī�޶� Ŭ�������� ����
	//	XMMatrixIdentity( &tempScale_sumTM );
	//	//XMMatrixIdentity( &tempRotationTM );
	//	//XMMatrixIdentity( &tempPositionTM );
	//}
	//
	//// �غ��� ��� �������� ������ķ� ó���ϵ��� �ߴ� (�ӽ�)
	//if ( GetType() == eObjectType_Light) {
	//	XMMatrixIdentity( &tempRotationTM );
	//	XMMatrixIdentity( &tempScale_sumTM );
	//}
	//
	//// ��Bone ��� �������� ������ķ� ó���ϵ��� �ߴ� (�ӽ�)
	////if ( GetType() == eObjectType_Bone) {
	//if ( m_name == "Dummy01") {
	//	XMMatrixIdentity( &tempPositionTM );
	//}

	// �⺻���� LocalTM�� Decomposed Data�� ����
	//tempRotationTM = XMMatrixRotationQuaternion(m_LocalTM_rot);

	//tempRotationTM = XMMatrixRotationQuaternion(m_LocalTM_rot);
	//tempScale_sumTM = XMMatrixScaling(m_LocalTM_scl.x, m_LocalTM_scl.y, m_LocalTM_scl.z);

	// ���� LocalTM�� ����� �ش�.
	XMMATRIX tempSumTM;
	tempSumTM = (tempScale_sumTM * tempRotationTM * tempPositionTM);
	//tempSumTM = (tempRotationTM * tempPositionTM);
	//tempSumTM = (tempRotationTM);

	SetLocalTM(tempSumTM);
	//SetLocalTM( tempPositionTM );
	//SetLocalTM(tempRotationTM);
	//SetLocalTM(tempScale_sumTM);

	/// �ִϸ��̼��� ���� ����� �⺻ LocalTM..
	//XMMATRIX tempScale_sumTM;			// �� ���� ���ߴ�.
	
	/*tempPositionTM = XMMatrixTranslation(m_LocalTM_pos.x, m_LocalTM_pos.y, m_LocalTM_pos.z);
	tempRotationTM = XMMatrixRotationQuaternion(m_LocalTM_rot);
	tempScale_sumTM = XMMatrixScaling(m_LocalTM_scl.x, m_LocalTM_scl.y, m_LocalTM_scl.z);			// TM�� ����ȯ �� ���� �̰� ������ �װ�Ƽ����� ��������. �ִϸ��̼���?

	SetLocalTM(tempScale_sumTM * tempRotationTM * tempPositionTM);*/
	
	//SetLocalTM(tempRotationTM* tempPositionTM);
	//SetLocalTM(XMMatrixIdentity());

	return true;
}

bool INodeTM::GetScenedata(Mesh* ponemesh)
{
	Scenedata* newscenedata = new Scenedata;	// ���� ������ �Ҵ�
	m_pSceneData = newscenedata;

	// �ذ� ����
	*m_pSceneData = ponemesh->m_scenedata;

	return true;
}
