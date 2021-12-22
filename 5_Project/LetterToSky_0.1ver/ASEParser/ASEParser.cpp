#include "framework.h"
#include "ASEParser.h"

CASEParser::CASEParser()
	:m_TokenString(""),
	m_Token(0),
	m_data_asciiexport(0),
	m_materialdata(nullptr),
	m_OneMesh(nullptr),
	m_nowshapeline(nullptr),
	m_nowshapevertex(nullptr),
	m_animation(nullptr)
{
	m_parsingmode = eNone;
	m_materialcount = 0;
}

CASEParser::~CASEParser()
{
	delete m_lexer;
	m_lexer = nullptr;
}

bool CASEParser::Init()
{
	m_lexer = new ASE::ASELexer;

	return TRUE;
}

//--------------------------------------------------------------------------------------
// 로딩한다.
// 이것이 끝났다면 정해진 데이터형에 모든 데이터를 읽어서 들어가 있어야 한다
//
// Parsing에 대해:
// 항상 느끼는 것이지만 parsing이라는 것은 데이터가 일정 규격에 맞게 들어가 있다는 것을 전제로 한다
// 하지만, 파일 내부에 순서가 바뀌어 들어가 있는것이 가능하다던지 하는 규칙이 생기면
// 검색하는 루틴이 복잡해지기 마련, 일단은 순서대로 일정 규약으로 들어가 있다는것을 가정하자
// -> 그래서, 재귀 호출을 하는 하나의 큰 함수로 해결봤다.
// -> depth를 기준으로 오동작에 대한 안전 코드를 넣어야겠다
//--------------------------------------------------------------------------------------
bool CASEParser::Load(LPSTR p_File)
{
	// 0) 파일을 로드한다
	if (!m_lexer->Open(p_File))
	{
		TRACE("파일을 여는 중에 문제가 발생했습니다!");
		return FALSE;
	}

	// 1) 재귀함수 한개로 처리 끝!
	Parsing_DivergeRecursiveALL(0);

	// m_MeshList -> 1, 4, 90
	for (UINT i = 0; i < m_MeshList.size(); i++)
	{
		Convert_Optimize_Ver02(m_MeshList[i]);
	}

	Get_NodeTM();
	Get_ParentMesh();
	Get_CountParent();
	Get_ParentMeshMaxCount();
	Get_LocalTM();

	return TRUE;
}

// 1차 변환, CScenedata 복사
bool CASEParser::TranslateToD3DFormat_scene(Scenedata* pscene)
{
	// 값 복사
	*pscene = this->m_scenedata;
	return TRUE;
}

/// <summary>
/// 메시의 최적화를 해 준다
/// 노말값, 텍스쳐 좌표에 따라 버텍스를 늘리고, 중첩되는것은 제거하고
/// </summary>
bool CASEParser::Convert_Optimize_Ver01(Mesh* pMesh)
{
	// 이것이 메시가 아니라면 리턴
	if (!(pMesh->m_type == eGeomobject || pMesh->m_type == eObjectType_SkinnedMesh))
	{
		return false;
	}

	unsigned int _faceCount = pMesh->m_meshface.size();
	//TRACE("FaceCount : %d", _faceCount);

	pMesh->m_opt_index = new IndexList[pMesh->m_meshface.size()];

	for (unsigned int i = 0; i < pMesh->m_meshface.size(); i++)
	{
		//TRACE("FaceNum : %d / %d\n", i, _faceCount);

		for (int j = 0; j < 3; j++)
		{
			// 1. Vertex를 조합한다.
			// 단, 데이터가 없으면 조합하지 않는다.
			Vertex* pVertex;
			pVertex = new Vertex;

			if (pMesh->m_meshvertex.size() != 0)
			{
				pVertex->m_pos = pMesh->m_meshvertex[pMesh->m_meshface[i]->m_vertexindex[j]]->m_pos;

				if (pMesh->m_type == eObjectType_SkinnedMesh)
				{
					pVertex->m_bw1 = pMesh->m_meshvertex[pMesh->m_meshface[i]->m_vertexindex[j]]->m_bw1;
					pVertex->m_bw2 = pMesh->m_meshvertex[pMesh->m_meshface[i]->m_vertexindex[j]]->m_bw2;
					pVertex->m_bw3 = pMesh->m_meshvertex[pMesh->m_meshface[i]->m_vertexindex[j]]->m_bw3;
					pVertex->m_weightIndices[0] = pMesh->m_meshvertex[pMesh->m_meshface[i]->m_vertexindex[j]]->m_weightIndices[0];
					pVertex->m_weightIndices[1] = pMesh->m_meshvertex[pMesh->m_meshface[i]->m_vertexindex[j]]->m_weightIndices[1];
					pVertex->m_weightIndices[2] = pMesh->m_meshvertex[pMesh->m_meshface[i]->m_vertexindex[j]]->m_weightIndices[2];
					pVertex->m_weightIndices[3] = pMesh->m_meshvertex[pMesh->m_meshface[i]->m_vertexindex[j]]->m_weightIndices[3];
				}
			}

			pVertex->m_normal = pMesh->m_meshface[i]->m_vertexnormal[j];

			if (pMesh->m_mesh_tvertex.size() != 0)
			{
				pVertex->u = pMesh->m_mesh_tvertex[pMesh->m_meshface[i]->m_TFace[j]]->m_u;
				pVertex->v = pMesh->m_mesh_tvertex[pMesh->m_meshface[i]->m_TFace[j]]->m_v;
			}

			// 2. 지금까지 조합된 Vertex중에 같은 Vertex가 있는지 검색한다.
			int index = 0;
			for (unsigned int vlistindex = 0; vlistindex < pMesh->m_opt_vertex.size(); vlistindex++, index++)
			{
				// 같은 버텍스가 있으면 지금 조합된것을 삭제하고 루프를 빠져나감.
				if (pMesh->m_opt_vertex[index]->m_pos == (pVertex)->m_pos &&
					pMesh->m_opt_vertex[index]->m_normal == (pVertex)->m_normal &&
					pMesh->m_opt_vertex[index]->u == (pVertex)->u &&
					pMesh->m_opt_vertex[index]->v == (pVertex)->v)
				{
					TRACE("같은 버텍스 삭제 : %d\n", index);
					delete pVertex;
					pVertex = NULL;
					break;
				}
			}

			// vertex가 삭제되지 않았다면 같은 vertex가 없다는 것이므로
			// 리스트에 추가한 뒤, 인덱스를 넣어준다 번호는 index
			if (pVertex != NULL)
			{
				pMesh->m_opt_vertex.push_back(pVertex);
				pMesh->m_opt_index[i].index[j] = index;
			}
			// vertex가 삭제되었을 때 -> 같은 Vertex가 있다는 것. 그에 해당하는 인덱스도 index다.
			else
			{
				pMesh->m_opt_index[i].index[j] = index;

			}
		}
	}

	pMesh->m_mesh_numfaces = pMesh->m_meshface.size();

	return false;
}

bool CASEParser::Convert_Optimize_Ver02(Mesh* pMesh)
{
	for (unsigned int i = 0; i < pMesh->m_meshvertex.size(); i++)
	{
		Vertex* pVertex = new Vertex;
		pVertex->m_pos = pMesh->m_meshvertex[i]->m_pos;
		pVertex->m_indices = pMesh->m_meshvertex[i]->m_indices;

		pMesh->m_opt_vertex.push_back(pVertex);
	}

	// 나머지는 face를 기준으로 한 인덱스로 찾아야 함
	unsigned int _faceCount = pMesh->m_meshface.size();
	/// 잠시 삭제
	///TRACE("FaceCount : %d", _faceCount);

	/// 스플릿된 버텍스의 번호 부여 및 개수를 측정하기 위함
	UINT split_Vertex_Num = 0 + pMesh->m_meshvertex.size();

	/// 각 페이스에 할당되어 있는 버텍스 노말값들을 비교하며 버텍스에 노말값을 채워준다
	/// 만약 버텍스에 노말값이 두개이상 들어가는 경우라면 스플릿을 해준다
	for (unsigned int i = 0; i < _faceCount; i++)
	{
		/// 잠시 삭제
		///TRACE("FaceNum : %d / %d\n", i, _faceCount);

		for (int j = 0; j < 3; j++)
		{
			int Vertex_num = pMesh->m_meshface[i]->m_vertexindex[j];

			Vertex* nowVertex = pMesh->m_opt_vertex[Vertex_num];

			if (nowVertex->m_isnormalset)
			{
				if (pMesh->m_opt_vertex[Vertex_num]->m_normal != pMesh->m_meshface[i]->m_vertexnormal[j])
				{
					bool Is_Data_Exist = false;

					if (split_Vertex_Num > pMesh->m_meshvertex.size())
					{
						for (UINT k = pMesh->m_meshvertex.size(); k < split_Vertex_Num; k++)
						{
							/// 만약 서로다른 버텍스 노말값으로 인해 생성된 데이터들 중에서도 또 같은값이 중복하여 있다면 생성을 해줄 필요가 없다
							if ((pMesh->m_opt_vertex[k]->m_indices == pMesh->m_meshface[i]->m_vertexindex[j]))
							{
								/// 노말값이 다를 때 새로 생성
								if ((pMesh->m_opt_vertex[k]->m_normal == pMesh->m_meshface[i]->m_vertexnormal[j]))
								{
									pMesh->m_meshface[i]->m_vertexindex[j] = k;
									Is_Data_Exist = true;
									break;
								}
							}
						}
					}

					/// 만약 이전에 생성되었던 버텍스가 없다면
					/// 현재 버텍스는 새로운 버텍스로 지정한다
					if (Is_Data_Exist == false)
					{
						Vertex* pVertex = new Vertex;
						pVertex->m_pos = nowVertex->m_pos;
						pVertex->m_indices = nowVertex->m_indices;
						pVertex->m_normal = pMesh->m_meshface[i]->m_vertexnormal[j];
						pVertex->m_isnormalset = true;

						if (pMesh->m_mesh_numtvertex)
						{
							pVertex->u = pMesh->m_mesh_tvertex[pMesh->m_meshface[i]->m_TFace[j]]->m_u;
							pVertex->v = pMesh->m_mesh_tvertex[pMesh->m_meshface[i]->m_TFace[j]]->m_v;
							pVertex->m_istextureset = true;
						}

						pMesh->m_opt_vertex.push_back(pVertex);

						// 매시에 맵핑되는 버텍스 번호도 바꿔줘야 하겠지?!
						pMesh->m_meshface[i]->m_vertexindex[j] = split_Vertex_Num;
						split_Vertex_Num++;

						continue;
					}
				}
			}

			/// 만약 텍스쳐가 셋팅 되어있더라도 uv값이 다를 때 새로 생성
			if (nowVertex->m_istextureset)
			{
				if ((nowVertex->u != pMesh->m_mesh_tvertex[pMesh->m_meshface[i]->m_TFace[j]]->m_u)
					|| (nowVertex->v != pMesh->m_mesh_tvertex[pMesh->m_meshface[i]->m_TFace[j]]->m_v))
				{
					Vertex* pVertex = new Vertex;
					pVertex->m_pos = nowVertex->m_pos;
					pVertex->m_indices = nowVertex->m_indices;
					pVertex->m_normal = pMesh->m_meshface[i]->m_vertexnormal[j];
					pVertex->m_isnormalset = true;
					pVertex->u = pMesh->m_mesh_tvertex[pMesh->m_meshface[i]->m_TFace[j]]->m_u;
					pVertex->v = pMesh->m_mesh_tvertex[pMesh->m_meshface[i]->m_TFace[j]]->m_v;
					pVertex->m_istextureset = true;
					pMesh->m_opt_vertex.push_back(pVertex);

					// 매시에 맵핑되는 버텍스 번호도 바꿔줘야 하겠지?!
					pMesh->m_meshface[i]->m_vertexindex[j] = split_Vertex_Num;
					split_Vertex_Num++;

					continue;
				}
			}

			if (!nowVertex->m_isnormalset)
			{
				nowVertex->m_normal = pMesh->m_meshface[i]->m_vertexnormal[j];
				nowVertex->m_isnormalset = true;
			}

			if (pMesh->m_mesh_numtvertex && !nowVertex->m_istextureset)
			{
				nowVertex->u = pMesh->m_mesh_tvertex[pMesh->m_meshface[i]->m_TFace[j]]->m_u;
				nowVertex->v = pMesh->m_mesh_tvertex[pMesh->m_meshface[i]->m_TFace[j]]->m_v;
				nowVertex->m_istextureset = true;
			}
		}
	}

	// 인덱스는 그냥 복사
	pMesh->m_opt_index = new IndexList[pMesh->m_meshface.size()];

	for (unsigned int i = 0; i < pMesh->m_meshface.size(); i++)
	{
		for (int j = 0; j < 3; j++)
		{
			pMesh->m_opt_index[i].index[j] = pMesh->m_meshface[i]->m_vertexindex[j];
		}
	}

	return FALSE;
}

bool CASEParser::ConvertAll(Mesh* pMesh)
{
	// 버텍스들은 일단 모두 복사
	for (unsigned int i = 0; i < pMesh->m_meshvertex.size(); i++)
	{
		Vertex* pvertex = new Vertex;
		pvertex->m_pos = pMesh->m_meshvertex[i]->m_pos;

		pMesh->m_opt_vertex.push_back(pvertex);
	}

	// 나머지는 face를 기준으로 한 인덱스로 찾아야 함
	unsigned int faceCount = pMesh->m_meshface.size();
	/// 잠시 삭제
	///TRACE("FaceCount : %d", faceCount);

	for (UINT i = 0; i < pMesh->m_meshface.size(); i++)
	{
		/// 잠시 삭제
		///TRACE("FaceNum : %d / %d\n", i, faceCount);

		for (int j = 0; j < 3; j++)
		{
			Vertex* nowVertex = pMesh->m_opt_vertex[pMesh->m_meshface[i]->m_vertexindex[j]];

			// face를 기준으로 버텍스의 노말을 넣어준다
			nowVertex->m_normal = pMesh->m_meshface[i]->m_vertexnormal[j];

			if (pMesh->m_mesh_tvertex.size() != 0)
			{
				nowVertex->u = pMesh->m_mesh_tvertex[pMesh->m_meshface[i]->m_TFace[j]]->m_u;
				nowVertex->v = pMesh->m_mesh_tvertex[pMesh->m_meshface[i]->m_TFace[j]]->m_v;
			}
		}
	}

	// 인덱스는 그냥 복사
	pMesh->m_opt_index = new IndexList[pMesh->m_meshface.size()];

	for (unsigned int i = 0; i < pMesh->m_meshface.size(); i++)
	{
		for (int j = 0; j < 3; j++)
		{
			pMesh->m_opt_index[i].index[j] = pMesh->m_meshface[i]->m_vertexindex[j];
		}
	}

	return FALSE;
}

void CASEParser::Get_NodeTM()
{
	// 파싱한 데이터를 결합하여 Mesh의 Node TM을 구한다
	for (UINT i = 0; i < m_MeshList.size(); i++)
	{
		XMMATRIX nodeTM(
			m_MeshList[i]->m_tm_row0.x, m_MeshList[i]->m_tm_row0.y, m_MeshList[i]->m_tm_row0.z, 0,
			m_MeshList[i]->m_tm_row1.x, m_MeshList[i]->m_tm_row1.y, m_MeshList[i]->m_tm_row1.z, 0,
			m_MeshList[i]->m_tm_row2.x, m_MeshList[i]->m_tm_row2.y, m_MeshList[i]->m_tm_row2.z, 0,
			m_MeshList[i]->m_tm_row3.x, m_MeshList[i]->m_tm_row2.y, m_MeshList[i]->m_tm_row2.z, 1);

		m_MeshList[i]->m_nodeTM = nodeTM;

		// 1. Scale TM 구하기
		Vector3 scaleValue = m_MeshList[i]->m_tm_scale;
		XMMATRIX scaleTM = XMMatrixScaling(scaleValue.x, scaleValue.y, scaleValue.z);

		// 2. Rotation TM 구하기
		Vector3 rotationValue = m_MeshList[i]->m_tm_rotaxis;
		Vector3 zeroAxis(0.0f, 0.0f, 0.0f);
		XMMATRIX rotationTM;

		// 2-1. 값이 0일 경우에는 단위 행렬을 넣어준다
		if (rotationValue == zeroAxis || m_MeshList[i]->m_tm_rotangle == 0)
		{
			XMMATRIX I = XMMatrixIdentity();
			rotationTM = I;
		}
		else
		{
			rotationTM = XMMatrixRotationAxis(m_MeshList[i]->m_tm_rotaxis, m_MeshList[i]->m_tm_rotangle);
		}

		// 3. Transform TM 구하기
		Vector3 transformValue = m_MeshList[i]->m_tm_pos;
		XMMATRIX transformTM = XMMatrixTranslation(transformValue.x, transformValue.y, transformValue.z);

		// 4. Node TM = Scale TM * Rotation TM * Position TM
		m_MeshList[i]->m_nodeTM = scaleTM * rotationTM * transformTM;

		// 5. Scale TM, Rotation TM, Translation의 역행렬 구하기
		XMMATRIX inverseScaleTM = XMMatrixInverse(NULL, scaleTM);
		XMMATRIX inverseRotationTM = XMMatrixInverse(NULL, rotationTM);
		XMMATRIX inverseTransformTM = XMMatrixInverse(NULL, transformTM);

		// translation 역행렬 * rotation 역행렬 * scale 역행렬 하기
		XMMATRIX inverseNodeTM = inverseTransformTM * inverseRotationTM * inverseScaleTM;
		m_MeshList[i]->m_inverseNodeTM = inverseNodeTM;

		for (UINT j = 0; j < m_MeshList[i]->m_opt_vertex.size(); j++)
		{
			m_MeshList[i]->m_opt_vertex[j]->m_pos = XMVector3Transform(m_MeshList[i]->m_opt_vertex[j]->m_pos, inverseNodeTM);
		}
	}
}

void CASEParser::Get_ParentMesh()
{
	for (UINT i = 0; i < m_MeshList.size(); i++)
	{
		// 현재 Mesh의 부모가 존재한다면
		if (m_MeshList[i]->m_isparentexist)
		{
			bool isFindParent = false;

			// MeshList를 한번 더 순회하여 부모를 찾는다
			for (UINT j = 0; j < m_MeshList.size(); j++)
			{
				// 부모를 찾았다면 탈출
				if (m_MeshList[j]->m_nodename._Equal(m_MeshList[i]->m_nodeparent))
				{
					m_MeshList[i]->m_parent = m_MeshList[j];
					isFindParent = true;
					break;
				}
			}

			// 부모를 찾는 순환문에서 부모를 찾지 못했다면 부모가 존재하지 않는다는것
			if (isFindParent == false)
			{
				m_MeshList[i]->m_isparentexist = false;
			}
		}
	}
}

bool CASEParser::IsParentExist(Mesh* mesh)
{
	if (mesh->m_parent != nullptr)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void CASEParser::Get_ParentMeshMaxCount()
{
	int maxCount = 0;

	for (UINT i = 0; i < m_MeshList.size(); i++)
	{
		if (maxCount < m_MeshList[i]->m_parentMeshCount)
		{
			maxCount = m_MeshList[i]->m_parentMeshCount;
		}
	}

	m_MaxParentMeshCount = maxCount;
}

void CASEParser::Get_CountParent()
{
	for (UINT i = 0; i < m_MeshList.size(); i++)
	{
		if (m_MeshList[i]->m_parent == nullptr)
		{
			continue;
		}

		Mesh* nowMesh = m_MeshList[i];

		while (true)
		{
			if (IsParentExist(nowMesh))
			{
				m_MeshList[i]->m_parentMeshCount++;
				nowMesh = nowMesh->m_parent;
			}
			else
			{
				break;
			}
		}
	}
}

void CASEParser::Get_LocalTM()
{
	// 자신의 Node TM에 부모의 Node TM을 곱해 얻는다
	for (UINT i = 0; i < m_MeshList.size(); i++)
	{
		// 현재 Mesh의 부모가 존재한다면
		if (m_MeshList[i]->m_isparentexist)
		{
			m_MeshList[i]->m_localTM = m_MeshList[i]->m_nodeTM * m_MeshList[i]->m_parent->m_inverseNodeTM;
		}
		// 부모가 존재하지 않는다면 자신의 Node TM을 Local TM으로 사용한다
		else
		{
			m_MeshList[i]->m_localTM = m_MeshList[i]->m_nodeTM;
		}
	}
}

ASEParser::Mesh* CASEParser::GetMesh(int index)
{
	return m_MeshList[index];
}

//--------------------------------------------------------------------------------------
// 재귀 호출됨을 전제로 하는 분기 함수이다
//
// 일단 읽고, 토큰에 따라 동작한다
// 재귀를 들어가는 조건은 { 을 만날 경우이다
// 리턴하는 조건은 '}'를 만나거나 TOKEND_END를 만났을 때이다
//
// 더불어, 재귀를 들어가기 전 토큰을 읽어, 지금 이 함수가 어떤 모드인지를 결정한다.
//--------------------------------------------------------------------------------------
void CASEParser::Parsing_DivergeRecursiveALL(int depth)
{
	// 지역변수들 선언
	// 재귀 돌리다가 일정 이상 루프를 도는 경우(오류)를 체크하기 위한 변수
	int i = 0;

	// 리스트에 데이터를 넣어야 할 때 쓰이는 리스트 카운터
	int listcount = 0;		// 매우 C스러운데 다른 해결책이 없을까?

	// 이것은 현 토큰이 어떤 종류인가를 판별해준다
	// 이것을 멤버 변수로 하고 재귀를 돌리는것은 위험하다? -> 잘못된 재귀방식이다?
	LONG nowtoken;

	Matrix TM1, TM2;
	Quaternion tempQT;
	Quaternion prevQT;
	Quaternion resultQT;

	int vertexIndex = 0;
	int faceNormalIndex = 0;
	int faceArrayIndex = 0;

	// 이것을 하면 한 개의 토큰을 읽고, 그 종류와 내용을 알 수 있다
	while (nowtoken = m_lexer->GetToken(m_TokenString), nowtoken != TOKEND_BLOCK_END)
	{
		// 일단 한 개의 토큰을 읽고, 그것이 괄호 닫기가 아니라면
		// 넘어온 토큰에 따라 처리해준다.
		static int iv = 0;

		switch (nowtoken)
		{
		case TOKEND_BLOCK_START:
			Parsing_DivergeRecursiveALL(depth++);
			break;

			//--------------------
			// 3DSMAX_ASCIIEXPORT
			//--------------------
		case TOKENR_3DSMAX_ASCIIEXPORT:
			m_data_asciiexport = Parsing_NumberLong();
			break;

			//--------------------
			// COMMENT
			//--------------------
		case TOKENR_COMMENT:
			Parsing_String();	// 그냥 m_TokenString에 읽어버리는 역할 뿐
			//AfxMessageBox( m_TokenString, NULL, NULL);		// 임시로 코멘트를 출력해본다
			break;

			//--------------------
			// SCENE
			//--------------------
		case TOKENR_SCENE:
			break;

		case TOKENR_SCENE_FILENAME:
			m_scenedata.m_filename = Parsing_String();		// 일관성 있는 함수의 사용을 위해 String과 Int도 만들어줬다
			break;

		case TOKENR_SCENE_FIRSTFRAME:
			m_scenedata.m_firstframe = Parsing_NumberLong();
			break;

		case TOKENR_SCENE_LASTFRAME:
			m_scenedata.m_lastframe = Parsing_NumberLong();
			break;

		case TOKENR_SCENE_FRAMESPEED:
			m_scenedata.m_framespeed = Parsing_NumberLong();
			break;

		case TOKENR_SCENE_TICKSPERFRAME:
			m_scenedata.m_ticksperframe = Parsing_NumberLong();
			break;

		case TOKENR_SCENE_MESHFRAMESTEP:
			m_scenedata.m_meshframestep = Parsing_NumberLong();
			break;

		case TOKENR_SCENE_KEYFRAMESTEP:
			m_scenedata.m_keyframestep = Parsing_NumberLong();
			break;

		case TOKENR_SCENE_BACKGROUND_STATIC:
		{
			m_scenedata.m_scene_background_static.x = Parsing_NumberFloat();
			m_scenedata.m_scene_background_static.y = Parsing_NumberFloat();
			m_scenedata.m_scene_background_static.z = Parsing_NumberFloat();
		}
		break;

		case TOKENR_SCENE_AMBIENT_STATIC:
		{
			m_scenedata.m_scene_ambient_static.x = Parsing_NumberFloat();
			m_scenedata.m_scene_ambient_static.y = Parsing_NumberFloat();
			m_scenedata.m_scene_ambient_static.z = Parsing_NumberFloat();
		}
		break;

		case TOKENR_SCENE_ENVMAP:
		{
			// 그냥 안의 내용을 읽어버린다 }가 나올때까지
			while (nowtoken = m_lexer->GetToken(m_TokenString), nowtoken != TOKEND_BLOCK_END)
			{

			}
		}
		break;

		//--------------------
		// MATERIAL_LIST
		//--------------------
		case TOKENR_MATERIAL_LIST:
			break;

		case TOKENR_MATERIAL_COUNT:
			m_materialcount = Parsing_NumberLong();
			break;

		case TOKENR_MATERIAL:
		{
			// 새 Material을 생성하고 번호 매겨주자
			Create_materialdata_to_list();
			m_materialdata->m_materialnumber = Parsing_NumberLong();
		}
		break;

		case TOKENR_MATERIAL_NAME:
			m_materialdata->m_material_name = Parsing_String();
			break;

		case TOKENR_MATERIAL_CLASS:
			m_materialdata->m_material_class = Parsing_String();
			break;

		case TOKENR_MATERIAL_AMBIENT:
		{
			m_materialdata->m_material_ambient.x = Parsing_NumberFloat();
			m_materialdata->m_material_ambient.y = Parsing_NumberFloat();
			m_materialdata->m_material_ambient.z = Parsing_NumberFloat();
		}
		break;

		case TOKENR_MATERIAL_DIFFUSE:
		{
			m_materialdata->m_material_diffuse.x = Parsing_NumberFloat();
			m_materialdata->m_material_diffuse.y = Parsing_NumberFloat();
			m_materialdata->m_material_diffuse.z = Parsing_NumberFloat();
		}
		break;

		case TOKENR_MATERIAL_SPECULAR:
		{
			m_materialdata->m_material_specular.x = Parsing_NumberFloat();
			m_materialdata->m_material_specular.y = Parsing_NumberFloat();
			m_materialdata->m_material_specular.z = Parsing_NumberFloat();
		}
		break;

		case TOKENR_MATERIAL_SHINE:
			m_materialdata->m_material_shine = Parsing_NumberFloat();
			break;

		case TOKENR_MATERIAL_SHINESTRENGTH:
			m_materialdata->m_material_shinestrength = Parsing_NumberFloat();
			break;

		case TOKENR_MATERIAL_TRANSPARENCY:
			m_materialdata->m_material_transparency = Parsing_NumberFloat();
			break;

		case TOKENR_MATERIAL_WIRESIZE:
			m_materialdata->m_material_wiresize = Parsing_NumberFloat();
			break;

		case TOKENR_NUMSUBMTLS:
			m_materialdata->m_numsubmtls = Parsing_NumberLong();
			break;

		case TOKENR_SUBMATERIAL:
			// (서브 매터리얼 번호를 읽고)
			break;

			/// MATERIAL - MAP_DIFFUSE 등등

		case TOKENR_MAP_SELFILLUM:
		{
			// 맵 하나를 만들고, 현재 Material에 넣어 준다
			m_materialdata->m_map_diffuse = new MaterialMap;
			m_materialdata->m_map_temp = m_materialdata->m_map_diffuse;	// 임시로 쓰이는 '현재' 맵 데이터
		}
		break;

		case TOKENR_MAP_SHINE:
		{
			// 맵 하나를 만들고, 현재 Material에 넣어 준다
			m_materialdata->m_map_diffuse = new MaterialMap;
			m_materialdata->m_map_temp = m_materialdata->m_map_diffuse;	// 임시로 쓰이는 '현재' 맵 데이터
		}
		break;

		case TOKENR_MAP_BUMP:
		{
			// 맵 하나를 만들고, 현재 Material에 넣어 준다
			m_materialdata->m_map_diffuse = new MaterialMap;
			m_materialdata->m_map_temp = m_materialdata->m_map_diffuse;	// 임시로 쓰이는 '현재' 맵 데이터
		}
		break;

		case TOKENR_MAP_OPACITY:
		{
			// 맵 하나를 만들고, 현재 Material에 넣어 준다
			m_materialdata->m_map_diffuse = new MaterialMap;
			m_materialdata->m_map_temp = m_materialdata->m_map_diffuse;	// 임시로 쓰이는 '현재' 맵 데이터
		}
		break;

		case TOKENR_MAP_REFLECT:
		{
			// 맵 하나를 만들고, 현재 Material에 넣어 준다
			m_materialdata->m_map_diffuse = new MaterialMap;
			m_materialdata->m_map_temp = m_materialdata->m_map_diffuse;	// 임시로 쓰이는 '현재' 맵 데이터
		}
		break;

		case TOKENR_MAP_GENERIC:
		{
			// 맵 하나를 만들고, 현재 Material에 넣어 준다
			m_materialdata->m_map_diffuse = new MaterialMap;
			m_materialdata->m_map_temp = m_materialdata->m_map_diffuse;	// 임시로 쓰이는 '현재' 맵 데이터
		}
		break;

		case TOKENR_MAP_SPECULAR:
		{
			// 맵 하나를 만들고, 현재 Material에 넣어 준다
			m_materialdata->m_map_diffuse = new MaterialMap;
			m_materialdata->m_map_temp = m_materialdata->m_map_diffuse;	// 임시로 쓰이는 '현재' 맵 데이터
		}
		break;

		case TOKENR_MAP_DIFFUSE:
		{
			// 맵 하나를 만들고, 현재 Material에 넣어 준다
			m_materialdata->m_map_diffuse = new MaterialMap;
			m_materialdata->m_map_temp = m_materialdata->m_map_diffuse;	// 임시로 쓰이는 '현재' 맵 데이터
		}
		break;

		case TOKENR_MAP_NAME:
			m_materialdata->m_map_temp->m_map_name = Parsing_String();
			break;

		case TOKENR_MAP_CLASS:
			m_materialdata->m_map_temp->m_map_class = Parsing_String();
			break;

		case TOKENR_MAP_SUBNO:
			m_materialdata->m_map_temp->m_subno = Parsing_NumberLong();
			break;

		case TOKENR_MAP_AMOUNT:
			m_materialdata->m_map_temp->m_map_amount = Parsing_NumberFloat();
			break;

		case TOKENR_BITMAP:
		{
			m_materialdata->m_map_temp->m_bitmap = Parsing_String();

			// 비트맵이 한개라도 있으면 텍스쳐가 있는 것이다
			m_materialdata->m_istextureexist = true;
		}
		break;

		case TOKENR_MAP_TYPE:
			m_materialdata->m_map_temp->m_map_type = Parsing_NumberLong();
			break;

		case TOKENR_UVW_U_OFFSET:
			m_materialdata->m_map_temp->m_uvw_u_offset = Parsing_NumberFloat();
			break;

		case TOKENR_UVW_V_OFFSET:
			m_materialdata->m_map_temp->m_uvw_v_offset = Parsing_NumberFloat();
			break;

		case TOKENR_UVW_U_TILING:
			m_materialdata->m_map_temp->m_u_tiling = Parsing_NumberFloat();
			break;

		case TOKENR_UVW_V_TILING:
			m_materialdata->m_map_temp->m_v_tiling = Parsing_NumberFloat();
			break;

		case TOKENR_UVW_ANGLE:
			m_materialdata->m_map_temp->m_uvw_angle = Parsing_NumberFloat();
			break;

		case TOKENR_UVW_BLUR:
			m_materialdata->m_map_temp->m_uvw_blur = Parsing_NumberFloat();
			break;

		case TOKENR_UVW_BLUR_OFFSET:
			m_materialdata->m_map_temp->m_uvw_blur_offset = Parsing_NumberFloat();
			break;

		case TOKENR_UVW_NOUSE_AMT:
			m_materialdata->m_map_temp->m_uvw_noise_amt = Parsing_NumberFloat();
			break;

		case TOKENR_UVW_NOISE_SIZE:
			m_materialdata->m_map_temp->m_uvw_noise_size = Parsing_NumberFloat();
			break;

		case TOKENR_UVW_NOISE_LEVEL:
			m_materialdata->m_map_temp->m_uvw_noise_level = Parsing_NumberFloat();
			break;

		case TOKENR_UVW_NOISE_PHASE:
			m_materialdata->m_map_temp->m_uvw_noise_phase = Parsing_NumberFloat();
			break;

		case TOKENR_BITMAP_FILTER:
			m_materialdata->m_map_temp->m_bitmap_filter = Parsing_NumberLong();
			break;

		case TOKENR_MATERIAL_SHADING:
			Parsing_String();
			break;

		case TOKENR_MATERIAL_XP_FALLOFF:
			Parsing_NumberFloat();
			break;

		case TOKENR_MATERIAL_SELFILLUM:
			Parsing_NumberFloat();
			break;

		case TOKENR_MATERIAL_FALLOFF:
			Parsing_String();
			break;

		case TOKENR_MATERIAL_XP_TYPE:
			Parsing_String();
			break;

			//--------------------
			// GEOMOBJECT
			//--------------------
		case TOKENR_GROUP:
			// 한 개의 그룹 시작, 이 다음에 이름이 스트링으로 나오기는 하는데
			break;

			//--------------------
			// HELPEROBJECT
			//--------------------
		case TOKENR_HELPEROBJECT:
		{
			// 일단 생성하고
			// 오브젝트의 타입 정해줌, 이것에 따라 서로 다른 파싱 모드 발동
			Create_onemesh_to_list();

			// 오브젝트의 타입 정해줌. 이것에 따라 서로 다른 파싱 모드 발동.
			m_parsingmode = eGeomobject;

			m_OneMesh->m_type = eObjectType::eObjectType_HelperObject;
		}
		break;

		case TOKENR_GEOMOBJECT:
		{
			/// 이 토큰을 만났다는건 새로운 메시가 생겼다는 것이다. 지역 변수로 mesh를 하나 선언, 그 포인터를 리스트에 넣고, m_onemesh에 그 포인터를 복사, 그대로 쓰면 될까?
			Create_onemesh_to_list();
			m_parsingmode = eHelperObject;
			m_OneMesh->m_type = eObjectType_Geomobject;
		}
		break;

		case TOKENR_NODE_NAME:
		{
			// 어쩄든 지금은 오브젝트들을 구별 할 수 있는 유일한 값이다.
			Parsing_String();

			// 모드에 따라 넣어야 할 곳이 다르다.
			switch (m_parsingmode)
			{
			case eAnimation:
				m_animation->m_nodename = m_TokenString;
				break;

			case eGeomobject:
			case eHelperObject:
				m_OneMesh->m_nodename = m_TokenString;
				break;

			case eShape:
				m_OneMesh->m_nodename = m_TokenString;
				break;

			case eCamera:
				// 카메라는 NodeTM이 두번 나온다. 두번째라면 넣지 않는다.
				if (m_OneMesh->m_camera_isloadTarget) break;
				m_OneMesh->m_nodename = m_TokenString;
				break;

			case eLight:
				// 마찬가지의 이유로 Light도 넣지 않는다.
				if (m_OneMesh->m_light_isloadTarget) break;
				m_OneMesh->m_nodename = m_TokenString;
				break;

			default:
				break;
			}
		}
		break;

		case TOKENR_HELPER_CLASS:
			Parsing_String();
			break;

		case TOKENR_NODE_PARENT:
		{
			// 현 노드의 부모 노드의 정보.
			m_OneMesh->m_isparentexist = true;

			// 일단 입력을 하고, 나중에 정리하자
			Parsing_String();
			m_OneMesh->m_nodeparent = m_TokenString;
		}
		break;

		//--------------------
		// NODE_TM
		//--------------------
		case TOKENR_NODE_TM:
			//m_parsingmode	= eGeomobject;
			// NODE_TM으로 진입 후 NODE_NAME이 한번 더 나온다
			// Animation과도 구별을 해야 하기 때문에 이렇게 모드를 적어준다

			// 게다가,
			// 카메라는 NodeTM이 두번 나온다, 두번째라면 넣지 않는다
			// 아예 이 재귀에서 리턴을 시키고 싶지만 중간에 읽는것을 무시해야 하기 때문에
			//if (m_onemesh->m_camera_isloadTarget)
			//	{
			//	'}'이 나올때까지 나오는건 무조건 무시! 뭐 이런거 안되나
			// 재귀구조의 문제점이다
			break;

		case TOKENR_INHERIT_POS:
		{
			// 카메라는 NodeTM이 두번 나온다. 두번째라면 넣지 않는다.
			if (m_OneMesh->m_camera_isloadTarget) break;
			if (m_OneMesh->m_light_isloadTarget) break;

			m_OneMesh->m_inherit_pos.x = Parsing_NumberFloat();
			m_OneMesh->m_inherit_pos.y = Parsing_NumberFloat();
			m_OneMesh->m_inherit_pos.z = Parsing_NumberFloat();
		}
		break;

		case TOKENR_INHERIT_ROT:
		{
			if (m_OneMesh->m_camera_isloadTarget) break;
			if (m_OneMesh->m_light_isloadTarget) break;

			m_OneMesh->m_inherit_rot.x = Parsing_NumberFloat();
			m_OneMesh->m_inherit_rot.y = Parsing_NumberFloat();
			m_OneMesh->m_inherit_rot.z = Parsing_NumberFloat();
		}
		break;

		case TOKENR_INHERIT_SCL:
		{
			if (m_OneMesh->m_camera_isloadTarget) break;
			if (m_OneMesh->m_light_isloadTarget) break;

			m_OneMesh->m_inherit_scl.x = Parsing_NumberFloat();
			m_OneMesh->m_inherit_scl.y = Parsing_NumberFloat();
			m_OneMesh->m_inherit_scl.z = Parsing_NumberFloat();
		}
		break;

		case TOKENR_TM_ROW0:
		{
			if (m_OneMesh->m_camera_isloadTarget) break;
			if (m_OneMesh->m_light_isloadTarget) break;

			// SKIN_INITTM일 경우 무시한다.
			if (m_parsingmode == eSkin_InitTM) break;
			if (m_parsingmode == eBone_List) break;

			m_OneMesh->m_tm_row0.x = Parsing_NumberFloat();
			m_OneMesh->m_tm_row0.y = Parsing_NumberFloat();
			m_OneMesh->m_tm_row0.z = Parsing_NumberFloat();
		}
		break;

		case TOKENR_TM_ROW1:
		{
			if (m_OneMesh->m_camera_isloadTarget) break;
			if (m_OneMesh->m_light_isloadTarget) break;

			if (m_parsingmode == eSkin_InitTM) break;
			if (m_parsingmode == eBone_List) break;

			m_OneMesh->m_tm_row1.x = Parsing_NumberFloat();
			m_OneMesh->m_tm_row1.y = Parsing_NumberFloat();
			m_OneMesh->m_tm_row1.z = Parsing_NumberFloat();
		}
		break;

		case TOKENR_TM_ROW2:
		{
			if (m_OneMesh->m_camera_isloadTarget) break;
			if (m_OneMesh->m_light_isloadTarget) break;

			if (m_parsingmode == eSkin_InitTM) break;
			if (m_parsingmode == eBone_List) break;

			m_OneMesh->m_tm_row2.x = Parsing_NumberFloat();
			m_OneMesh->m_tm_row2.y = Parsing_NumberFloat();
			m_OneMesh->m_tm_row2.z = Parsing_NumberFloat();
		}
		break;

		case TOKENR_TM_ROW3:
		{
			if (m_OneMesh->m_camera_isloadTarget) break;
			if (m_OneMesh->m_light_isloadTarget) break;

			if (m_parsingmode == eSkin_InitTM) break;
			if (m_parsingmode == eBone_List) break;

			m_OneMesh->m_tm_row3.x = Parsing_NumberFloat();
			m_OneMesh->m_tm_row3.y = Parsing_NumberFloat();
			m_OneMesh->m_tm_row3.z = Parsing_NumberFloat();
		}
		break;

		case TOKENR_TM_POS:
		{
			if (m_OneMesh->m_camera_isloadTarget) break;
			if (m_OneMesh->m_light_isloadTarget) break;

			m_OneMesh->m_tm_pos.x = Parsing_NumberFloat();
			m_OneMesh->m_tm_pos.z = Parsing_NumberFloat();
			m_OneMesh->m_tm_pos.y = Parsing_NumberFloat();
		}
		break;

		case TOKENR_TM_ROTAXIS:
		{
			if (m_OneMesh->m_camera_isloadTarget) break;
			if (m_OneMesh->m_light_isloadTarget) break;

			m_OneMesh->m_tm_rotaxis.x = Parsing_NumberFloat();
			m_OneMesh->m_tm_rotaxis.z = Parsing_NumberFloat();
			m_OneMesh->m_tm_rotaxis.y = Parsing_NumberFloat();
		}
		break;

		case TOKENR_TM_ROTANGLE:
		{
			if (m_OneMesh->m_camera_isloadTarget) break;
			if (m_OneMesh->m_light_isloadTarget) break;

			m_OneMesh->m_tm_rotangle = Parsing_NumberFloat();
		}
		break;

		case TOKENR_TM_SCALE:
		{
			if (m_OneMesh->m_camera_isloadTarget) break;
			if (m_OneMesh->m_light_isloadTarget) break;

			m_OneMesh->m_tm_scale.x = Parsing_NumberFloat();
			m_OneMesh->m_tm_scale.z = Parsing_NumberFloat();
			m_OneMesh->m_tm_scale.y = Parsing_NumberFloat();
		}
		break;

		case TOKENR_TM_SCALEAXIS:
		{
			if (m_OneMesh->m_camera_isloadTarget) break;
			if (m_OneMesh->m_light_isloadTarget) break;

			m_OneMesh->m_tm_scaleaxis.x = Parsing_NumberFloat();
			m_OneMesh->m_tm_scaleaxis.z = Parsing_NumberFloat();
			m_OneMesh->m_tm_scaleaxis.y = Parsing_NumberFloat();
		}
		break;

		case TOKENR_TM_SCALEAXISANG:
		{
			if (m_OneMesh->m_camera_isloadTarget) break;
			if (m_OneMesh->m_light_isloadTarget) break;

			m_OneMesh->m_tm_scaleaxisang = Parsing_NumberFloat();

			// 현재 카메라 상태였다면 이미 노드를 읽은 것으로 표시해준다.
			if (m_OneMesh->m_type == eObjectType_Camera) m_OneMesh->m_camera_isloadTarget = true;
			if (m_OneMesh->m_type == eObjectType_Light) m_OneMesh->m_light_isloadTarget = true;
		}
		break;

		case TOKENR_BOUNDINGBOX_MIN:
			Parsing_NumberVector3();
			break;

		case TOKENR_BOUNDINGBOX_MAX:
			Parsing_NumberVector3();
			break;

			//--------------------
			// MESH
			//--------------------
		case TOKENR_MESH:
			break;

		case TOKENR_TIMEVALUE:
		{
			if (m_OneMesh->m_light_isAnimate == true)
			{
				m_OneMesh->m_lightanimation->m_timevalue = Parsing_NumberFloat();
			}
			else
			{
				m_OneMesh->m_timevalue = Parsing_NumberFloat();
			}
		}
		break;

		case TOKENR_MESH_NUMBONE:
		{
			// 이게 있다면 이것은 Skinned Mesh라고 단정을 짓는다.
			m_OneMesh->m_is_skinningobject = true;
			m_OneMesh->m_type = eObjectType_SkinnedMesh;

			// 내용 입력
			m_OneMesh->m_numbone = Parsing_NumberLong();
		}
		break;

		case TOKENR_MESH_NUMSKINWEIGHT:
			m_OneMesh->m_numskinweight = Parsing_NumberLong();
			break;

		case TOKENR_MESH_NUMVERTEX:
			m_OneMesh->m_mesh_numvertex = Parsing_NumberLong();
			break;

		case TOKENR_MESH_NUMFACES:
			m_OneMesh->m_mesh_numfaces = Parsing_NumberLong();
			break;

			//--------------------
			// MESH_VERTEX_LIST
			//--------------------
		case TOKENR_MESH_VERTEX_LIST:
			// 버텍스의 값들을 집어넣어야 하는데
			// 이미 벡터로 선언이 돼 있으므로 그냥 넣으면 된다
			break;

		case TOKENR_MESH_VERTEX:
		{
			vertexIndex = Parsing_NumberInt();			// 첫번째는 버텍스의 번호

			Create_onevertex_to_list();

			// 루프를 돌까도 생각해봤지만, 할당된 크기만 정확하다면
			// vertex정보 자체에 번호가 있기 때문에 카운트도, 루프도 필요없다!

			// 버텍스 벡터에 데이터를 하나 넣자

			// 데이터 입력
			m_OneMesh->m_meshvertex[vertexIndex]->m_pos = Parsing_NumberVector3();
		}
		break;

		//--------------------
		// Bone
		//--------------------
		case TOKENR_SKIN_INITTM:
			m_parsingmode = eSkin_InitTM;
			break;

		case TOKENR_BONE_LIST:
			m_parsingmode = eBone_List;
			break;

		case TOKENR_BONE:
		{
			/// 모드 체인지 해 주고, Bone을 소유하고 있다는 것은 이것은 스키닝 오브젝트라는 것이다.
			m_parsingmode = eBone_List;
			m_OneMesh->m_is_skinningobject = true;
			m_OneMesh->m_type = eObjectType_SkinnedMesh;

			// 본 하나를 만들어서 임시 포인터 보관, 벡터에 넣고
			Bone* _tempbone = new Bone;
			m_OneMesh->m_bone = _tempbone;
			m_OneMesh->m_vector_bone_list.push_back(_tempbone);
			// Bone의 넘버를 읽어 주자
			m_OneMesh->m_bone->m_bone_number = Parsing_NumberLong();
		}
		break;

		//이 다음에 본의 이름을 넣어야 한다. 하지만 {를 한 개 더 열었으므로 임시 포인터 변수로서 보관해야겠지.
		case TOKENR_BONE_NAME:
			m_OneMesh->m_bone->m_bone_name = Parsing_String();

		case TOKENR_BONE_PROPERTY:
			// 이 다음 ABSOLUTE가 나오기는 하는데, 쓸 일이 없다.
			break;
			// 다음에는 TM_ROW0~3이 나오는데 역시 무시됨..

		case TOKENR_MESH_WVERTEXS:
			break;

		case TOKENR_MESH_WEIGHT:
		{
			// 버텍스 하나의 정보를 만들어서 리스트에 넣음
			VertexWeight* tempWVertex = new VertexWeight;
			m_OneMesh->m_wvertex = tempWVertex;
			m_OneMesh->m_vector_wvertexs.push_back(tempWVertex);
			m_OneMesh->m_wvertex->m_wvertex_number = Parsing_NumberLong();
		}
		break;

		case TOKENR_BONE_BLENGING_WEIGHT:
		{
			// 대체 몇 단계를 들어가는거야...
			// 가중치 한개를 만들어서 리스트에 넣는다
			Weight* _tempWeight = new Weight;
			m_OneMesh->m_wvertex->m_temp_bone_blend_weight = _tempWeight;
			_tempWeight->m_bone_number = Parsing_NumberLong();
			_tempWeight->m_bone_weight = Parsing_NumberFloat();
			m_OneMesh->m_wvertex->m_bone_blending_weight.push_back(_tempWeight);
			/// 헥 헥....
		}
		break;

		//--------------------
		// MESH_FACE_LIST
		//--------------------
		case TOKENR_MESH_FACE_LIST:
			break;

		case TOKENR_MESH_FACE:
		{
			// 새로운 Face 생성
			Face* newFace = new Face;

			// Face의 번호
			int faceIndex = Parsing_NumberLong();

			// A:를 읽고
			Parsing_String();
			newFace->m_vertexindex[0] = Parsing_NumberLong();

			// B:
			Parsing_String();
			newFace->m_vertexindex[2] = Parsing_NumberLong();

			// C:
			Parsing_String();
			newFace->m_vertexindex[1] = Parsing_NumberLong();

			// 뒤에 정보가 더 있지만 default에 의해 스킵될 것이다

			static int count = 0;
			count++;

			// 벡터에 넣어준다
			m_OneMesh->m_meshface.push_back(newFace);
		}
		break;

		case TOKENR_MESH_NUMTVERTEX:
			m_OneMesh->m_mesh_numtvertex = Parsing_NumberLong();
			break;

			//--------------------
			// MESH_TVERTLIST
			//--------------------
		case TOKENR_MESH_TVERTLIST:
			break;

		case TOKENR_MESH_TVERT:
		{
			// 버텍스의 인덱스가 나오는데 어차피 순서와 같으므로 버린다
			Parsing_NumberLong();

			// 새로운 TVertex를 만들어서 벡터에 넣는다
			COneTVertex* textureVertexValue = new COneTVertex;

			textureVertexValue->m_u = Parsing_NumberFloat();
			textureVertexValue->m_v = 1.0f - Parsing_NumberFloat();			/// y-z 뒤집힘 때문에 v도 1-v을 해줘야 함
			textureVertexValue->m_w = Parsing_NumberFloat();

			m_OneMesh->m_mesh_tvertex.push_back(textureVertexValue);
		}
		break;

		case TOKENR_MESH_NUMTVFACES:
			m_OneMesh->m_mesh_tvfaces = Parsing_NumberLong();
			break;

			//--------------------
			// MESH_NORMALS
			//--------------------
		case TOKENR_MESH_NORMALS:
			m_OneMesh->m_mesh_vertexnormal = new Vector3[m_OneMesh->m_mesh_numvertex];
			break;

		case TOKENR_MESH_FACENORMAL:
		{
			// 페이스 노말 데이터 입력
			faceNormalIndex = Parsing_NumberLong();			// Face의 번호인데...

			if (21680 < faceNormalIndex)
			{
				int kk = 0;
			}

			m_OneMesh->m_meshface[faceNormalIndex]->m_facenormal.x = Parsing_NumberFloat();
			m_OneMesh->m_meshface[faceNormalIndex]->m_facenormal.z = Parsing_NumberFloat();
			m_OneMesh->m_meshface[faceNormalIndex]->m_facenormal.y = Parsing_NumberFloat();

			// 새로운 face를 읽었으므로 vertex인덱스를 초기화(바로 아래구문 보면 안다)
			iv = 0;
		}
		break;

		case TOKENR_MESH_VERTEXNORMAL:
		{
			// facenumber 는 위에서 연계됨

			/// 재귀형태라 021식으로 읽으려면..
			int _convertedIndex = 0;
			if (iv == 0) _convertedIndex = 0;
			else if (iv == 1) _convertedIndex = 2;
			else if (iv == 2) _convertedIndex = 1;
			else
			{
				// error
				_convertedIndex = 0;
			}

			///	일단 숫자 하나를 읽는데 이것은 버텍스의 인덱스.
			vertexIndex = Parsing_NumberLong();
			m_OneMesh->m_meshface[faceNormalIndex]->m_vertexnormal[_convertedIndex].x = Parsing_NumberFloat();
			m_OneMesh->m_meshface[faceNormalIndex]->m_vertexnormal[_convertedIndex].z = Parsing_NumberFloat();
			m_OneMesh->m_meshface[faceNormalIndex]->m_vertexnormal[_convertedIndex].y = Parsing_NumberFloat();

			// face한개당 vertex가 3개 있기 때문에
			iv++;
		}
		break;

		//--------------------
		// MESH_TFACELIST
		//--------------------
		case TOKENR_MESH_TFACELIST:
			break;

		case TOKENR_MESH_TFACE:
		{
			int tfaceIndex = Parsing_NumberLong();	// Face의 번호. TVFaces는 VFaces와 갯수가 같다고 가정한다. (그렇다면 바로 위의 NUMTVFACES는 왜 있을까? 하는 의문이 들기는 하지만.)
			m_OneMesh->m_meshface[tfaceIndex]->m_TFace[0] = Parsing_NumberLong();
			m_OneMesh->m_meshface[tfaceIndex]->m_TFace[2] = Parsing_NumberLong();	/// 021인것은 y-z좌표계 변환 때문
			m_OneMesh->m_meshface[tfaceIndex]->m_TFace[1] = Parsing_NumberLong();
		}
		break;

		//--------------------
		// Shape
		//--------------------
		case TOKENR_SHAPEOBJECT:
		{
			// 파서 내부에서는 모든 오브젝트가 하나로 관리된다.
			Create_onemesh_to_list();

			m_OneMesh->m_type = eObjectType_Shape;

			// 모드도 바꿔 준다.
			m_parsingmode = eShape;
		}
		break;

		case TOKENR_SHAPE_LINECOUNT:
			m_OneMesh->m_shape_linecount = Parsing_NumberLong();
			break;

		case TOKENR_SHAPE_LINE:
		{
			// 리스트에 한 개의 라인을 추가한다.
			ShapeLine* temp = new ShapeLine;
			m_nowshapeline = temp;
			m_nowshapeline->m_line_number = Parsing_NumberLong();		// 현재 무슨 라인인가?
			m_OneMesh->m_vector_shape_line.push_back(m_nowshapeline);
		}
		break;

		case TOKENR_SHAPE_CLOSED:
			m_nowshapeline->m_shape_closed = true;
			break;

		case TOKENR_SHAPE_VERTEXCOUNT:
			m_nowshapeline->m_shape_vertexcount = Parsing_NumberLong();
			break;

		case TOKENR_SHAPE_VERTEX_KNOT:
		{
			ShapeVertex* temp = new ShapeVertex;
			temp->m_isknot = true;
			temp->m_indexnum = Parsing_NumberLong();
			//temp->m_pos.x = Parsing_NumberFloat();
			//temp->m_pos.z = Parsing_NumberFloat();
			//temp->m_pos.y = Parsing_NumberFloat();
			temp->m_pos = Parsing_NumberVector3();		/// 만들었으니 한번 써 봤다. 나중에 다 이것으로 대체하자.

			m_nowshapeline->m_shapevertex.push_back(temp);
		}
		break;

		case TOKENR_SHAPE_VERTEX_INTERP:
		{
			ShapeVertex* temp = new ShapeVertex;
			temp->m_isknot = false;
			temp->m_indexnum = Parsing_NumberLong();
			temp->m_pos = Parsing_NumberVector3();

			m_nowshapeline->m_shapevertex.push_back(temp);
		}
		break;

		//--------------------
		// Camera
		//--------------------
		case TOKENR_CAMERAOBJECT:
		{
			// 카메라 오브젝트도 결국 COneObject로 받아서 써야한다.
			Create_onemesh_to_list();

			m_OneMesh->m_type = eObjectType_Camera;

			// 모드도 바꿔 준다.
			m_parsingmode = eCamera;
		}
		break;

		case TOKENR_CAMERA_TYPE:
		{
			Parsing_String();
			m_OneMesh->m_camera_type = m_TokenString;
		}
		break;

		case TOKENR_CAMERA_SETTINGS:
			// 카메라 셋팅을 읽는다. 별 다른건 없다.
			break;

			//case TOKENR_TIMEVALUE:		//-> 이건 이미 MESH에 있다.

		case TOKENR_CAMERA_NEAR:
			m_OneMesh->m_camera_near = Parsing_NumberFloat();
			break;

		case TOKENR_CAMERA_FAR:
			m_OneMesh->m_camera_far = Parsing_NumberFloat();
			break;

		case TOKENR_CAMERA_FOV:
			m_OneMesh->m_camera_fov = Parsing_NumberFloat();
			break;

		case TOKENR_CAMERA_TDIST:
			m_OneMesh->m_camera_tdist = Parsing_NumberFloat();
			break;

			//--------------------
			// Light
			//--------------------
		case TOKENR_LIGHTOBJECT:
		{
			// Light 오브젝트도 마찬가지로 COneObject로 읽자.
			Create_onemesh_to_list();

			m_OneMesh->m_type = eObjectType_Light;

			m_parsingmode = eLight;
		}
		break;

		case TOKENR_LIGHT_TYPE:
		{
			Parsing_String();
			m_OneMesh->m_light_type = m_TokenString;	// 아마도 target
		}
		break;

		case TOKENR_LIGHT_SHADOWS:
		{
			Parsing_String();
			m_OneMesh->m_light_shadows = m_TokenString;	// 아마도 target
		}
		break;

		case TOKENR_LIGHT_USELIGHT:
			m_OneMesh->m_light_uselight = Parsing_NumberLong();
			break;

		case TOKENR_LIGHT_SPOTSHAPE:
		{
			Parsing_String();
			m_OneMesh->m_light_spotshape = m_TokenString;	// 아마도 target
		}
		break;

		case TOKENR_LIGHT_USEGLOBAL:
			m_OneMesh->m_light_useglobal = Parsing_NumberLong();
			break;

		case TOKENR_LIGHT_ABSMAPBIAS:
			m_OneMesh->m_light_absmapbias = Parsing_NumberLong();
			break;

		case TOKENR_LIGHT_OVERSHOOT:
			m_OneMesh->m_light_overshoot = Parsing_NumberLong();
			break;

		case TOKENR_LIGHT_SETTINGS:
		{
			// Light Setting으로 돌입
			// 돌입하면서, 하나의 Setting클래스 포인터를 만들고,
			// 그 포인터를 리스트에 넣는다.

			// 새로운 포인터를 만들고, 그것을 리스트에 넣는다.
			Light_AnimationData* temp;
			temp = new Light_AnimationData;
			m_OneMesh->m_lightanimation = temp;
			m_OneMesh->m_list_lightanimation.push_back(temp);
		}
		break;

		case TOKENR_LIGHT_COLOR:
		{
			m_OneMesh->m_lightanimation->m_light_color.x = Parsing_NumberFloat();
			m_OneMesh->m_lightanimation->m_light_color.y = Parsing_NumberFloat();
			m_OneMesh->m_lightanimation->m_light_color.z = Parsing_NumberFloat();
		}
		break;

		case TOKENR_LIGHT_INTENS:
			m_OneMesh->m_lightanimation->m_light_intens = Parsing_NumberFloat();
			break;

		case TOKENR_LIGHT_ASPECT:
			m_OneMesh->m_lightanimation->m_light_aspect = Parsing_NumberFloat();
			break;

		case TOKENR_LIGHT_HOTSPOT:
			m_OneMesh->m_lightanimation->m_light_hotspot = Parsing_NumberFloat();
			break;

		case TOKENR_LIGHT_FALLOFF:
			m_OneMesh->m_lightanimation->m_light_falloff = Parsing_NumberFloat();
			break;

		case TOKENR_LIGHT_TDIST:
			m_OneMesh->m_lightanimation->m_light_tdist = Parsing_NumberFloat();
			break;

		case TOKENR_LIGHT_MAPBIAS:
			m_OneMesh->m_lightanimation->m_light_mapbias = Parsing_NumberFloat();
			break;

		case TOKENR_LIGHT_MAPRANGE:
			m_OneMesh->m_lightanimation->m_light_maprange = Parsing_NumberFloat();
			break;

		case TOKENR_LIGHT_MAPSIZE:
			m_OneMesh->m_lightanimation->m_light_mapsize = Parsing_NumberLong();
			break;

		case TOKENR_LIGHT_RAYBIAS:
			m_OneMesh->m_lightanimation->m_light_raybias = Parsing_NumberFloat();
			break;

		case TOKENR_LIGHT_ANIMATION:
		{
			// 애니메이션 돌입함을 알리고, 그 다음부터 나오는 LIGHT_SETTING은
			// 리스트를 만들고서, 넣어야 한다.
			m_OneMesh->m_light_isAnimate = true;
		}
		break;

		//--------------------
		// Animation
		//--------------------
		case TOKENR_TM_ANIMATION:
		{
			// 모드 설정 해 주고(NODE_NAME때문이다)
			m_parsingmode = eAnimation;

			// 애니메이션 데이터를 생성, 리스트에 넣어준다.
			Create_animationdata_to_list();

			// 애니메이션 데이터가 있음을 기록한다.
			m_OneMesh->m_isAnimated = true;
			m_animation->m_ticksperFrame = m_scenedata.m_ticksperframe;
			m_animation->m_minKeyframe = m_scenedata.m_ticksperframe;
		}
		break;

		case TOKENR_CONTROL_POS_TRACK:/// 이 세가지는 그냥 괄호를 여는 것밖에는...
			break;

		case TOKENR_CONTROL_ROT_TRACK:
			break;

		case TOKENR_CONTROL_SCALE_TRACK:
			break;

		case TOKENR_CONTROL_POS_SAMPLE:
		{
			// 포지션을 하나 만들고, 읽고, 벡터에 넣는다
			Animation_pos* temp = new Animation_pos;
			temp->m_time = Parsing_NumberInt();
			temp->m_pos.x = Parsing_NumberFloat();
			temp->m_pos.z = Parsing_NumberFloat();
			temp->m_pos.y = Parsing_NumberFloat();

			// m_position은 vector이다. 뒤에 한 개를 추가시켜 준다.
			m_animation->m_position.push_back(temp);
		}
		break;

		case TOKENR_CONTROL_ROT_SAMPLE:
		{
			// 로테이션을 하나 만들고, 읽고, 벡터에 넣는다
			Animation_rot* temp = new Animation_rot;
			temp->m_time = Parsing_NumberInt();
			temp->m_rot.x = Parsing_NumberFloat();
			temp->m_rot.z = Parsing_NumberFloat();
			temp->m_rot.y = Parsing_NumberFloat();
			temp->m_angle = Parsing_NumberFloat();

			/// 지금까지의 내용을 읽어서 누적, 쿼터니언으로 만들어 넣는다.
			//Quaternion tempQT;	(앞쪽에 선언돼있다)
			//Quaternion prevQT;
			//Quaternion resultQT;

			// 현재 쿼터니온을 구해서,
			tempQT = XMQuaternionRotationAxis(temp->m_rot, temp->m_angle);

			/// 쿼터니온이 제대로 옮겨졌는지 확인했다.
			TM1 = XMMatrixRotationAxis(temp->m_rot, temp->m_angle);
			TM2 = XMMatrixRotationQuaternion(tempQT);

			// 벡터의 사이즈가 0이라면 이전 내용이 없는 것이므로 그냥 넣고
			if (m_animation->m_rotation.size() == 0)
			{
				// 현재 값을 그대로 넣어준다.
				temp->m_rotQT_accumulation = tempQT;
				m_animation->m_rotation.push_back(temp);
			}
			else
			{
				// 아니라면 이전 내용이 있는 것이므로 이전 내용과 Multiply
				// 이건 이전 내용
				prevQT = m_animation->m_rotation[m_animation->m_rotation.size() - 1]->m_rotQT_accumulation;
				resultQT = XMQuaternionMultiply(prevQT, tempQT);
				temp->m_rotQT_accumulation = resultQT;
				m_animation->m_rotation.push_back(temp);
			}
		}
		break;

		case TOKENR_CONTROL_SCALE_SAMPLE:
		{
			// 스케일을 하나 만들고 벡터에 넣는다
			Animation_scl* temp = new Animation_scl;
			temp->m_time = Parsing_NumberInt();
			temp->m_scale = Parsing_NumberVector3();
			temp->m_scaleaxis = Parsing_NumberVector3();
			temp->m_scaleaxisang = Parsing_NumberFloat();

			// 현재 쿼터니온
			tempQT = XMQuaternionRotationAxis(temp->m_scaleaxis, temp->m_scaleaxisang);

			// 벡터의 사이즈가 0이라면 이전 내용이 없는 것이므로 그냥
			if (m_animation->m_scale.size() == 0)
			{
				// 현재 값을 그대로 넣어준다.
				temp->m_scalerotQT_accumulation = tempQT;
			}
			else
			{
				// 아니라면 이전 내용이 있는 것이므로 이전 내용과 Multiply
				// 이건 이전 내용
				prevQT = m_animation->m_scale[m_animation->m_scale.size() - 1]->m_scalerotQT_accumulation;
				resultQT = XMQuaternionMultiply(prevQT, tempQT);
				temp->m_scalerotQT_accumulation = resultQT;

			}

			// 벡터에 넣어준다.
			m_animation->m_scale.push_back(temp);
		}
		break;

		case TOKENR_WIREFRAME_COLOR:
			m_OneMesh->m_wireframe_color.x = Parsing_NumberFloat();
			m_OneMesh->m_wireframe_color.y = Parsing_NumberFloat();
			m_OneMesh->m_wireframe_color.z = Parsing_NumberFloat();
			break;

		case TOKENR_MESH_NUMCVERTEX:
			m_OneMesh->m_mesh_numcvertex = Parsing_NumberLong();
			break;

		// 나머지들?
		case TOKENR_PROP_MOTIONBLUR:
			m_OneMesh->m_prop_motionblur = Parsing_NumberLong();
			break;

		case TOKENR_PROP_CASTSHADOW:
			m_OneMesh->m_prop_castshadow = Parsing_NumberLong();
			break;

		case TOKENR_PROP_RECVSHADOW:
			m_OneMesh->m_prop_recvshadow = Parsing_NumberLong();
			break;

		case TOKENR_MATERIAL_REF:
			m_OneMesh->m_material_ref = Parsing_NumberLong();
			return;

		case TOKEND_END:
		{
			// 아마도 이건 파일의 끝이 나타났을때인것 같은데, while을 탈출해야 하는데?
			//AfxMessageBox("파일의 끝을 본 것 같습니다!", MB_OK, NULL);
			/// 잠시 삭제
			//TRACE("TRACE: 파싱중: 파일의 끝을 봤습니다!\n");
			return;
		}
		break;

		// 위의 아무것도 해당하지 않을때
		default:
			// 아무것도 하지 않는다
			break;

		} // switch()

		///-----------------------------------------------
		/// 안전 코드
		i++;
		if (i > 1000000)
		{
			// 루프를 1000000번이상이나 돌 이유가 없다, (데이터가 100000개가 아닌이상)
			// 만약 1000000이상 돌았다면 확실히 뭔가 문제가 있는 것이므로
			TRACE("루프를 백만번 돌았습니다!");
			return;
		}
		///-----------------------------------------------

	} // while()

	// 여기까지 왔다면 while()을 벗어났다는 것이고 그 말은
	// 괄호를 닫았다는 것이므로
	// 리턴하자 (재귀함수)
	return;
}

///--------------------------------------------------
/// parsing을 위한 단위별 함수들
///--------------------------------------------------
int CASEParser::Parsing_NumberInt()
{
	LONG token;
	int tempNumber;

	token = m_lexer->GetToken(m_TokenString);	// ASSERT(token == TOKEND_NUMBER);
	tempNumber = (int)atoi(m_TokenString);

	return tempNumber;
}

int CASEParser::Parsing_NumberLong()
{
	LONG token;
	LONG tempNumber;

	token = m_lexer->GetToken(m_TokenString);	// ASSERT(token == TOKEND_NUMBER);
	tempNumber = strtoul(m_TokenString, NULL, 10);

	return tempNumber;
}

float CASEParser::Parsing_NumberFloat()
{
	LONG token;
	float tempNumber;

	token = m_lexer->GetToken(m_TokenString);	// ASSERT(token == TOKEND_NUMBER);
	tempNumber = (float)atof(m_TokenString);

	return tempNumber;
}

LPSTR CASEParser::Parsing_String()
{
	// ※m_TokenString(char[255])이기 때문에 CString에 넣으면 에러 날거라 생각했는데, 생각보다 CString은 잘 만들어진 것 같다. 알아서 받아들이는데?
	m_lexer->GetToken(m_TokenString);

	return m_TokenString;
}

DirectX::SimpleMath::Vector3 CASEParser::Parsing_NumberVector3()
{
	LONG token;
	Vector3 tempVector3;

	token = m_lexer->GetToken(m_TokenString);
	tempVector3.x = (float)atof(m_TokenString);

	token = m_lexer->GetToken(m_TokenString);
	tempVector3.z = (float)atof(m_TokenString);

	token = m_lexer->GetToken(m_TokenString);
	tempVector3.y = (float)atof(m_TokenString);

	// 스태틱 변수의 레퍼런스보다는 값 전달을 하자
	return tempVector3;
}

///--------------------------------------------------
/// 내부에서 뭔가를 생성, 리스트에 넣는다
///--------------------------------------------------
void CASEParser::Create_onemesh_to_list()
{
	// 메시를 하나 동적으로 생성하고, 그 포인터를 리스트에 넣는다
	Mesh* temp = new Mesh;
	m_OneMesh = temp;
	m_OneMesh->m_scenedata = m_scenedata;		// 클래스간 값 복사
	m_MeshList.push_back(m_OneMesh);
}

void CASEParser::Create_materialdata_to_list()
{
	// 매터리얼 하나를 동적으로 생성하고, 그 포인터를 리스트에 넣는다
	ASEMaterial* temp = new ASEMaterial;
	m_materialdata = temp;
	m_list_materialdata.push_back(m_materialdata);
}

void CASEParser::Create_animationdata_to_list()
{
	// 애니메이션 데이터 하나를 동적으로 생성하고, 그 포인터를 리스트에 넣는다
	Animation* temp = new Animation;
	m_animation = temp;
	m_list_animation.push_back(m_animation);
}

void CASEParser::Create_onevertex_to_list()
{
	// 정점 하나를 동적으로 생성하고, 그 포인터를 리스트에 넣는다
	Vertex* temp = new Vertex;
	m_OneMesh->m_meshvertex.push_back(temp);
}
