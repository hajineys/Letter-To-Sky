#include "INodeTM.h"

INodeTM::INodeTM()
{
	/// 타입, 이름, 부모 노드
	//std::string m_name;		// 이름
	//std::string m_nodeparent;	// 부모 노드의 이름
	m_isparentexist = false;	// 부모가 있는가?
	m_parentspointer = NULL;	// ※부모 노드의 포인터

	/// 변환, 행렬
	m_NodeTM = Matrix::Identity;
	m_NodeTM2 = Matrix::Identity;

	// NodeTM 관련 (ASE파싱시 초기값)
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

	/// 애니메이션 데이터
	m_isExistAnimation = false;	// 기본적으로 오브젝트는 애니메이션이 없는 것으로 간주한다
	m_elapsedFrame = 0.0f;
	m_nowframe = 0;				// 현재 프레임
	m_nowticks = 0;				// 현재 Ticks
	m_minKeyframe = 192;		// 용가리는 800, Joe는 192등등 애니메이션마다 다르다.

	m_pAnimationTable = NULL;	// Parser에서 받아온다
	m_pSceneData = NULL;		// 마찬가지
}

INodeTM::~INodeTM()
{

}

bool INodeTM::GetNodeTM_FromMesh(Mesh* pMesh)
{
	//------------------------------------------------------------------------------------
	//	이름, 부모 외 여러가지
	//------------------------------------------------------------------------------------
	m_name = pMesh->m_nodename;						// 이름
	m_nodeparent = pMesh->m_nodeparent;				// 부모
	m_isparentexist = pMesh->m_isparentexist;		// 부모의 존재 여부
	m_isExistAnimation = pMesh->m_isAnimated;		// 애니메이션데이터 존재 여부

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

	/// 위의 것을 합쳐서 NodeTM을 만든다
	XMMATRIX _posTM;
	XMMATRIX _rotTM;
	XMMATRIX _sclTM;

	// 포지션
	_posTM = XMMatrixTranslation(pMesh->m_tm_pos.x, pMesh->m_tm_pos.y, pMesh->m_tm_pos.z);

	// 로테이션
	//D3DXQUATERNION __tempQT;	// 쿼터니언으로 만들어봤다.
	//D3DXQuaternionRotationAxis(&__tempQT, &ponemesh->m_tm_rotaxis, ponemesh->m_tm_rotangle);
	//XMMatrixRotationQuaternion(&_RotTM, &__tempQT);
	_rotTM = XMMatrixRotationAxis(pMesh->m_tm_rotaxis, pMesh->m_tm_rotangle);

	// 스케일
	_sclTM = XMMatrixScaling(pMesh->m_tm_scale.x, pMesh->m_tm_scale.y, pMesh->m_tm_scale.z);

	XMMATRIX _tempTM;
	XMMATRIX _tempTM2;
	_tempTM = XMMatrixRotationAxis(pMesh->m_tm_scaleaxis, pMesh->m_tm_scaleaxisang);
	_tempTM2 = XMMatrixInverse(nullptr, _tempTM);
	//_SclTM = __tempTM2 * _SclTM * __tempTM;

	_sclTM = XMMatrixMultiply(_tempTM2, _sclTM);
	_sclTM = XMMatrixMultiply(_sclTM, _tempTM);
	/// ※이게 결론적으로 필요했던 NodeTM.
	m_NodeTM = _sclTM * _rotTM * _posTM;

	///--------------------
	/// NodeTM2
	///--------------------
	/// 일단 변환 행렬을 구해서 4x4로 만들어준다.
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

	// 헌데 그냥 저장하면 D3D에서는 못쓴다
	// 축 문제로 이렇게 저렇게 변환해서 읽어야 함
	swap(m_NodeTM2._12, m_NodeTM2._13);
	swap(m_NodeTM2._22, m_NodeTM2._23);
	swap(m_NodeTM2._32, m_NodeTM2._33);
	swap(m_NodeTM2._42, m_NodeTM2._43);

	swap(m_NodeTM2._21, m_NodeTM2._31);
	swap(m_NodeTM2._22, m_NodeTM2._32);
	swap(m_NodeTM2._23, m_NodeTM2._33);
	swap(m_NodeTM2._24, m_NodeTM2._34);

	///--------------------
	/// Negative Scale 검출 시도.
	/// NodeTM1을 쓰는 경우, 2와의 비교를 통해서 검출
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
			// 네거티브 스케일인 경우
			// 노말을 다 뒤집어보자
			// 근본적인 해결책이 못되며 Cannon은 여전이 움직임이 뒤집힌 문제가 있다

			for (unsigned int i = 0; i < pMesh->m_opt_vertex.size(); i++)
			{
				pMesh->m_opt_vertex[i]->m_normal.x = -(pMesh->m_opt_vertex[i]->m_normal.x);
				pMesh->m_opt_vertex[i]->m_normal.y = -(pMesh->m_opt_vertex[i]->m_normal.y);
				pMesh->m_opt_vertex[i]->m_normal.z = -(pMesh->m_opt_vertex[i]->m_normal.z);
			}

			/// 스케일 자체는 뒤집을 필요가 없다. (decomposed data로 했을 경우)
			//m_NodeTM2._11 *= -1.0f;
			//m_NodeTM2._22 *= -1.0f;
			//m_NodeTM2._33 *= -1.0f;
		}
	}

	///--------------------
	/// 두 번째
	/// Negative Scale 검출 시도.
	/// NodeTM2를 쓰는 경우. 자체적으로 cross하고 세 번째 벡터와 비교
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

			// 노말도 뒤집어 (줄 수 있으면) 뒤집어준다.
			for (unsigned int i = 0; i < pMesh->m_opt_vertex.size(); i++)
			{
				pMesh->m_opt_vertex[i]->m_normal.x = -(pMesh->m_opt_vertex[i]->m_normal.x);
				pMesh->m_opt_vertex[i]->m_normal.y = -(pMesh->m_opt_vertex[i]->m_normal.y);
				pMesh->m_opt_vertex[i]->m_normal.z = -(pMesh->m_opt_vertex[i]->m_normal.z);
			}
		}
		*/
	}

	/// 발 뒤집힌 것이 나오게 할 수 있다.
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

	/*XMFLOAT4X4 tempTM;					// 최종적으로 적용될 매트릭스.

	//---------------------------------------------------------------------------------------------------------------
	// 버텍스들의 로테이션 매트릭스를 구한다.
	// 1. 기존 데이터는 원점 기준으로 되어 있다.
	// m_axi_pos는 축의 위치를 나타내는 것
	XMFLOAT4X4 rotateTM;				// 회전용 매트릭스

	XMMatrixIdentity(&rotateTM);		// 회전용 매트릭스
	XMMatrixIdentity(&tempTM);		// 임시용 매트릭스

	// (원점 상태에서) 로테이션
	XMMatrixIdentity(&tempTM);
	D3DXMatrixRotationX(&tempTM, m_MoveTMrot.x);
	rotateTM *= tempTM;

	XMMatrixIdentity(&tempTM);
	D3DXMatrixRotationY(&tempTM, m_MoveTMrot.y);
	rotateTM *= tempTM;

	XMMatrixIdentity(&tempTM);
	D3DXMatrixRotationZ(&tempTM, m_MoveTMrot.z);
	rotateTM *= tempTM;

	/// 한번에 돌리기
	//XMMatrixIdentity(&tempTM);
	//D3DXMatrixRotationYawPitchRoll(&tempTM, m_rot.y, m_rot.x, m_rot.z);
	//rotateTM *= tempTM;
	//---------------------------------------------------------------------------------------------------------------

	//---------------------------------------------------------------------------------------------------------------
	// 2. 이번에는 이동 변화량 매트릭스를 구하는데, 좌표계가 자신이 기준이다.
	XMFLOAT4X4 moveTM;					// 이동을 위한 매트릭스
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
		// 부모가 없다면? 월드는 로컬과 같다.
		m_WorldTM = m_LocalTM;
	}
	else
	{
		// 부모가 있다면 하나씩 곱해나가면 된다
		m_WorldTM = m_LocalTM;		/// 이건 일종의 안전코드라고 할 수 있다.

		// 부모가 있다면 부모의 로컬 좌표를 곱해주자
		INodeTM* nowparent;
		nowparent = (INodeTM*)m_parentspointer;

		while (nowparent != NULL)
		{
			m_WorldTM = m_WorldTM * nowparent->GetLocalTM();

			// 현재 부모 메쉬의 포인터를 한 칸 위로 올린다
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
	// 부모없는 녀석이라면 LocalTM은 자신의 WorldTM과 같다.
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
///	애니메이션 관련
///--------------------------------------------------

bool INodeTM::AnimateFrames(float deltaTime)
{
	// 애니메이션이 없다면 이 오브젝트의 m_nowticks도 없다
	if (m_pAnimationTable == NULL)
	{
		m_nowticks = 0;
		return false;
	}

	///m_elaspedtime += ((float)m_pSceneData->m_framespeed / 60.0f) * deltaTime;	// 비례해서 늘어나도록.

	m_elapsedFrame += deltaTime * (float)m_pSceneData->m_framespeed;
	m_nowframe = (int)m_elapsedFrame;			// float형이므로 캐스팅
	m_nowticks = m_nowframe * m_pAnimationTable->m_ticksperFrame;

	// 애니메이션 총 길이
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
	// 애니메이션이 없다면
	if (m_isExistAnimation == false)
	{
		//계산 해 줄 것도 없이 결과 행렬은 기본적인 LocalTM이다.
		return false;
	}

	//--------------------------------------------------
	// 목표 :
	// 오브젝트 내부에 애니메이션 테이블을 가지고 있다.
	// 절대적인 프레임 넘버를 참조, 애니메이션 데이터 값을 가져온다.(Position, Rotation, Scale로 나뉘어진)
	// 그 데이터를 기존의 값을 대체해서 결국 메쉬 하나의 LocalTM을 만든다.
	//--------------------------------------------------

	// 지역변수들
	bool flag_findsamename = false;

	// nowtick
	LONG nowtick = m_nowticks;

	LONG starttick, endtick;
	int lastindex;
	bool flag_findkeyframe = false;

	// SceneData를 입력받지 못해서 없는 오브젝트일수도 있다.
	// 이런 경우 디폴트 월드의 SceneData를 쓴다. 위의 구문과 순서가 바뀔경우 위험할수 있음.
	if (m_pSceneData == NULL)
	{
		m_pSceneData = worldSceneData;
	}

	int ticksperframe = m_pAnimationTable->m_ticksperFrame;

	// ticks per frame에 문제가 있다.
	// 아마 이것은 최소프레임의 값은 아닌듯? 보간시 필요한 데이터는 최소 키프레임 간격 값인데.
	// 원래대로라면 최소 키프레임의 값이 들어가야 하는데 dragon, IK JOE 최소키프레임 값이 아니다. (더 작다-그래서 뚝뚝 끊기는 문제 발생. 반대로 캐논에서 키프레임을 더 크게 잡아버리면 뻥뻥이 아니라 질질이 된다..)
	/// 그래서 임시 수정.
	/// 궁극으로는 최소 키프레임을 계산해주는 부분이 있어야겠다.
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
	//	/// 애니메이션이 없을 경우의 기본 LocalTM..
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
	/// 1. 일단 포지션을 구하자.
	///--------------------------------------------------
	XMMATRIX tempPositionTM;
	tempPositionTM = XMMatrixIdentity();

	// 인터폴레이션에 필요한 변수
	XMFLOAT3	leftpos;				// 실제 포지션 값
	XMFLOAT3	rightpos;
	float		lefttime = 0.0f;		// 그 포지션이 기록된 시간값
	float		righttime = 0.0f;

	// 포지션 사이즈가 0이라면 포지션의 이동은 없는거다 (애니 데이터가 있지만 포지션의 애니메이션이 없을수도 있다)
	if (m_pAnimationTable->m_position.size() == 0)
	{
		// 기본적인 LocalTM의 Decomposed Data를 쓴다
		tempPositionTM = XMMatrixTranslation(m_LocalTM_pos.x, m_LocalTM_pos.y, m_LocalTM_pos.z);
	}
	else
	{
		///--------------------------------------------------------------
		/// 현재 포지션을 기준으로 좌, 우값을 찾아주기 위한 부분
		lastindex = m_pAnimationTable->m_position.size() - 1;

		starttick = m_pAnimationTable->m_position[0]->m_time;
		endtick = m_pAnimationTable->m_position[lastindex]->m_time;

		// 1. 현재 tick이 starttick보다 작다면 아직 애니메이션이 없다.
		if (nowtick <= starttick)
		{
			// 기본적인 LocalTM의 Decomposed Data를 쓴다.
			tempPositionTM = XMMatrixTranslation(m_LocalTM_pos.x, m_LocalTM_pos.y, m_LocalTM_pos.z);
		}	// 1.
		else
		{
			if (m_name == "Bone01")
			{
				int kk = 0;
			}

			// 2. 포지션이 나오기는 나왔다는 것.
			// a. endtick보다 크다면 마지막 값을 유지한다.
			if (nowtick > endtick)
			{
				// 마지막 값을 계속 유지
				leftpos = m_pAnimationTable->m_position[lastindex]->m_pos;
				rightpos = m_pAnimationTable->m_position[lastindex]->m_pos;
				lefttime = (float)m_pAnimationTable->m_position[lastindex]->m_time;
				righttime = (float)m_pAnimationTable->m_position[lastindex]->m_time + ticksperframe;
			}
			else
			{
				// b. 그것도 아니면 현재 tick이 starttick과 endtick 사이에 있는 것이다.
				// 루프를 돌아가면서 일단 현재 tick보다 큰 수를 찾는다
				for (int i = 1; i <= lastindex; i++)
				{
					// 현재 틱보다 큰 값을 발견했을 때
					if (nowtick <= m_pAnimationTable->m_position[i]->m_time)
					{
						// 우 값을 가져오되, 바로 다음 키 값이 아니라면(차가 ticksperframe보다 크다면) 좌, 우 값은 좌값으로 된다.
						if (m_pAnimationTable->m_position[i]->m_time - nowtick > ticksperframe)
						{
							leftpos = m_pAnimationTable->m_position[i - 1]->m_pos;
							rightpos = m_pAnimationTable->m_position[i - 1]->m_pos;
							lefttime = (float)m_pAnimationTable->m_position[i - 1]->m_time;	// 시간 간격은 기본값
							righttime = (float)m_pAnimationTable->m_position[i - 1]->m_time + ticksperframe;
							flag_findkeyframe = true;
							break;
						}
						else
						{
							// 바로 다음에 키 값이 있다면, 좌, 우 값을 보간하면 된다
							// 좌값은 바로 전 값이던 아니던 상관없다. (그냥 지금 tick보다 왼쪽에서 가져오면 된다)

							// 좌, 우값을 기록한다.
							leftpos = m_pAnimationTable->m_position[i - 1]->m_pos;
							rightpos = m_pAnimationTable->m_position[i]->m_pos;
							lefttime = (float)m_pAnimationTable->m_position[i - 1]->m_time;
							righttime = (float)m_pAnimationTable->m_position[i]->m_time;
							flag_findkeyframe = true;
							break;
						}
					}
				}	// for m_position.size()

				// 루프를 다 돌았는데 못찾는 경우가 있을까?
				// 현재 틱보다 큰 값 발견 못했을시.. 이건 로직에 문제가 있는 경우다
				if (flag_findkeyframe == false)
				{
					____error(L"애니메이션 Tick을 찾던 도중 문제가 발생했습니다!");
				}

			}	// b

			///--------------------------------------------------------------
			/// 여기까지 왔다면 좌, 우값이 기록된거다. (벡터,시간값)
			/// 자 이제 시간대를 기준으로 interpolation

			// 자 선형 보간을 해 보자
			LONG nowtime = nowtick;

			// 선형 보간을 위한 비율이다
			float s = 0;
			if (righttime - lefttime != 0)
			{
				s = (float)((nowtime - lefttime) / (righttime - lefttime));
			}
			else
			{
				s = 0;
			}

			// 선형 보간이 된 pos벡터이다
			XMFLOAT3 resultVec;
			///resultVec = XMVectorLerp(&leftpos, &rightpos, s);
			resultVec = Vector3::Lerp(leftpos, rightpos, s);

			// 결론적으로 보간된 이동 행렬을 만들어준다.
			tempPositionTM = XMMatrixTranslation(resultVec.x, resultVec.y, resultVec.z);
		}
	}

	/// ------------------------------------------------------------------
	/// 2. 로테이션을 구한다.
	/// ------------------------------------------------------------------

	XMMATRIX tempRotationTM;
	tempRotationTM = XMMatrixIdentity();
	Quaternion leftQT;
	Quaternion rightQT;
	Quaternion resultQT;
	leftQT = Quaternion::Identity;
	rightQT = Quaternion::Identity;
	resultQT = Quaternion::Identity;

	//D3DXQuaternionIsIdentity(&leftQT);			// identity로 만들어 줄 필요까지는 없지만!
	//D3DXQuaternionIsIdentity(&rightQT);			// 정말 없을까?
	//D3DXQuaternionIsIdentity(&resultQT);

	flag_findsamename = false;
	flag_findkeyframe = false;

	// 로테이션 사이즈가 0이라면 로테이션은 없는거다 (애니 데이터가 있지만 로테이션의 애니메이션이 없을수도 있다)
	if (m_pAnimationTable->m_rotation.size() == 0)
	{
		// 기본적인 LocalTM의 Decomposed Data를 쓴다
		tempRotationTM = XMMatrixRotationQuaternion(m_LocalTM_rot);
	}
	else
	{
		///--------------------------------------------------------------
		/// 현재 포지션을 기준으로 좌, 우값을 찾아주기 위한 부분
		lastindex = m_pAnimationTable->m_rotation.size() - 1;

		starttick = m_pAnimationTable->m_rotation[0]->m_time;
		endtick = m_pAnimationTable->m_rotation[lastindex]->m_time;

		// 1. 현재 tick이 starttick보다 작다면 아직 애니메이션이 없다.
		if (nowtick <= starttick)
		{
			// 기본적인 LocalTM의 Decomposed Data를 쓴다.
			tempRotationTM = XMMatrixRotationQuaternion(m_LocalTM_rot);
		}	// 1.
		else
		{
			// 2. endtick보다 크다면 마지막 값을 유지한다.
			if (nowtick > endtick)
			{
				// 마지막 값을 계속 유지
				leftQT = m_pAnimationTable->m_rotation[lastindex]->m_rotQT_accumulation;
				rightQT = m_pAnimationTable->m_rotation[lastindex]->m_rotQT_accumulation;
				lefttime = (float)m_pAnimationTable->m_rotation[lastindex]->m_time;
				righttime = (float)m_pAnimationTable->m_rotation[lastindex]->m_time + ticksperframe;
			}	// 2.
			else
			{
				// 3. 그것도 아니면 현재 tick이 starttick과 endtick 사이에 있는 것이다.
				// 루프를 돌아가면서 일단 현재 tick보다 큰 수를 찾는다
				for (int i = 1; i <= lastindex; i++)
				{
					// 현재 틱보다 큰 값을 발견했을 때
					if (nowtick <= m_pAnimationTable->m_rotation[i]->m_time)
					{
						// 우 값을 가져오되, 바로 다음 키 값이 아니라면(차가 ticksperframe보다 크다면) 좌, 우 값은 좌값으로 된다.
						if (m_pAnimationTable->m_rotation[i]->m_time - nowtick > ticksperframe)
						{
							leftQT = m_pAnimationTable->m_rotation[i - 1]->m_rotQT_accumulation;
							rightQT = m_pAnimationTable->m_rotation[i - 1]->m_rotQT_accumulation;
							lefttime = (float)m_pAnimationTable->m_rotation[i - 1]->m_time;	// 시간 간격은 기본값
							righttime = (float)m_pAnimationTable->m_rotation[i - 1]->m_time + ticksperframe;
							flag_findkeyframe = true;
							//TRACE("1\n");
							break;
						}
						else
						{
							// 바로 다음에 키 값이 있다면, 좌, 우 값을 보간하면 된다
							// 좌값은 바로 전 값이던 아니던 상관없다. (그냥 지금 tick보다 왼쪽에서 가져오면 된다)

							// 좌, 우값을 기록한다.
							leftQT = m_pAnimationTable->m_rotation[i - 1]->m_rotQT_accumulation;
							rightQT = m_pAnimationTable->m_rotation[i]->m_rotQT_accumulation;
							lefttime = (float)m_pAnimationTable->m_rotation[i - 1]->m_time;
							righttime = (float)m_pAnimationTable->m_rotation[i]->m_time;
							flag_findkeyframe = true;
							break;
						}
					}
				}	// for m_position.size()

				// 루프를 다 돌았는데 못찾는 경우가 있을까?
				// 현재 틱보다 큰 값 발견 못했을시.. 이건 로직에 문제가 있는 경우다
				if (flag_findkeyframe == false)
				{
					TRACE(L"애니메이션 Tick을 찾던 도중 문제가 발생했습니다!");
				}

			}	// 3.

			///--------------------------------------------------------------
			/// 여기까지 왔다면 좌, 우값이 기록된거다. (벡터,시간값)
			/// 자 이제 시간대를 기준으로 interpolation

			// 자 선형 보간을 해 보자
			LONG nowtime = nowtick;
			// 선형 보간을 위한 비율이다
			float s = 0;
			if (righttime - lefttime != 0)
			{
				s = (float)((nowtime - lefttime) / (righttime - lefttime));
				//s = (float)((nowtime - lefttime) / (ticksperframe));
			}
			else
			{
				// 좌 우 시간값이 같은 경우는, 한 개의 키가 쭉 유지될 때이다. 그러므로 S의 값은 0이어야 한다.
				s = 0;
			}

			if (s < 0 || 1.0f < s)
			{

				int kk = 0;
			}

			// 쿼터니언 구면선형보간.
			leftQT.Normalize();
			rightQT.Normalize();
			resultQT = XMQuaternionSlerp(leftQT, rightQT, s);
			resultQT.Normalize();

			//XMVECTOR r = XMVectorLerp(leftQT, rightQT, s);
			//resultQT = XMQuaternionNormalize(r);

			//resultQT = leftQT;
			//resultQT = rightQT;

			// 결과 쿼터니언을 행렬로 바꿔준다. 
			tempRotationTM = XMMatrixRotationQuaternion(resultQT);

			// tempRotationTM을 구했으니 반드시 break!	
		}
	}


#pragma region _SCALE
	/// ------------------------------------------------------------------
	/// 3. 마지막이다. 스케일을 구한다. 헥헥..
	/// ------------------------------------------------------------------

	// 스케일 연산은 두가지의 TM이 존재한다. 각각 pos와 rot처럼 처리해주면 된다고 한다.
	XMMATRIX tempScale_scaTM;
	XMMATRIX tempScale_rotTM;
	XMMATRIX tempScale_sumTM;			// 두 개를 합했다.
	tempScale_scaTM = XMMatrixIdentity();
	tempScale_rotTM = XMMatrixIdentity();
	tempScale_sumTM = XMMatrixIdentity();

	Vector3 leftscale;					// 실제 스케일 값
	Vector3 rightscale;
	/// rot변수는 로테이션것을 그대로 가져다가 쓰자
	leftQT = Quaternion::Identity;
	rightQT = Quaternion::Identity;
	resultQT = Quaternion::Identity;
	//D3DXQuaternionIsIdentity(&leftQT);	// identity로 만들어 줄 필요까지는 없지만!
	//D3DXQuaternionIsIdentity(&rightQT);	// 정말 없을까?
	//D3DXQuaternionIsIdentity(&resultQT);

	flag_findsamename = false;

	// 스케일 사이즈가 0이라면 스케일은 없는거다 (애니 데이터가 있지만 스케일의 애니메이션이 없을수도 있다)
	if (m_pAnimationTable->m_scale.size() == 0)
	{
		// 기본적인 LocalTM의 Decomposed Data를 쓴다
		tempScale_sumTM = XMMatrixScaling(m_LocalTM_scl.x, m_LocalTM_scl.y, m_LocalTM_scl.z);
	}
	else
	{
		///--------------------------------------------------------------
		/// 현재 포지션을 기준으로 좌, 우값을 찾아주기 위한 부분
		lastindex = m_pAnimationTable->m_scale.size() - 1;

		starttick = m_pAnimationTable->m_scale[0]->m_time;
		endtick = m_pAnimationTable->m_scale[lastindex]->m_time;

		// 1. 현재 tick이 starttick보다 작다면 아직 애니메이션이 없다.
		if (nowtick <= starttick)
		{
			// 기본적인 LocalTM의 Decomposed Data를 쓴다.
			tempScale_sumTM = XMMatrixScaling(m_LocalTM_scl.x, m_LocalTM_scl.y, m_LocalTM_scl.z);
		}	// 1.
		else
		{
			// 2. endtick보다 크다면 마지막 값을 유지한다.
			if (nowtick > endtick)
			{
				// 마지막 값을 계속 유지
				leftscale = m_pAnimationTable->m_scale[lastindex]->m_scale;
				rightscale = m_pAnimationTable->m_scale[lastindex]->m_scale;
				leftQT = m_pAnimationTable->m_scale[lastindex]->m_scalerotQT_accumulation;
				rightQT = m_pAnimationTable->m_scale[lastindex]->m_scalerotQT_accumulation;
				lefttime = (float)m_pAnimationTable->m_scale[lastindex]->m_time;
				righttime = (float)m_pAnimationTable->m_scale[lastindex]->m_time + ticksperframe;
			}
			else
			{
				// 3. 그것도 아니면 현재 tick이 starttick과 endtick 사이에 있는 것이다.
				// 루프를 돌아가면서 일단 현재 tick보다 큰 수를 찾는다
				for (int i = 1; i <= lastindex; i++)
				{
					// 현재 틱보다 큰 값을 발견했을 때
					if (nowtick <= m_pAnimationTable->m_scale[i]->m_time)
					{
						// 우 값을 가져오되, 바로 다음 키 값이 아니라면(차가 ticksperframe보다 크다면) 좌, 우 값은 좌값으로 된다.
						if (m_pAnimationTable->m_scale[i]->m_time - nowtick > ticksperframe)
						{
							leftscale = m_pAnimationTable->m_scale[i - 1]->m_scale;
							rightscale = m_pAnimationTable->m_scale[i - 1]->m_scale;
							leftQT = m_pAnimationTable->m_scale[i - 1]->m_scalerotQT_accumulation;
							rightQT = m_pAnimationTable->m_scale[i - 1]->m_scalerotQT_accumulation;
							lefttime = (float)m_pAnimationTable->m_scale[i - 1]->m_time;	// 시간 간격은 기본값
							righttime = (float)m_pAnimationTable->m_scale[i - 1]->m_time + ticksperframe;
							flag_findkeyframe = true;
							break;
						}
						else
						{
							// 바로 다음에 키 값이 있다면, 좌, 우 값을 보간하면 된다
							// 좌값은 바로 전 값이던 아니던 상관없다. (그냥 지금 tick보다 왼쪽에서 가져오면 된다)

							// 좌, 우값을 기록한다.
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

				// 루프를 다 돌았는데 못찾는 경우가 있을까?
				// 현재 틱보다 큰 값 발견 못했을시.. 이건 로직에 문제가 있는 경우다
				if (flag_findkeyframe == false) ____error(L"애니메이션 Tick을 찾던 도중 문제가 발생했습니다!");

			}	// 3.

			///--------------------------------------------------------------
			/// 여기까지 왔다면 좌, 우값이 기록된거다. (벡터,시간값)
			/// 자 이제 시간대를 기준으로 interpolation

			// 자 선형 보간을 해 보자
			LONG nowtime = nowtick;
			// 선형 보간을 위한 비율이다
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
			// 선형 보간이 된 scale벡터이다
			Vector3 resultVec;
			resultVec = Vector3::Lerp(leftscale, rightscale, s);

			// 결론적으로 스케일 행렬을 만들어준다.
			tempScale_scaTM = XMMatrixScaling(resultVec.x, resultVec.y, resultVec.z);

			// tempScale_scaTM 구했음

			/// ScaleRotation
			// 쿼터니언 구면선형보간.
			resultQT = XMQuaternionSlerp(leftQT, rightQT, s);

			// 결과 쿼터니언을 행렬로 바꿔준다.
			tempScale_rotTM = XMMatrixRotationQuaternion(resultQT);

			// tempScale_rotTM 구했음

			/// tempScale_sumTM 을 만들어준다.
			tempScale_sumTM = tempScale_rotTM * tempScale_scaTM;

			// 다 구했으니 break;
		}
	}
#pragma endregion

	////--------------------------------------------------
	//// ※헬퍼 오브젝트인 경우 스케일을 단위행렬로 처리하도록 했다 (임시)
	//if ( GetType() == eObjectType_HelperObject) {
	//	XMMatrixIdentity( &tempScale_sumTM );
	//}
	//
	//// 카메라의 경우 스케일을 단위행렬로 처리하도록 했음 (임시)
	//if ( GetType() == eObjectType_Camera) {
	//
	//	/// 첫째, 스케일을 넣으면 이상해지는 이유?
	//	/// 둘째, 애니메이션 데이터가 없을때 처리는 카메라 클래스에서 했음
	//	XMMatrixIdentity( &tempScale_sumTM );
	//	//XMMatrixIdentity( &tempRotationTM );
	//	//XMMatrixIdentity( &tempPositionTM );
	//}
	//
	//// ※빛의 경우 스케일을 단위행렬로 처리하도록 했다 (임시)
	//if ( GetType() == eObjectType_Light) {
	//	XMMatrixIdentity( &tempRotationTM );
	//	XMMatrixIdentity( &tempScale_sumTM );
	//}
	//
	//// ※Bone 경우 포지션을 단위행렬로 처리하도록 했다 (임시)
	////if ( GetType() == eObjectType_Bone) {
	//if ( m_name == "Dummy01") {
	//	XMMatrixIdentity( &tempPositionTM );
	//}

	// 기본적인 LocalTM의 Decomposed Data를 쓴다
	//tempRotationTM = XMMatrixRotationQuaternion(m_LocalTM_rot);

	//tempRotationTM = XMMatrixRotationQuaternion(m_LocalTM_rot);
	//tempScale_sumTM = XMMatrixScaling(m_LocalTM_scl.x, m_LocalTM_scl.y, m_LocalTM_scl.z);

	// 최종 LocalTM을 만들어 준다.
	XMMATRIX tempSumTM;
	tempSumTM = (tempScale_sumTM * tempRotationTM * tempPositionTM);
	//tempSumTM = (tempRotationTM * tempPositionTM);
	//tempSumTM = (tempRotationTM);

	SetLocalTM(tempSumTM);
	//SetLocalTM( tempPositionTM );
	//SetLocalTM(tempRotationTM);
	//SetLocalTM(tempScale_sumTM);

	/// 애니메이션이 없을 경우의 기본 LocalTM..
	//XMMATRIX tempScale_sumTM;			// 두 개를 합했다.
	
	/*tempPositionTM = XMMatrixTranslation(m_LocalTM_pos.x, m_LocalTM_pos.y, m_LocalTM_pos.z);
	tempRotationTM = XMMatrixRotationQuaternion(m_LocalTM_rot);
	tempScale_sumTM = XMMatrixScaling(m_LocalTM_scl.x, m_LocalTM_scl.y, m_LocalTM_scl.z);			// TM을 역변환 한 경우는 이게 없으면 네거티브들이 뒤집힌다. 애니메이션은?

	SetLocalTM(tempScale_sumTM * tempRotationTM * tempPositionTM);*/
	
	//SetLocalTM(tempRotationTM* tempPositionTM);
	//SetLocalTM(XMMatrixIdentity());

	return true;
}

bool INodeTM::GetScenedata(Mesh* ponemesh)
{
	Scenedata* newscenedata = new Scenedata;	// 새로 공간을 할당
	m_pSceneData = newscenedata;

	// ※값 복사
	*m_pSceneData = ponemesh->m_scenedata;

	return true;
}
