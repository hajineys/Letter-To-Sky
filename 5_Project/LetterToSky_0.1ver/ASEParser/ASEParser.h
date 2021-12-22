#pragma once

//------------------------------------------------------------
// ASE Parser
// Ver 2.1.20080124
// 
// ��ü������ ������ ������ ������ 
// �̰��� D3D�� Ư�� ������Ʈ�� ��ȯ�ϴ� ������ Ŀ���Ѵ�
//------------------------------------------------------------
#pragma once
#include <list>

// �������� �������ֽ� Lexer
#include "ASEFile.h"

// �ļ� ���ο��� ���� ��������
#include "ParsingData.h"
#include "Animation.h"

using namespace std;
using namespace ASEParser;

enum eObjectType
{
	eObjectType_ObjectRoot,
	eObjectType_Geomobject,
	eObjectType_Shape,
	eObjectType_Camera,
	eObjectType_Light,
	eObjectType_HelperObject,
	eObjectType_MarkerObject,
	eObjectType_SkinnedMesh,
	eObjectType_Bone,
	eObjectType_CMeshPlane,
	eObjectType_CBullet_Trace,
	eObjectType_BasicSphere,
	eObjectType_Missile,
	eObjectType_MeshBillboard,
	eObjectType_PointSprite,			// CMesh���..
	eObjectType_End,
};

// �ļ� ���ο��� Ư�� ��带 ������ �۵��Ѵ�. (�ļ��� ��ͱ��� ����)
enum eParsingmode
{
	eNone,
	eGeomobject,
	eAnimation,
	eShape,
	eCamera,
	eLight,
	eHelperObject,
	eSkin_InitTM,
	eBone_List,
	eEnd,
};

/// <summary>
/// ASE Parser
///
/// ASE�� �Ľ��ϱ� ���� Ŭ����, �⺻������ �־����� LexerŬ������ �̿��Ѵ�
/// 2021.04.12 LeHideKGIT
/// </summary>
class CASEParser
{
public:
	CASEParser();
	~CASEParser();

	// �ʱ�ȭ, �ε�
	bool Init();
	bool Load(LPSTR p_File);		// ASE������ �о, ��ü ����Ʈ�� �ִ´�

	// 1�� ��ȯ
	// CScenedata ��ȯ
	bool TranslateToD3DFormat_scene(Scenedata* pscene);

	bool Convert_Optimize_Ver01(Mesh* pMesh);	// ���ؽ�(�븻), �ؽ��� ������ ����ȭ�� �� �ش�, ������ ����
	bool Convert_Optimize_Ver02(Mesh* pMesh);	// ������ ����
	bool ConvertAll(Mesh* pMesh);				// ��� �׳� ��ȯ (���ÿ�)

	// Local TM(Local Transform Matrix) ���ϱ�
	void Get_NodeTM();
	void Get_ParentMesh();
	bool IsParentExist(Mesh* mesh);
	void Get_ParentMeshMaxCount();
	void Get_CountParent();
	void Get_LocalTM();

	// ��ū�� �����س��� ���� ��Ʈ��
private:
	char m_TokenString[256];
	int m_Token;
	int m_parsingmode;				// ���� ���, ��� ���� ���� �������� nodename�̶����.. row123, mapname���..

	// �⺻���� �о���� �����
public:
	int m_data_asciiexport;			// *3DSMAX_ASCIIEXPORT 200 - �� ó�� ������ ����ε� ���� �ǹ������� �𸣰ڰ�, �ϴ� ���ǻ� �о �������ش�
	Scenedata m_scenedata;			// ���� Scene�� �����Ͱ� �� ����ü

	// ��Ʈ���� ����
	int	m_materialcount;					// Material�� �� ��ΰ�?
	ASEMaterial* m_materialdata;			// Material�� �����Ͱ� �� ����ü
	list<ASEMaterial*> m_list_materialdata;	// Material���� ����Ʈ

	// �� ���� N���� �޽ð� ������ �� �ִ�
	Mesh* m_OneMesh;						// �޽� �ϳ��� �����Ͱ� �� ����ü
	vector<Mesh*> m_MeshList;				// �޽õ�
	int m_MaxParentMeshCount;				// �޽õ��� �θ��� �ִ� ����

	// Shape ������Ʈ�� �Ľ��� �� �ʿ��� �߰� ����
	// �ļ� ���ο��� Shape ������Ʈ�� GeomObject�� �����ϰ� ��޵ȴ�
	ShapeLine* m_nowshapeline;				// ���� �а� �ִ� Line
	ShapeVertex* m_nowshapevertex;			// ���� �а� �ִ� ShapeVertex

	// �ִϸ��̼�
	Animation* m_animation;					// �� ������Ʈ�� �ִϸ��̼� ������(�Ľ̿�)
	list<Animation*> m_list_animation;		// �� �������� ����Ʈ

public:
	//--------------------------------------------------
	// Export
	//--------------------------------------------------
	Mesh* GetMesh(int index);

public:
	//--------------------------------------------------
	//	Lexcer
	//--------------------------------------------------
private:
	ASE::ASELexer* m_lexer;

	//--------------------------------------------------
	// Parsing_DivergeRecursiveALL() ���ο��� ���� ���� �Լ���
	// (parsing�� ���� ������ �Լ���)
	//--------------------------------------------------
private:
	// �Ľ��� �ð� �Ǵ� �Ŵ��� ����Լ�, ���⼭ ��� ��ū�� ó���� �Ѵ�
	void Parsing_DivergeRecursiveALL(int depth);

	int Parsing_NumberInt();				// int
	int Parsing_NumberLong();				// long�� �о �������ش�
	float Parsing_NumberFloat();			// float
	LPSTR Parsing_String();					// string
	Vector3 Parsing_NumberVector3();		// 3���� Float�� ���� �ϳ��� �о �������ش�

	void Create_onemesh_to_list();			// �޽� �ϳ��� �������� �����ϰ�, ����Ʈ�� �ִ´�
	void Create_materialdata_to_list();		// ��Ʈ���� �ϳ��� �������� �����ϰ�, ����Ʈ�� �ִ´�
	void Create_animationdata_to_list();	// �ִϸ��̼� ������(�� ���� ������Ʈ ����) �ϳ��� �������� �����ϰ�, ����Ʈ�� �ִ´�
	void Create_onevertex_to_list();		// ���ؽ� �ϳ��� �������� �����ϰ�, ����Ʈ�� �ִ´�
};
